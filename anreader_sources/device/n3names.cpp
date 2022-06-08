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

