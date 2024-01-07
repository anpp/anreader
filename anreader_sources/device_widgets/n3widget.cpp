#include "n3widget.h"
#include "device/n3names.h"
#include "device/n3devicesettings.h"
#include "device/neptune_hil.h"
#include "choice_datetime_dialog.h"
#include "mainwindow.h"
#include "n3_main_settings_dialog.h"
#include "n3_names_dialog.h"
#include <QTimer>
#include <QCheckBox>


const static QString ExpiredNames[] =
    {QObject::tr("Fixing jumps's dates for N3 software revision 3 (2017-08 expired)"),
     QObject::tr("Fixing jumps's dates for N3 software revision 4 (2025-08 expired)"),
     QObject::tr("Fixing jumps's dates for Atlas software revision 3 (2025-08 expired)"),
     QObject::tr("Fixing jumps's dates for Atlas (2028-04 expired)"),
     QObject::tr("Fixing jumps's dates for Atlas 2 software revision 5 (2025-08 expired)"),
     };


//----------------------------------------------------------------------------------------------------------------------
DeviceFrame::DeviceFrame(QWidget *parent, ExpiredType et) : QFrame (parent), m_et(et)
{
    QFormLayout *lForm = new QFormLayout;
    QVBoxLayout *lMain = new QVBoxLayout(this);
    QHBoxLayout *lClock = new QHBoxLayout;
    QHBoxLayout *lSettings = new QHBoxLayout;
    QHBoxLayout *lReadJumps = new QHBoxLayout;

    lSettings->addWidget(&tb_settings);
    lSettings->addWidget(&tb_dropzones);
    lSettings->addWidget(&tb_aircrafts);
    lSettings->addWidget(&tb_alarms);
    lSettings->addStretch();

    tb_settings.setToolTip(tr("Device settings..."));
    tb_settings.setIconSize(QSize(32, 32));
    tb_settings.setIcon(QIcon(":/images/icons/buttons/device_settings.png"));    

    tb_dropzones.setToolTip(tr("Dropzones..."));
    tb_dropzones.setIconSize(QSize(32, 32));
    tb_dropzones.setIcon(QIcon(":/images/icons/buttons/dz.png"));

    tb_aircrafts.setToolTip(tr("Aircrafts..."));
    tb_aircrafts.setIconSize(QSize(32, 32));
    tb_aircrafts.setIcon(QIcon(":/images/icons/buttons/airplane.png"));

    tb_alarms.setToolTip(tr("Alarms..."));
    tb_alarms.setIconSize(QSize(32, 32));
    tb_alarms.setIcon(QIcon(":/images/icons/buttons/alarms.png"));

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

    if(ExpiredType::None != m_et)
    {
        m_cbCorrectDate = new QCheckBox(tr("Fixing jumps's dates"), this);
        m_cbCorrectDate->setToolTip(tr(ExpiredNames[et].toStdString().c_str()));
        m_cbCorrectDate->setChecked(true);
        lMain->addWidget(m_cbCorrectDate);
    }

    this->setFixedHeight(n3widget_defs::element_height * n3widget_defs::n_rows +
                         lForm->spacing() + lReadJumps->spacing() + lMain->spacing() + lClock->spacing() +
                         lMain->contentsMargins().top() + lMain->contentsMargins().bottom() +
                         lSettings->spacing() +
                         line_horz0.height() + line_horz2.height()+ line_horz2.height() +
                         (m_cbCorrectDate? m_cbCorrectDate->height() : 0));
}

//----------------------------------------------------------------------------------------------------------------------
int DeviceFrame::calcCorrectKoeff() const
{
    int koeff = 0;

    int month_diff = 0;

    if(m_cbCorrectDate)
        if(m_cbCorrectDate->checkState() == Qt::CheckState::Checked)
        {
            switch(m_et)
            {
            case ExpiredType::OldN3:
                month_diff = (QDate::currentDate().year() - 2007) * 12 + QDate::currentDate().month();
                break;
            case ExpiredType::N3:
                month_diff = (QDate::currentDate().year() - 2015) * 12 + QDate::currentDate().month();
                break;
            case ExpiredType::Atlas:
                month_diff = ((QDate::currentDate().year() - 2007) * 12) + 128 + QDate::currentDate().month();
                break;
            case ExpiredType::AtlasRev3:
                month_diff = (QDate::currentDate().year() - 2015) * 12 + QDate::currentDate().month();
                break;
            case ExpiredType::Atlas2Rev5:
                month_diff = (QDate::currentDate().year() - 2015) * 12 + QDate::currentDate().month();
                break;

            default:
                koeff = 0;
                break;
            }
            if(month_diff > 0)
                koeff = month_diff / 128;
        }
    return koeff;
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
    connect(&device_frame->tb_settings, &QToolButton::clicked, this, &N3Widget::N3Settings);
    connect(&device_frame->tb_dropzones, &QToolButton::clicked, this, &N3Widget::N3Dropzones);
    connect(&device_frame->tb_aircrafts, &QToolButton::clicked, this, &N3Widget::N3Airplanes);
    connect(&device_frame->tb_alarms, &QToolButton::clicked, this, &N3Widget::N3Alarms);

    connect(this, &N3Widget::controls_is_enabled, &device_frame->pb_read_jumps, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->pb_edit_clock, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->le_clock, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->sb_number, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->tb_settings, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->tb_dropzones, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->tb_aircrafts, &QPushButton::setEnabled);
    connect(this, &N3Widget::controls_is_enabled, &device_frame->tb_alarms, &QPushButton::setEnabled);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::deleteDeviceFrame()
{    
    if(device_frame == nullptr) return;

    disconnect(&device_frame->pb_read_jumps, &QPushButton::clicked, this, &N3Widget::read_jumps);
    disconnect(device_frame->m_set_clock_action, &QAction::triggered, this, &N3Widget::set_current_datetime);
    disconnect(&device_frame->pb_edit_clock, &QPushButton::clicked, this, &N3Widget::choice_datetime);
    disconnect(&device_frame->tb_settings, &QToolButton::clicked, this, &N3Widget::N3Settings);
    disconnect(&device_frame->tb_dropzones, &QToolButton::clicked, this, &N3Widget::N3Dropzones);
    disconnect(&device_frame->tb_aircrafts, &QToolButton::clicked, this, &N3Widget::N3Airplanes);
    disconnect(&device_frame->tb_alarms, &QToolButton::clicked, this, &N3Widget::N3Alarms);

    delete device_frame;
    device_frame = nullptr;

    this->resize(this->width(), wtop->height());

    emit setHeight(this->height());
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::makeFrame()
{
    if(!m_device) return;

    DeviceFrame::ExpiredType et = DeviceFrame::ExpiredType::None;

    //вычисление - надо ли показывать галочку коррекции даты для разных приборов
    switch(static_cast<N3Types>(m_device->product_type()))
    {
    case N3Types::N3:
        if((m_device->revision() < 4) && (QDate::currentDate() >= QDate(2017, 8, 1)))
            et = DeviceFrame::ExpiredType::OldN3;
        if((m_device->revision() == 4) && (QDate::currentDate() >= QDate(2025, 8, 1)))
            et = DeviceFrame::ExpiredType::N3;
        break;
    case N3Types::Atlas:
        if((m_device->revision() == 3) && (QDate::currentDate() >= QDate(2025, 8, 1)))
            et = DeviceFrame::ExpiredType::AtlasRev3;
        else
        if(QDate::currentDate() >= QDate(2028, 4, 1))
            et = DeviceFrame::ExpiredType::Atlas;
        break;
    case N3Types::Atlas2:
        if((m_device->revision() == 5) && (QDate::currentDate() >= QDate(2025, 8, 1)))
            et = DeviceFrame::ExpiredType::Atlas2Rev5;
        break;

    default:
        et = DeviceFrame::ExpiredType::None;
        break;
    }
    device_frame = new DeviceFrame(this, et);
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
void N3Widget::N3Names_dialog(N3Names &names)
{
    QString title = (names.type() == N3NamesType::Airplanes ? "Airplanes" :
                                                              (names.type() == N3NamesType::Dropzones ? "Dropzones" : "Alarms"));
    std::unique_ptr<N3NamesDialog> nd = std::make_unique<N3NamesDialog>(title, names, this);
    connect(this, &N3Widget::device_disconnect, nd.get(), &QDialog::close, Qt::QueuedConnection);

    if(nd->exec() == QDialog::Accepted)
    {
        if(nd->isChanged() && m_device)
        {
            bool is_changed_current = nd->isChangedCurrentName();
            bool is_changed_data = nd->isChangedData();

            if(is_changed_data)
                names.data() = nd->new_n3names().data_const();

            switch(names.type())
            {
            case N3NamesType::Airplanes:
                if(is_changed_current)
                {
                    m_device->airplanes().setActive(nd->new_n3names().active_index());
                    m_device->summary().setAPIndex(nd->new_n3names().active_index());
                    m_device->write_summary_jumps();
                }

                if(is_changed_data)
                    m_device->write_airplanes();
                break;
            case N3NamesType::Dropzones:
                if(is_changed_current)
                {
                    m_device->dropzones().setActive(nd->new_n3names().active_index());
                    m_device->summary().setDZIndex(nd->new_n3names().active_index());
                    m_device->write_summary_jumps();
                }

                if(is_changed_data)
                    m_device->write_dropzones();
                break;
            default:
                break;
            }
        }
    }
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
        emit device_disconnect();
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
            emit device_disconnect();
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
        case AbstractDevice::DeviceStates::Sending:
            setImage("n3_refresh.png");
            break;
        case AbstractDevice::DeviceStates::Ready:
            setImage("n3_ok.png");
            addDeviceFrame();
            break;
        case AbstractDevice::DeviceStates::Error:
            setImage("n3_error.png");
            emit device_disconnect();
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
        emit afterConnect(*this);

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

        static_cast<N3Names&>(m_device->airplanes()).initAfterLoad();
        static_cast<N3Names&>(m_device->dropzones()).initAfterLoad();

        m_device->airplanes().setActive(summary.currentAPIndex());
        m_device->dropzones().setActive(summary.currentDZIndex());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::read_jumps()
{
    if(device_frame != nullptr)
    {
        ((Neptune*)(m_device.get()))->setCorrectDateKoeff(device_frame->calcCorrectKoeff());
        read_last_jumps(static_cast<unsigned int>(device_frame->sb_number.value()));
    }
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
    if(!m_device) return;

    std::unique_ptr<ChoiceDateTimeDialog> cdtDialog = std::make_unique<ChoiceDateTimeDialog>(m_device->dateTime(), this);
    connect(this, &N3Widget::device_disconnect, cdtDialog.get(), &QDialog::close, Qt::QueuedConnection);

    if(QDialog::Accepted == cdtDialog->exec())
    {
        if(m_device)
            m_device->set_date_time(cdtDialog->datetime());
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::N3Settings()
{
    if(!m_device) return;

    std::unique_ptr<N3MainSettingsDialog> sd = std::make_unique<N3MainSettingsDialog>(static_cast<const N3DeviceSettings&>(m_device->settings()), this);
    connect(this, &N3Widget::device_disconnect, sd.get(), &QDialog::close, Qt::QueuedConnection);

    if(sd->exec() == QDialog::Accepted)
    {
        if(sd->isChanged() && m_device)
        {
            m_device->settings().data() = sd->new_settings().data();
            m_device->write_settings();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::N3Dropzones()
{
    if(!m_device) return;
    N3Names_dialog(static_cast<N3Names&>(m_device->dropzones()));
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::N3Airplanes()
{
    if(!m_device) return;    
    N3Names_dialog(static_cast<N3Names&>(m_device->airplanes()));
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::N3Alarms()
{
    if(!m_device) return;

    if(m_device->alarms().count() > 0)
        edit_alarms();
    else
    {
        connect(m_device.get(), &Neptune::allCommandsComplete, this, &N3Widget::edit_alarms, Qt::QueuedConnection);
        m_device->read_alarms();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Widget::edit_alarms()
{
    if(!m_device) return;
    disconnect(m_device.get(), &Neptune::allCommandsComplete, nullptr, nullptr);
    static_cast<N3Names&>(m_device->alarms()).initAfterLoad();
    N3Names_dialog(static_cast<N3Names&>(m_device->alarms()));
}




