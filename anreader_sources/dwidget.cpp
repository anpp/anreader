#include "dwidget.h"

int DWidget::s_idGen = 0;

//----------------------------------------------------------------------------------------------------------------------
DWidget::DWidget(QWidget *parent) : QFrame(parent)
{
    init();
}

//----------------------------------------------------------------------------------------------------------------------
DWidget::DWidget(dtype device_type, QWidget *parent) : QFrame(parent)
{
    this->device_type = device_type;
    init();
}

//----------------------------------------------------------------------------------------------------------------------
DWidget::~DWidget()
{
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::init()
{
    m_id = s_idGen++;
    connect_button.setVisible(false);

    wtop = new QWidget(this);

    lMain = new QVBoxLayout(this);
    lMain->setContentsMargins(dw_consts::base_margin, dw_consts::base_margin, 0, 0);

    lMain->addWidget(wtop);

    lTopHorz = new QHBoxLayout();
    lTopVert = new QVBoxLayout();
    lTopVert->setContentsMargins(0, 0, 0, dw_consts::base_margin);

    lTopHorz->addWidget(&lblIcon);
    lTopHorz->addWidget(&lblPort);
    lTopHorz->addStretch();

    lTopVert->addLayout(lTopHorz);
    wtop->setLayout(lTopVert);

    lConnectButon = new QHBoxLayout();    

    lConnectButon->addWidget(&connect_button);
    lConnectButon->setContentsMargins(0, 0, 0, dw_consts::base_margin);
    lConnectButon->addStretch();

    lTopVert->addStretch();
    lTopVert->addLayout(lConnectButon);

    lMain->setSpacing(dw_consts::base_spacing);
    lTopVert->setSpacing(dw_consts::base_spacing);

    m_wtop_height = lMain->spacing() + lTopVert->spacing() + lTopHorz->spacing() + lConnectButon->spacing();
}


//----------------------------------------------------------------------------------------------------------------------
void DWidget::setVisibleCB(const bool value)
{
    is_visible_connect_button = value;
    connect_button.setVisible(is_visible_connect_button);
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::setupConnects()
{
    if(m_device)
    {
        connect(m_device.get(), &AbstractDevice::changedState, this, &DWidget::stateChanged);
        connect(m_device.get(), &AbstractDevice::setProgress, [this] (unsigned int value) {emit setProgress(getId(), value); });
        connect(m_device.get(), &AbstractDevice::stepProgress, [this] () {emit stepProgress(getId()); });
        connect(m_device.get(), &AbstractDevice::log, this, &DWidget::slotLoger);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::addConnectButton()
{
    if(is_visible_connect_button) return;

    setVisibleCB(true);
    if(!this->isVisible()) //костыль - кнопка имеет огромные размера на невидимом виджете
        connect_button.setFixedHeight(dw_consts::connect_button_height);

    wtop->setFixedHeight(m_wtop_height + m_icon_height + connect_button.height());
    this->resize(this->width(), wtop->height());
    emit setHeight(this->height());
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::deleteConnectButton()
{    
    if(!is_visible_connect_button) return;

    setVisibleCB(false);
    wtop->setFixedHeight(m_wtop_height + m_icon_height);
    this->resize(this->width(), wtop->height());    
    emit setHeight(this->height());
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::setButton(bool bconnect)
{
    connect_button.setText(bconnect ? QObject::tr("Connect") : QObject::tr("Disconnect"));
    connect_button.disconnect();
    if(bconnect)
        connect(&connect_button, &QPushButton::clicked, this, &DWidget::slotOpen);
    else
        connect(&connect_button, &QPushButton::clicked, this, &DWidget::slotClose);
}

void DWidget::setIconHeight(const int icon_height)
{
    if(m_icon_height <= 0 && wtop)
    {
        m_icon_height = icon_height;
        wtop->setFixedHeight(m_wtop_height + m_icon_height);
        this->resize(this->width(), wtop->height());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::stateChanged()
{
    if(!m_device) return;

    if(m_device->state() != AbstractDevice::DeviceStates::Unk)
    {
        emit newTextOfState(getId(), m_device->deviceName() + " (" + m_device->comPort() + "): " + m_device->state_str() + (m_device->state() == AbstractDevice::DeviceStates::Error? ": " + m_device->getErrorMessage() : ""));
    }

    AbstractDevice::DeviceStates state = m_device->state();

    connect_button.setEnabled(AbstractDevice::DeviceStates::Error == state || AbstractDevice::DeviceStates::Ready == state || AbstractDevice::DeviceStates::Disconnected == state);
    setButton(state == AbstractDevice::DeviceStates::Disconnected);
    emit controls_is_enabled(!(AbstractDevice::DeviceStates::Processing == state || AbstractDevice::DeviceStates::Receiving == state));
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::slotReceivedData()
{
    disconnect(m_device.get(), &AbstractDevice::allCommandsComplete, nullptr, nullptr);
    connect(m_device.get(), &AbstractDevice::setProgress, [this] (unsigned int value) {emit setProgress(getId(), value); });
    if(m_device)
        emit receivedData(*this);
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::setImage(const QString &image_name)
{
    QPixmap pxm;
    QString path = ":/images/";

    if(device_type == dtype::N3)
        path += "N3/";

    pxm.load(path + image_name);
    setIconHeight(pxm.height());
    lblIcon.setPixmap(pxm);
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::setPortInfo(const QSerialPortInfo& port_info)
{
    this->port_info = std::make_unique<QSerialPortInfo>(port_info);
    lblPort.setText(getPortName());
    stateChanged();    
}

//----------------------------------------------------------------------------------------------------------------------
const QString DWidget::getPortName() const
{
    return (!port_info ? "": port_info->portName());
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::clear()
{
    connect_button.disconnect();

    port_info.reset();
    m_device.reset();

    lblPort.setText(getPortName());
    stateChanged();
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::slotOpen()
{
    open();
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::slotClose()
{
    close();
    emit setProgress(getId(), 1);
}

//----------------------------------------------------------------------------------------------------------------------
void DWidget::slotLoger(const QString &value)
{
    emit log(value);
}



