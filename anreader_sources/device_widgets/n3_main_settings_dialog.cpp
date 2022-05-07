#include "n3_main_settings_dialog.h"
#include "ui_n3_main_settings_dialog.h"

#include "device/n3devicesettings.h"

//-------------------------------------------------------------------------------------
N3MainSettingsDialog::N3MainSettingsDialog(const N3DeviceSettings& n3settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::N3MainSettingsDialog),
    m_n3settings(n3settings)
{
    ui->setupUi(this);

    fillComboBoxes();
    initComboBoxes();
}

//-------------------------------------------------------------------------------------
N3MainSettingsDialog::~N3MainSettingsDialog()
{
    delete ui;
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
    ui->cbSpeed->setCurrentIndex(static_cast<int>(m_n3settings.speedMeasure()));
    ui->cbTemp->setCurrentIndex(static_cast<int>(m_n3settings.temperatureMeasure()));
    ui->cbAlt->setCurrentIndex(static_cast<int>(m_n3settings.altitudeMeasure()));
    ui->cbCanopyAlarm->setCurrentIndex(static_cast<int>(m_n3settings.canopyAlarmsMode()));
    ui->cbCanopyDisp->setCurrentIndex(static_cast<int>(m_n3settings.canopyDisplayEnabled()));
    ui->cbDateFormat->setCurrentIndex(static_cast<int>(m_n3settings.dateFormat()));
    ui->cbAirDisp->setCurrentIndex(static_cast<int>(m_n3settings.climbDisplayMode()));
    ui->cbDisplay->setCurrentIndex(static_cast<int>(m_n3settings.dislpayIsFlipped()));
    ui->cbLogBook->setCurrentIndex(static_cast<int>(m_n3settings.logbookEnabled()));
    ui->cbTimeFormat->setCurrentIndex(static_cast<int>(m_n3settings.timeFormat()));
}
