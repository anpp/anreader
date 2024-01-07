#include "n3alarms_names.h"
#include "n3alarms_settings.h"

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsNames::N3AlarmsNames(N3DeviceSettings* device_settings) : N3Names(N3NamesType::Alarms)
{
    m_settings = std::make_unique<N3AlarmsSettings>(device_settings);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsNames::calculateCheckSum()
{
    N3Names::calculateCheckSum();
    m_settings->calculateCheckSum();
}

//----------------------------------------------------------------------------------------------------------------------
bool operator==(const N3AlarmsNames& left, const N3AlarmsNames& right)
{
    return (static_cast<const N3Names&>(left) == static_cast<const N3Names&>(right) && *left.m_settings == *right.m_settings);
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsNames &N3AlarmsNames::operator=(const N3AlarmsNames &right) noexcept
{
    if (this == &right)
        return *this;

    m_data = right.m_data;
    m_map_active = right.m_map_active;
    m_type = right.m_type;
    *m_settings = *right.m_settings;
    return *this;
}
