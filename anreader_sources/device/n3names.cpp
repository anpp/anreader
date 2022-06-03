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
#include <QDebug>
//----------------------------------------------------------------------------------------------------------------------
QString N3Names::byIndex(const uint index) const
{    
    QString result = "";

    uint offset = (index * static_cast<uint>(N3NamesValues::length)) + static_cast<uint>(N3NamesValues::offset);

    if(index < count())
    {
        QByteArray bytes_name(static_cast<uint>(N3NamesValues::length), 0);
        for(uint i = offset; i < offset + static_cast<uint>(N3NamesValues::length); ++i)
            bytes_name[i - offset] = m_data[i];

        bool hidden = (bytes_name[0] & 0b10000000) >> 7;
        bool used = (bytes_name[1] & 0b10000000) >> 7;

        bytes_name[0] = bytes_name[0] & 0b01111111;
        bytes_name[1] = bytes_name[1] & 0b01111111;
        result = QString::fromLatin1(bytes_name);

        qDebug() << result << " " << used << " " << hidden;
    }

    return result;
}

