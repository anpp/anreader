#include "n3widget.h"
#include "choice_datetime_dialog.h"
#include "mainwindow.h"
#include <QTimer>


//----------------------------------------------------------------------------------------------------------------------
DeviceFrame::DeviceFrame(QWidget *parent) : QFrame (parent)
{
    QFormLayout *lForm = new QFormLayout;
    QVBoxLayout *lMain = new QVBoxLayout(this);
    QHBoxLayout *lClock = new QHBoxLayout;
    QHBoxLayout *lSettings = new QHBoxLayout;
    QHBoxLayout *lReadJumps = new QHBoxLayout;

    lSettings->addWidget(&tb_settings);
    lSettings->addStretch();

    tb_settings.setToolTip(tr("Device settings..."));
    tb_settings.setIconSize(QSize(32, 32));
    tb_settings.setIcon(QIcon(":/images/icons/buttons/device_settings.png"));


    lForm->setContentsMargins(0, 0, 0, 0);
    lClock->setContentsMargins(0, 0, 0, 0);
    lReadJumps->setContentsMargins(0, 0, 0, 0);
    lSettings->setContentsMargins(0, 0, 0, 0);
    lMain->setContentsMargins(n3widget_defs::spacing, lMain->contentsMargins().top(), n3widget_defs::spacing, lMain->contentsMargins().bottom());
    lMain->setSpacing(n3widget_defs::spacing);
    lReadJumps->setSpacing(n3widget_defs::spacing);
    lClock->setSpacing(n3widget_defs::spacing);

    te_sn.setReadOnly(true);
    te_total_jumps.setReadOnly(true);
    te_top_jump.setReadOnly(true);
    te_next_jump.setReadOnly(true);
    te_total_freefall_time.setReadOnly(true);
    te_total_canopy_time.setReadOnly(true);

    line_horz0.setFrameShape(QFrame::HLine);
    line_horz0.setFrameShadow(QFrame::Sunken);
    line_horz0.setFixedHeight(n3widget_defs::line_height);

    line_horz1.setFrameShape(QFrame::HLine);
    line_horz1.setFrameShadow(QFrame::Sunken);
    line_horz1.setFixedHeight(n3widget_defs::line_height);

    line_horz2.setFrameShape(QFrame::HLine);
    line_horz2.setFrameShadow(QFrame::Sunken);
    line_horz2.setFixedHeight(n3widget_defs::line_height);

    lForm->setSpacing(n3widget_defs::spacing);
    lForm->addRow("s/n", &te_sn);
    lForm->addRow(QObject::tr("Total jumps"), &te_total_jumps);
    lForm->addRow(QObject::tr("Top jump"), &te_top_jump);
    lForm->addRow(QObject::tr("Next jump"), &te_next_jump);
    lForm->addRow(QObject::tr("Freefall time"), &te_total_freefall_time);
    lForm->addRow(QObject::tr("Canopy time"), &te_total_canopy_time);

    lMain->addWidget(&line_horz0);
    lMain->addLayout(lForm);
    lMain->addWidget(&line_horz1);
    lMain->addLayout(lSettings);
    lMain->addWidget(&line_horz2);
    lMain->addLayout(lClock);
    lMain->addLayout(lReadJumps);

    sb_number.setRange(0, 0);

    pb_read_jumps.setIconSize(QSize(n3widget_defs::button_height, n3widget_defs::button_height));
    pb_read_jumps.setIcon(QIcon(":/images/icons/buttons/export_jumps.png"));
    pb_read_jumps.setToolTip(tr("Read jumps to main table"));
    pb_read_jumps.setEnabled(false);


    le_clock.setReadOnly(true);

    pb_edit_clock.setIconSize(QSize(n3widget_defs::button_height, n3widget_defs::button_height));
    pb_edit_clock.setIcon(QIcon(":/images/icons/buttons/calendar.png"));
    pb_edit_clock.setToolTip("Edit time...");

    m_set_clock_action = le_clock.addAction(QIcon(":/images/icons/buttons/accept.png"), QLineEdit::TrailingPosition);
    m_set_clock_action->setToolTip(tr("Set current time"));


    lClock->addWidget(&le_clock);
    lClock->addWidget(&pb_edit_clock);

    lReadJumps->addWidget(&sb_number);
    lReadJumps->addWidget(&pb_read_jumps);

    this->setFixedHeight(n3widget_defs::element_height * n3widget_defs::n_rows +
                         lForm->spacing() + lReadJumps->spacing() + lMain->spacing() + lClock->spacing() +
                         lMain->contentsMargins().top() + lMain->contentsMargins().bottom() +
                         lSettings->spacing() +
                         line_horz0.height() + line_horz2.height()+ line_horz2.height());
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
    m_clock_timer = std::make_unique<QTimer>();
    connect(m_clock_timer.get(), &QTimer::timeout, this, &N3Widget::clockUpdate, Qt::QueuedConnection);

    connect(this, &DWidget::newTextOfState, MainWindow::instance(), &MainWindow::setStatusText);
    connect(this, &DWidget::setProgress, MainWindow::instance(), &MainWindow::initProgress);
    connect(this, &DWidget::stepProgress, MainWindow::instance(), &MainWindow::stepProgress);
    connect(this, &DWidget::receivedData, MainWindow::instance(), &MainWindow::finish);
    connect(this, &DWidget::afterConnect, MainWindow::instance(), &MainWindow::afterConnect);
    connect(this, &DWidget::controls_is_enabled, MainWindow::instance(), &MainWindow::enableActions);
    connect(this, &DWidget::log, MainWindow::instance(), &MainWindow::log);
    connect(this, &DWidget::giveLastJump, MainWindow::instance(), &MainWindow::takeLastJump);

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

    m_clock_timer->stop();

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
    connect(device_frame->m_set_clock_action, &QAction::triggered, this, &N3Widget::set_current_datetime);
    connect(&device_frame->pb_edit_clock, &QPushButton::clicked, this, &N3Widget::choice_datetime);

    connect(this, &N3Widget::controls_is_enabled, &device_frame->pb_read_jumps, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->pb_edit_clock, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->le_clock, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->sb_number, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->tb_settings, &QPushButton::setEnabled);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::deleteDeviceFrame()
{    
    if(device_frame == nullptr) return;

    disconnect(&device_frame->pb_read_jumps, &QPushButton::clicked, this, &N3Widget::read_jumps);
    disconnect(device_frame->m_set_clock_action, &QAction::triggered, this, &N3Widget::set_current_datetime);
    disconnect(&device_frame->pb_edit_clock, &QPushButton::clicked, this, &N3Widget::choice_datetime);

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
    m_device->read_datetime();
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
        switch (m_device->state()) {
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
        emit afterConnect(*this);

        qDebug() << settings.data().toHex();

        qDebug() << static_cast<int>(settings.altitudeMeasure());
        qDebug() << static_cast<int>(settings.speedMeasure());
        qDebug() << static_cast<int>(settings.temperatureMeasure());
        qDebug() << settings.dislpayIsFlipped();
        qDebug() << settings.logbookEnabled();
        qDebug() << settings.timeFormat();
        qDebug() << static_cast<int>(settings.dateFormat());
        qDebug() << settings.canopyDisplayEnabled(); //Not Scaled = 1
        qDebug() << static_cast<int>(settings.climbDisplayMode());
        qDebug() << static_cast<int>(settings.canopyAlarmsMode());


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

        int last_jump = 0;
        emit giveLastJump(last_jump);
        device_frame->sb_number.setValue(summary.topJumpNumber() - last_jump);

        m_clock_timer->start(1000);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::read_jumps()
{
    if(device_frame != nullptr)
        read_last_jumps(static_cast<unsigned int>(device_frame->sb_number.value()));
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::clockUpdate()
{
    if(!device_frame)
    {
        m_clock_timer->stop();
        return;
    }
    device_frame->le_clock.setText(m_device->dateTime().toString("dd.MM.yyyy hh:mm"));
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::set_current_datetime()
{
    QDateTime dt;
    dt = QDateTime::currentDateTime();
    if(m_device)
        m_device->set_date_time(dt);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::choice_datetime()
{
    if(!m_device)
        return;

    std::unique_ptr<ChoiceDateTimeDialog> cdtDialog = std::make_unique<ChoiceDateTimeDialog>(m_device->dateTime(), this);
    if(QDialog::Accepted == cdtDialog->exec())
    {
        if(m_device)
            m_device->set_date_time(cdtDialog->datetime());
    }

}



