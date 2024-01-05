#include "adevicesettings.h"
#include <QObject>
#include <QString>

const static QString altitude_measure_string[] = {QObject::tr("Feet"), QObject::tr("Meters")};
const static QString speed_measure_string[] = {QObject::tr("m/h"), QObject::tr("km/h")};
const static QString temperature_measure_string[] = {QObject::tr("Fahrenheit"), QObject::tr("Celsius")};
const static QString date_format_string[] = {QObject::tr("US"), QObject::tr("International")};
const static QString time_format_string[] = {QObject::tr("12"), QObject::tr("24")};
const static QString climb_display_mode_string[] = {QObject::tr("Climb Screen"), QObject::tr("Altimeter")};
const static QString canopy_alarm_mode_string[] = {QObject::tr("Normal"), QObject::tr("Loud")};
const static QString canopy_display_mode_string[] = {QObject::tr("Scaled"), QObject::tr("Not scaled")};


//----------------------------------------------------------------------------------------------------------------------
ADeviceSettings::ADeviceSettings()
{

}

//----------------------------------------------------------------------------------------------------------------------
const QStringList &ADeviceSettings::listBySettingType(device_setting_type dst) const
{
    m_dstlist.clear();

    switch(dst){
    case device_setting_type::speed:
        for(const QString& s: speed_measure_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::altitude:
        for(const QString& s: altitude_measure_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::temperature:
        for(const QString& s: temperature_measure_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::date_format:
        for(const QString& s: date_format_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::time_format:
        for(const QString& s: time_format_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::canopy_display:
        for(const QString& s: canopy_display_mode_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::aircraft_display:
        for(const QString& s: climb_display_mode_string)
            m_dstlist.append(s);

        break;
    case device_setting_type::display:
        m_dstlist.append("Normal");
        m_dstlist.append("Flipped");

        break;
    case device_setting_type::logbook:
        m_dstlist.append("Disabled");
        m_dstlist.append("Enabled");

        break;
    case device_setting_type::canopy_alarm:
        for(const QString& s: canopy_alarm_mode_string)
            m_dstlist.append(s);

        break;
    default:
        break;
    }

    return m_dstlist;
}
