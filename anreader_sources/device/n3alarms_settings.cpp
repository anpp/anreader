#include "n3alarms_settings.h"
#include "n3_constants.h"
#include "n3devicesettings.h"
#include "bytes_operations.h"

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::calculateCheckSum()
{
    if(m_data.size() < static_cast<int>(N3Constants::DeviceSettingsSize)) return;
    m_data[0] = 1;
    for(unsigned int i = 2; i < N3Constants::AlarmsSettingsSize; i++)
        m_data[0] = m_data[0] + m_data[i];
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeFreeFallIndex(N3AlarmsSettings) const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        return BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], false);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeCanopyIndex(N3AlarmsSettings) const
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
    return *this;
}



