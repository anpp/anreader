#include "n3devicesettings.h"

//----------------------------------------------------------------------------------------------------------------------
altitude_measure N3DeviceSettings::altitudeMeasure() const
{
    return getByte(ds_offsets::altitudeMeasure) == 0 ? altitude_measure::feet : altitude_measure::meters;
}

//----------------------------------------------------------------------------------------------------------------------
speed_measure N3DeviceSettings::speedMeasure() const
{
    return getByte(ds_offsets::speedMeasure) == 0 ? speed_measure::mph : speed_measure::kmh;
}

//----------------------------------------------------------------------------------------------------------------------
temperature_measure N3DeviceSettings::temperatureMeasure() const
{
    return getByte(ds_offsets::temperatureMeasure) == 0 ? temperature_measure::fahrenheit : temperature_measure::celsius;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3DeviceSettings::dislpayIsFlipped() const
{
    return getByte(ds_offsets::displayFlipped) == 1;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3DeviceSettings::logbookEnabled() const
{
    return getByte(ds_offsets::logbookEnabled) == 1;
}

//----------------------------------------------------------------------------------------------------------------------
time_format N3DeviceSettings::timeFormat() const
{
    return getByte(ds_offsets::timeFormat) == 0 ? time_format::h12 : time_format::h24;
}

//----------------------------------------------------------------------------------------------------------------------
date_format N3DeviceSettings::dateFormat() const
{
    return getByte(ds_offsets::dateFormat) == 0 ? date_format::US : date_format::International;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3DeviceSettings::canopyDisplayEnabled() const
{
    return getByte(ds_offsets::canopyDisplayEnabled) == 1;
}

//----------------------------------------------------------------------------------------------------------------------
climb_display_mode N3DeviceSettings::climbDisplayMode() const
{
    return getByte(ds_offsets::climbDisplayMode) == 0 ? climb_display_mode::show_time : climb_display_mode::show_altitude;
}

//----------------------------------------------------------------------------------------------------------------------
canopy_alarms_mode N3DeviceSettings::canopyAlarmsMode() const
{
    return getByte(ds_offsets::canopyAlarmsMode) == 0 ? canopy_alarms_mode::normal : canopy_alarms_mode::loud;
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t N3DeviceSettings::getByte(const int offset) const
{
    if(m_data.size() >= offset)
        return m_data[offset];
    return 0;
}
