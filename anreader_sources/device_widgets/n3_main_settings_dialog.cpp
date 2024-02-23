#include "n3_main_settings_dialog.h"
#include "ui_n3_main_settings_dialog.h"
#include "device/n3devicesettings.h"

#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
#include "common/common.h"
#endif

#include <QPushButton>


//-------------------------------------------------------------------------------------
N3MainSettingsDialog::N3MainSettingsDialog(const N3DeviceSettings& n3settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::N3MainSettingsDialog),
    m_n3settings(n3settings)
{
    ui->setupUi(this);

    m_title = windowTitle();

    m_new_n3settings = std::make_unique<N3DeviceSettings>();
    m_new_n3settings->data() = m_n3settings.data();

    fillComboBoxes();
    initComboBoxes();

    connect(this, &N3MainSettingsDialog::changed, ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok), &QPushButton::setEnabled);
    emit changed(false);
}

//-------------------------------------------------------------------------------------
N3MainSettingsDialog::~N3MainSettingsDialog()
{
    delete ui;
}

//-------------------------------------------------------------------------------------
bool N3MainSettingsDialog::isChanged() const
{
    return m_new_n3settings->data() != m_n3settings.data();
}

//-------------------------------------------------------------------------------------
void N3MainSettingsDialog::fillComboBoxes()
{
    ui->cbSpeed->addItems(m_n3settings.listBySettingType(device_setting_type::speed));
    ui->cbTemp->addItems(m_n3settings.listBySettingType(device_setting_type::temperature));
    ui->cbAlt->addItems(m_n3settings.listBySettingType(device_setting_type::altitude));
    ui->cbAirDisp->addItems(m_n3settings.listBySettingType(device_setting_type::aircraft_display));
    ui->cbCanopyAlarm->addItems(m_n3settings.listBySettingType(device_setting_type::canopy_alarm));
    ui->cbCanopyDisp->addItems(m_n3settings.listBySettingType(device_setting_type::canopy_display));
    ui->cbDateFormat->addItems(m_n3settings.listBySettingType(device_setting_type::date_format));
    ui->cbDisplay->addItems(m_n3settings.listBySettingType(device_setting_type::display));
    ui->cbLogBook->addItems(m_n3settings.listBySettingType(device_setting_type::logbook));
    ui->cbTimeFormat->addItems(m_n3settings.listBySettingType(device_setting_type::time_format));
}

//-------------------------------------------------------------------------------------
void N3MainSettingsDialog::initComboBoxes()
{
    ui->cbSpeed->setCurrentIndex(static_cast<int>(m_new_n3settings->speedMeasure()));
    ui->cbTemp->setCurrentIndex(static_cast<int>(m_new_n3settings->temperatureMeasure()));
    ui->cbAlt->setCurrentIndex(static_cast<int>(m_new_n3settings->altitudeMeasure()));
    ui->cbCanopyAlarm->setCurrentIndex(static_cast<int>(m_new_n3settings->canopyAlarmsMode()));
    ui->cbCanopyDisp->setCurrentIndex(static_cast<int>(m_new_n3settings->canopyDisplayEnabled()));
    ui->cbDateFormat->setCurrentIndex(static_cast<int>(m_new_n3settings->dateFormat()));
    ui->cbAirDisp->setCurrentIndex(static_cast<int>(m_new_n3settings->climbDisplayMode()));
    ui->cbDisplay->setCurrentIndex(static_cast<int>(m_new_n3settings->dislpayIsFlipped()));
    ui->cbLogBook->setCurrentIndex(static_cast<int>(m_new_n3settings->logbookEnabled()));
    ui->cbTimeFormat->setCurrentIndex(static_cast<int>(m_new_n3settings->timeFormat()));

    ui->cbSpeed->setProperty("setting_type", static_cast<int>(device_setting_type::speed));
    ui->cbTemp->setProperty("setting_type", static_cast<int>(device_setting_type::temperature));
    ui->cbAlt->setProperty("setting_type", static_cast<int>(device_setting_type::altitude));
    ui->cbCanopyAlarm->setProperty("setting_type", static_cast<int>(device_setting_type::canopy_alarm));
    ui->cbCanopyDisp->setProperty("setting_type", static_cast<int>(device_setting_type::canopy_display));
    ui->cbDateFormat->setProperty("setting_type", static_cast<int>(device_setting_type::date_format));
    ui->cbAirDisp->setProperty("setting_type", static_cast<int>(device_setting_type::aircraft_display));
    ui->cbDisplay->setProperty("setting_type", static_cast<int>(device_setting_type::display));
    ui->cbLogBook->setProperty("setting_type", static_cast<int>(device_setting_type::logbook));
    ui->cbTimeFormat->setProperty("setting_type", static_cast<int>(device_setting_type::time_format));

    connect(ui->cbSpeed, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbTemp, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbAlt, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbCanopyAlarm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbCanopyDisp, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbDateFormat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbAirDisp, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbLogBook, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbDisplay, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
    connect(ui->cbTimeFormat, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3MainSettingsDialog::combo_changeindex);
}

//-------------------------------------------------------------------------------------
void N3MainSettingsDialog::combo_changeindex(int index)
{
    device_setting_type st = static_cast<device_setting_type>((QObject::sender()->property("setting_type").toInt()));

    switch(st)
    {
    case device_setting_type::speed:
        m_new_n3settings->setSpeedMeasure(static_cast<speed_measure>(index));
        break;
    case device_setting_type::temperature:
        m_new_n3settings->setTemperatureMeasure(static_cast<temperature_measure>(index));
        break;
    case device_setting_type::altitude:
        m_new_n3settings->setAltitudeMeasure(static_cast<altitude_measure>(index));
        break;
    case device_setting_type::canopy_alarm:
        m_new_n3settings->setCanopyAlarmsMode(static_cast<canopy_alarms_mode>(index));
        break;
    case device_setting_type::canopy_display:
        m_new_n3settings->setCanopyDisplayEnabled(static_cast<canopy_display_mode>(index));
        break;
    case device_setting_type::date_format:
        m_new_n3settings->setDateFormat(static_cast<date_format>(index));
        break;
    case device_setting_type::aircraft_display:
        m_new_n3settings->setClimbDisplayMode(static_cast<climb_display_mode>(index));
        break;
    case device_setting_type::display:
        m_new_n3settings->setDislpayIsFlipped(static_cast<bool>(index));
        break;
    case device_setting_type::logbook:
        m_new_n3settings->setLogbookEnabled(static_cast<bool>(index));
        break;
    case device_setting_type::time_format:
        m_new_n3settings->setTimeFormat(static_cast<time_format>(index));
        break;
    default:
        break;
    }

    bool is_changed = isChanged();
    emit changed(is_changed);

    setWindowTitle(tr(m_title.toStdString().c_str()) + (is_changed ? "*" : ""));
}

//-------------------------------------------------------------------------------------
void N3MainSettingsDialog::on_buttonBox_accepted()
{
    m_new_n3settings->calculateCheckSum();
    accept();
}
