#include "n3names.h"

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::count() const
{
    if(m_data.size() > 0)
        return static_cast<uint>(N3NamesValues::count);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::filled() const
{
    if(m_data.size() > static_cast<int>(N3NamesValues::offset))
        return m_data[1];

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::used(uint index) const
{
    if(m_map_used.contains(index))
        return m_map_used[index];
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::hidden(uint index) const
{
    if(m_map_hidden.contains(index))
        return m_map_hidden[index];
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::active(uint index) const
{
    if(m_map_active.contains(index))
        return m_map_active[index];
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setActive(uint index, bool value) const
{
    m_map_active[index] = value;
}

//----------------------------------------------------------------------------------------------------------------------
N3Names &N3Names::operator=(const N3Names &right)
{
    if (this == &right)
        return *this;

    m_map_active = right.m_map_active;
    m_map_used = right.m_map_used;
    m_map_hidden = right.m_map_hidden;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
bool operator==(const N3Names& left, const N3Names& right)
{
    return (left.m_data == right.m_data
            && left.m_map_active == right.m_map_active
            && left.m_map_used == right.m_map_used
            && left.m_map_hidden == right.m_map_hidden);
}

//----------------------------------------------------------------------------------------------------------------------
QString N3Names::byIndex(uint index) const
{    
    QString result = "";

    uint offset = (index * static_cast<uint>(N3NamesValues::length)) + static_cast<uint>(N3NamesValues::offset);

    if(index < count())
    {
        QByteArray bytes_name(static_cast<uint>(N3NamesValues::length), 0);
        for(uint i = offset; i < offset + static_cast<uint>(N3NamesValues::length); ++i)
            bytes_name[i - offset] = m_data[i];

        m_map_hidden[index] = (bytes_name[0] & 0b10000000) >> 7;
        m_map_used[index] = (bytes_name[1] & 0b10000000) >> 7;        

        bytes_name[0] = bytes_name[0] & 0b01111111;
        bytes_name[1] = bytes_name[1] & 0b01111111;
        result = QString::fromLatin1(bytes_name);
    }
    return result;
}

