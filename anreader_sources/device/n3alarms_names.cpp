#include "n3alarms_names.h"
#include "n3alarms_settings.h"

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsNames::setActives()
{
    m_map_active.clear();
    setActive(m_settings->nameIndex(m_settings->activeFreeFallIndex()), true);
    setActive(m_settings->nameIndex(m_settings->activeCanopyIndex()), true);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsNames::setSelected()
{
    m_map_selected_in_alarms.clear();
    for(int i = 0; i < 8; ++i)
        m_map_selected_in_alarms[m_settings->nameIndex(i)] = true;
}

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
void N3AlarmsNames::setActive(uint index, bool value)
{
    m_map_active[index] = value;
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsNames::init()
{
    N3Names::init();
    m_settings->init();
    setActives();
    setSelected();
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsNames::isEqual(const ANames &right) const
{
    return (N3Names::isEqual(right)) && (*m_settings == *static_cast<const N3AlarmsNames&>(right).m_settings);
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsNames& N3AlarmsNames::operator=(const N3Names &right) noexcept
{
    N3Names::operator =(right);
    *m_settings = *static_cast<const N3AlarmsNames&>(right).m_settings;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsNames& N3AlarmsNames::operator=(const N3AlarmsNames &right) noexcept
{
    N3AlarmsNames::operator = (static_cast<const N3Names&>(right));
    return *this;
}
