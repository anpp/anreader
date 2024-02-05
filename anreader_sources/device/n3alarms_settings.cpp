#include "n3alarms_settings.h"
#include "n3_constants.h"
#include "n3devicesettings.h"
#include "bytes_operations.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::calculateCheckSum()
{
    BytesOperations::calculateCheckSum(m_data, N3Constants::AlarmsSettingsSize, 2);
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeFreeFallIndex() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        return BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], false);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeCanopyIndex() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        return BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], false);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::enabledFreeFallAlarms() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        return !BytesOperations::checkBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], 7);
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::enabledCanopyAlarms() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        return !BytesOperations::checkBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], 7);
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
int8_t N3AlarmsSettings::nameIndex(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
        return (m_data[static_cast<int>(as_offsets::beginArray) + (index * 10)] >> 2);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsSettings::alarm_type N3AlarmsSettings::type(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        uint8_t temp = m_data[static_cast<int>(as_offsets::beginArray) + (index * 10)];
        temp &= 0b00000011;
        return static_cast<alarm_type>(temp);
    }
    return alarm_type::unk;
}

//----------------------------------------------------------------------------------------------------------------------
int8_t N3AlarmsSettings::tone(int index, int altindex) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + 1 + altindex)
        return m_data[static_cast<int>(as_offsets::beginArray) + (index * 10) + 1 + altindex];
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3AlarmsSettings::altitude(int index, int altindex) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        double result =  BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2));
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        double meters_step = (type(index) == alarm_type::FreeFall ? 25.0 : 5.0);
        double feet_step = (type(index) == alarm_type::FreeFall ? 100.0 : 10.0);
        double factor = (altitude_measure::meters == am ? 10.0 : 1.0);

        result = floor(result / factor) * factor / 2.0;

        if(altitude_measure::meters == am)
            return round(result / meters_step) * meters_step;
        else
            return round(round(result * (1000 / 25.4 / 12) / feet_step - 0.1) * feet_step);

    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::active(int index) const
{
    return (activeFreeFallIndex() == index || activeCanopyIndex() == index);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setActiveFreeFallIndex(int index)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem) && index >= 0 && index < 8)
    {
        bool enabled_alarms = enabledFreeFallAlarms();
        m_data[static_cast<int>(as_offsets::activeFreeFallItem)] = index;
        enableFreeFallAlarms(enabled_alarms);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setActiveCanopyIndex(int index)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem) && index >= 0 && index < 8)
    {
        bool enabled_alarms = enabledCanopyAlarms();
        m_data[static_cast<int>(as_offsets::activeCanopyItem)] = index;
        enableFreeFallAlarms(enabled_alarms);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::enableFreeFallAlarms(bool enable)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        m_data[static_cast<int>(as_offsets::activeFreeFallItem)] = BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], !enable);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::enableCanopyAlarms(bool enable)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        m_data[static_cast<int>(as_offsets::activeCanopyItem)] = BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], !enable);
}

//----------------------------------------------------------------------------------------------------------------------
bool operator==(const N3AlarmsSettings& left, const N3AlarmsSettings& right)
{
    return (left.m_data == right.m_data);
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsSettings& N3AlarmsSettings::operator=(const N3AlarmsSettings &right) noexcept
{
    if (this == &right)
        return *this;

    m_data = right.m_data;
    m_device_settings = right.m_device_settings;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &N3AlarmsSettings::alitudePostfix() const
{
    if(nullptr == m_device_settings)
        return m_feet;
    return m_device_settings->altitudeMeasure() == altitude_measure::feet? m_feet: m_meters;
}



