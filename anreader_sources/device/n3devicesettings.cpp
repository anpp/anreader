#include "n3devicesettings.h"
#include "n3_constants.h"

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::calculateCheckSum()
{
    if(m_data.size() < static_cast<int>(N3Constants::DeviceSettingsSize)) return;
    m_data[0] = 1;
    for(unsigned int i = 2; i < N3Constants::DeviceSettingsSize; i++)
        m_data[0] = m_data[0] + m_data[i];
}

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
canopy_display_mode N3DeviceSettings::canopyDisplayEnabled() const
{
    return getByte(ds_offsets::canopyDisplayEnabled) == 0 ? canopy_display_mode::scaled : canopy_display_mode::not_scaled;
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
void N3DeviceSettings::setAltitudeMeasure(altitude_measure value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::altitudeMeasure);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setSpeedMeasure(speed_measure value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::speedMeasure);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setTemperatureMeasure(temperature_measure value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::temperatureMeasure);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setDislpayIsFlipped(bool value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::displayFlipped);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setLogbookEnabled(bool value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::logbookEnabled);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setTimeFormat(time_format value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::timeFormat);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setDateFormat(date_format value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::dateFormat);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setCanopyDisplayEnabled(canopy_display_mode value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::canopyDisplayEnabled);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setClimbDisplayMode(climb_display_mode value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::climbDisplayMode);
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setCanopyAlarmsMode(canopy_alarms_mode value)
{
    setByte(static_cast<uint8_t>(value), ds_offsets::canopyAlarmsMode);
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t N3DeviceSettings::getByte(int offset) const
{
    if(m_data.size() >= offset)
        return m_data[offset];
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
void N3DeviceSettings::setByte(uint8_t byte, int offset)
{
    if(m_data.size() >= offset)
        m_data[offset] = byte;
}

