#ifndef ADEVICESETTINGS_H
#define ADEVICESETTINGS_H

#include <QByteArray>
#include <QStringList>

enum class device_setting_type: unsigned {speed, altitude, temperature, date_format, time_format, canopy_display,
                                         aircraft_display, display, logbook, canopy_alarm};

enum class altitude_measure: unsigned {feet = 0, meters};
enum class speed_measure: unsigned {mph = 0, kmh};
enum class temperature_measure: unsigned {fahrenheit = 0, celsius};
enum class date_format : unsigned {US = 0, International};
enum class time_format : unsigned {h12 = 0, h24};
enum class climb_display_mode : unsigned {show_time = 0, show_altitude};
enum class canopy_alarms_mode : unsigned {normal = 0, loud};


class ADeviceSettings
{
    mutable QStringList m_dstlist;
public:
    explicit ADeviceSettings(const QByteArray& adata);
    virtual ~ADeviceSettings() {}

    virtual altitude_measure    altitudeMeasure() const = 0;
    virtual speed_measure       speedMeasure() const = 0;
    virtual temperature_measure temperatureMeasure() const = 0;
    virtual bool                dislpayIsFlipped() const = 0;
    virtual bool                logbookEnabled() const = 0;
    virtual time_format         timeFormat() const = 0;
    virtual date_format         dateFormat() const = 0;
    virtual bool                canopyDisplayEnabled() const = 0;
    virtual climb_display_mode  climbDisplayMode() const = 0;
    virtual canopy_alarms_mode  canopyAlarmsMode() const = 0;

    const QByteArray& data() const {return m_data; }
    const QStringList& listBySettingType(device_setting_type dst) const;

protected:
    const QByteArray& m_data;
};

#endif // ADEVICESETTINGS_H
