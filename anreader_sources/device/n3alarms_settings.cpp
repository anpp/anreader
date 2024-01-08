#include "n3alarms_settings.h"
#include "n3_constants.h"
#include "n3devicesettings.h"
#include "bytes_operations.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::calculateCheckSum()
{
    if(m_data.size() < static_cast<int>(N3Constants::DeviceSettingsSize)) return;
    m_data[0] = 1;
    for(unsigned int i = 2; i < N3Constants::AlarmsSettingsSize; i++)
        m_data[0] = m_data[0] + m_data[i];
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
        uint16_t result =  BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2));

        altitude_measure am = altitude_measure::feet;
        float koeff = (type(index) == alarm_type::FreeFall ? 100.0 : 10.0);
        if(nullptr != m_device_settings)
            am = m_device_settings->altitudeMeasure();

        if(altitude_measure::meters == am)
            result = round((koeff * (result / 2.0)) / koeff);
        else
            result = round((((((result / 2.0) * 1000) / 25.4) / 12) / koeff) * koeff);

        return result;
    }
    return 0;
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



