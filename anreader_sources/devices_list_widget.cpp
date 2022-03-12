#include "devices_list_widget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSerialPortInfo>

//----------------------------------------------------------------------------------------------------------------------
void WorkerEnumerator::stop()
{
    isEnumerated = false;
}

//----------------------------------------------------------------------------------------------------------------------
void WorkerEnumerator::enumerate()
{
    isEnumerated = true;
    QEventLoop event_loop;

    while(isEnumerated)
    {
        //auto new_ports = QSerialPortInfo::availablePorts().toStdList(); //deprecated
        auto availablePorts = QSerialPortInfo::availablePorts();
        auto new_ports = std::list<QSerialPortInfo>(availablePorts.begin(), availablePorts.end());

        //поиск добавленных
        for (const QSerialPortInfo &new_port_info : new_ports)
        {
            auto found = std::find_if(ports.begin(), ports.end(), [&new_port_info] (QSerialPortInfo &port_info)
            {
                return port_info.portName() == new_port_info.portName();
            });
            if(found == ports.end())
            {                
                ports.push_back(new_port_info);
                emit attachDevice(new_port_info);
            }
        }

        //поиск удаленных
        {
            QStringList detachedList;

            for (const QSerialPortInfo &port_info : ports)
            {
                auto found = std::find_if(new_ports.begin(), new_ports.end(), [&port_info] (QSerialPortInfo &new_port_info)
                {
                    return port_info.portName() == new_port_info.portName();
                });
                if(found == new_ports.end())
                {                    
                    detachedList.append(port_info.portName());
                    emit detachDevice(port_info);
                }
            }
                for(const auto& portName : detachedList)
                    ports.remove_if([portName](QSerialPortInfo &element) { return portName == element.portName(); });
        }

        QThread::msleep(Enumerator::DelayMs);
        event_loop.processEvents();
    }
    emit finished();
}



//=========================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
DevicesWidget::DevicesWidget(const Settings& asettings, QWidget *parent) :
    QWidget(parent),
    settings(asettings)
{
    qRegisterMetaType <QSerialPortInfo>();
    qRegisterMetaType <QList<QSerialPortInfo>>();

    init();
    start();
}

//----------------------------------------------------------------------------------------------------------------------
DevicesWidget::~DevicesWidget()
{    
    emit stopEnumerate();
    threadEnumerator.wait();
}

//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::start()
{    
    threadEnumerator.start();
}

//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::createDefaults()
{
    createDevice(new N3Widget());
}

//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::init()
{
    worker.moveToThread(&threadEnumerator);

    connect(&threadEnumerator, &QThread::started, &worker, &WorkerEnumerator::enumerate);
    connect(&worker, &WorkerEnumerator::finished, &threadEnumerator, &QThread::quit, Qt::DirectConnection);
    connect(&worker, &WorkerEnumerator::attachDevice, this, &DevicesWidget::attach);
    connect(&worker, &WorkerEnumerator::detachDevice, this, &DevicesWidget::detach);
    connect(this, &DevicesWidget::stopEnumerate, &worker, &WorkerEnumerator::stop);


    auto layout = new QVBoxLayout;

    table_devices.setFrameStyle(QFrame::NoFrame);
    table_devices.setStyleSheet("QTableWidget{selection-background-color: transparent; background-color: transparent; border-top: 1px solid gray}");
    table_devices.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    table_devices.setColumnCount(1);    
    table_devices.setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_devices.verticalHeader()->hide();
    table_devices.horizontalHeader()->hide();


    table_devices.horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(&table_devices);
    this->setLayout(layout);

    this->setMinimumWidth(DevicesWidget_defs::default_width);
}

//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::createDevice(ptrWidget device_widget)
{
    connect(device_widget, &DWidget::newTextOfState, this, &DevicesWidget::newTextOfState);
    connect(device_widget, &DWidget::setProgress, this, &DevicesWidget::setProgress);
    connect(device_widget, &DWidget::stepProgress, this, &DevicesWidget::stepProgress);
    connect(device_widget, &DWidget::setHeight, this, &DevicesWidget::setHeightRow);
    connect(device_widget, &DWidget::receivedData, this, &DevicesWidget::receivedData);
    connect(device_widget, &DWidget::afterConnect, this, &DevicesWidget::afterConnect);
    connect(device_widget, &DWidget::controls_is_enabled, this, &DevicesWidget::controls_is_enabled);
    connect(device_widget, &DWidget::log, this, &DevicesWidget::log);
    connect(device_widget, &DWidget::giveLastJump, this, &DevicesWidget::giveLastJump);

    devices.emplace_back(device_widget);
    int row_count = table_devices.rowCount();

    table_devices.setRowCount(row_count + 1);
    table_devices.setRowHeight(row_count, device_widget->height());    

    table_devices.setIndexWidget(table_devices.model()->index(row_count, 0), device_widget);

    emit createStatusFrame(device_widget->getId());
}



//----------------------------------------------------------------------------------------------------------------------
dtype DevicesWidget::typeByDescription(const QString &description) const
{
    dtype type = dtype::unk;

    for(const auto& item: settings.map_set(kindset::device_types))
    {
        QString device_string_value = item.second->getValue().toString();
        QStringList values = device_string_value.split(";");

        for(auto& s: values)
            if(s.trimmed() == description)
            {
                type = DWidget::typeByName(item.first);
                if(dtype::unk != type)
                    break;
            }
    }
    //return type;
    return (type == dtype::Atlas ? dtype::N3: type);
}



//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::attach(QSerialPortInfo port_info)
{
    emit log("Attached: " + port_info.description() + " " + port_info.portName());
#ifdef Q_OS_MACOS
    if(port_info.portName().contains("cu."))
        return;
#endif
    dtype type = typeByDescription(port_info.description());

    auto device_widget = std::find_if(devices.begin(), devices.end(), [type] (const auto& widget) { return (type == widget->getDeviceType() && widget->getPortName() == ""); });
    if(device_widget != devices.end()){
        (*device_widget)->setPortName(port_info.portName());
        (*device_widget)->create();
    }
    else
    {
        if(type == dtype::N3)
        {
            ptrWidget new_widget = new N3Widget();
            createDevice(new_widget);
            new_widget->setPortName(port_info.portName());
            new_widget->create();
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::detach(QSerialPortInfo port_info)
{    
    emit log("Detached: " + port_info.description() + " " + port_info.portName());

    auto device_widget = std::find_if(devices.begin(), devices.end(), [&port_info] (const auto& widget) { return port_info.portName() == widget->getPortName(); });
    if(device_widget != devices.end())
        (*device_widget)->clear();
}


//----------------------------------------------------------------------------------------------------------------------
void DevicesWidget::setHeightRow(int height)
{
    DWidget* widget = qobject_cast<DWidget*>(sender());
    if(widget != nullptr)
    {
        QModelIndex index = table_devices.indexAt(widget->pos());
        table_devices.setRowHeight(index.row(), height);
    }
}
