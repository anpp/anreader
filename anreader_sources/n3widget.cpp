#include "n3widget.h"

//----------------------------------------------------------------------------------------------------------------------
DeviceFrame::DeviceFrame(QWidget *parent) : QFrame (parent)
{
    QFormLayout *lForm = new QFormLayout;
    QVBoxLayout *lMain = new QVBoxLayout(this);
    QHBoxLayout *lButtons = new QHBoxLayout;

    te_sn.setReadOnly(true);
    te_total_jumps.setReadOnly(true);
    te_top_jump.setReadOnly(true);
    te_next_jump.setReadOnly(true);
    te_total_freefall_time.setReadOnly(true);
    te_total_canopy_time.setReadOnly(true);

    lForm->setContentsMargins(0, 0, 0, 0);
    lForm->setSpacing(n3widget_defs::spacing);
    lForm->addRow("s/n", &te_sn);
    lForm->addRow(QObject::tr("Total jumps"), &te_total_jumps);
    lForm->addRow(QObject::tr("Top jump"), &te_top_jump);
    lForm->addRow(QObject::tr("Next jump"), &te_next_jump);
    lForm->addRow(QObject::tr("Freefall time"), &te_total_freefall_time);
    lForm->addRow(QObject::tr("Canopy time"), &te_total_canopy_time);
    lMain->addLayout(lForm);
    lMain->addLayout(lButtons);

    sb_number.setRange(0, 0);
    sb_number.setEnabled(false);

    pb_read_jumps.setText(QObject::tr("Read"));
    pb_read_jumps.setEnabled(false);

    lButtons->addWidget(&sb_number);
    lButtons->addWidget(&pb_read_jumps);

    this->setFixedHeight(n3widget_defs::element_height * n3widget_defs::n_rows + lForm->spacing() + lButtons->spacing() + lMain->spacing());
}




//=================================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
N3Widget::N3Widget(dtype device_type, QWidget *parent) : DWidget (device_type, parent)
{
    stateChanged();
}

//----------------------------------------------------------------------------------------------------------------------
N3Widget::N3Widget(QWidget *parent) : DWidget (dtype::N3, parent)
{    
    stateChanged();
}

//----------------------------------------------------------------------------------------------------------------------
N3Widget::~N3Widget()
{

}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::open()
{
    if(!m_device) return;

    if(m_device->state() == AbstractDevice::DeviceStates::Disconnected)
    {
        m_device->open();
        read_summary_settings();
    }
}


//----------------------------------------------------------------------------------------------------------------------
void N3Widget::close()
{
    if(!m_device) return;

    if(m_device->state() == AbstractDevice::DeviceStates::Error)
        m_device->destroy_port();
    else
        m_device->end_communication();
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::create()
{
    if(!m_device)
    {
        m_device = std::make_unique<Neptune_HiL>(getPortName(), this);
        setupConnects();
    }
}


//----------------------------------------------------------------------------------------------------------------------
void N3Widget::setupConnects()
{
    DWidget::setupConnects();
}


//----------------------------------------------------------------------------------------------------------------------
void N3Widget::addDeviceFrame()
{
    if(device_frame != nullptr) return;

    makeFrame();
    lMain->addWidget(device_frame);
    this->resize(this->width(), this->height() + device_frame->height());

    emit setHeight(this->height());

    connect(&device_frame->pb_read_jumps, &QPushButton::clicked, this, &N3Widget::read_jumps);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->pb_read_jumps, &QPushButton::setEnabled);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::deleteDeviceFrame()
{
    if(device_frame == nullptr) return;

    delete device_frame;
    device_frame = nullptr;

    this->resize(this->width(), wtop->height());

    emit setHeight(this->height());
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::makeFrame()
{
    device_frame = new DeviceFrame(this);
    device_frame->te_sn.setText(m_device->getSerialNumber());
}


//----------------------------------------------------------------------------------------------------------------------
void N3Widget::read_summary_settings()
{
    if(!m_device) return;

    disconnect(m_device.get(), &Neptune::allCommandsComplete, nullptr, nullptr);
    connect(m_device.get(), &Neptune::allCommandsComplete, this, &N3Widget::readed_summary_settings, Qt::QueuedConnection);

    m_device->read_summary_jumps();
    m_device->read_settings();
    m_device->read_dropzones();
    m_device->read_airplanes();
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::read_last_jumps(unsigned int n_jumps)
{
    if(!m_device) return;

    const ASummaryInfo &summary = m_device->summary();

    n_jumps = (n_jumps > summary.totalJumps()) ? summary.totalJumps() : n_jumps;

    //сигнал количества итераций от девайса отключен, так как девайс вызывается несколько раз
    disconnect(m_device.get(), &AbstractDevice::setProgress, nullptr , nullptr);
    emit setProgress(getId(), m_device->n_iterations_by_jumps(n_jumps));

    unsigned int firstJumpOffset_1 =  (summary.lastJumpOffset() > n_jumps) ? summary.lastJumpOffset() - n_jumps : 0;
    unsigned int firstJumpOffset_2 = (n_jumps > summary.lastJumpOffset()) ? summary.totalJumps() - (n_jumps - summary.lastJumpOffset()) : summary.totalJumps();

    unsigned int num_jumps_1 = (summary.lastJumpOffset() < n_jumps) ? summary.lastJumpOffset() : n_jumps;
    unsigned int num_jumps_2 = n_jumps - num_jumps_1;

    connect(m_device.get(), &AbstractDevice::allCommandsComplete, this, &DWidget::slotReceivedData, Qt::QueuedConnection);
    //disconnect в родительском классе в слоте slotReceivedData

    m_device->clearJumps();
    m_device->read_details_jumps(firstJumpOffset_1, num_jumps_1);
    if(num_jumps_2 > 0)
        m_device->read_details_jumps(firstJumpOffset_2, num_jumps_2);
}


//----------------------------------------------------------------------------------------------------------------------
void N3Widget::stateChanged()
{    
    DWidget::stateChanged();

    if(getPortName() == "")
    {
        setImage("n3_off.png");
        deleteDeviceFrame();
        deleteConnectButton();
    }
    else
    {
        setImage("n3_com.png");
        addConnectButton();
    }

    if(m_device)
    {
        AbstractDevice::DeviceStates state = m_device->state();
        switch (state) {
        case AbstractDevice::DeviceStates::Connected:
            setImage("n3_com2.png");
            break;
        case AbstractDevice::DeviceStates::Disconnected:
            setImage("n3_com.png");
            deleteDeviceFrame();
            break;
        case AbstractDevice::DeviceStates::Initializing:
            setImage("n3_on_com.png");            
            break;
        case AbstractDevice::DeviceStates::Processing:
            setImage("n3_refresh.png");
            break;
        case AbstractDevice::DeviceStates::Receiving:
            setImage("n3_refresh.png");
            break;
        case AbstractDevice::DeviceStates::Ready:
            setImage("n3_ok.png");
            addDeviceFrame();
            break;
        case AbstractDevice::DeviceStates::Error:
            setImage("n3_error.png");
            break;
        default:
           setImage("n3_off.png");
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::readed_summary_settings()
{
    if(!m_device) return;

    disconnect(m_device.get(), &Neptune::allCommandsComplete, nullptr, nullptr);

    if(device_frame != nullptr)
    {

        const ADeviceSettings &settings = m_device->settings();
/*
        qDebug() << settings.altitudeMeasure();
        qDebug() << settings.speedMeasure();
        qDebug() << settings.temperatureMeasure();
        qDebug() << settings.dislpayIsFlipped();
        qDebug() << settings.logbookEnabled();
        qDebug() << settings.timeFormat();
        qDebug() << settings.dateFormat();
        qDebug() << settings.canopyDisplayEnabled();
        qDebug() << settings.climbDisplayMode();
*/

        auto calc_time { [] (uint32_t secs) -> QString
            {
                uint32_t hour = secs / 3600;
                uint32_t min = (secs - hour * 3600) / 60;
                uint32_t sec = secs - hour * 3600 - min * 60 ;
                return QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec);
            }};

        const ASummaryInfo &summary = m_device->summary();
        device_frame->te_total_jumps.setText(QString::number(summary.totalJumps()));
        device_frame->te_top_jump.setText(QString::number(summary.topJumpNumber()));
        device_frame->te_next_jump.setText(QString::number(summary.nextJumpNumber()));
        device_frame->te_total_freefall_time.setText(calc_time(summary.totalFreeFallTime()));
        device_frame->te_total_canopy_time.setText(calc_time(summary.totalCanopyTime()));

        device_frame->sb_number.setRange(0, summary.totalJumps());
        device_frame->sb_number.setEnabled(true);

        int last_jump = 0;
        emit giveLastJump(last_jump);
        device_frame->sb_number.setValue(summary.topJumpNumber() - last_jump);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::read_jumps()
{
    if(device_frame != nullptr)
        read_last_jumps(static_cast<unsigned int>(device_frame->sb_number.value()));
}



