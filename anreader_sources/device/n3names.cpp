#include "n3names.h"

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::count() const
{
    if(data.size() > 0)
        return static_cast<uint>(N3NamesValues::count);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::filled() const
{
    if(data.size() > static_cast<int>(N3NamesValues::offset))
        return data[1];

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
        QByteArray arr(static_cast<uint>(N3NamesValues::length), 0);
        for(uint i = offset; i < offset + static_cast<uint>(N3NamesValues::length); ++i)
        {
            arr[i - offset] = data[i];

            //if('\0' == data[i]) continue;
            //result += (data[i] & 0b01111111);
            //if(i - offset == 1)
              //  qDebug() << ((data[i] & 0b10000000) >> 8);
        }
        qDebug() << arr.toHex();
        //qDebug() << QString::fromStdString( arr.toStdString());
    }

    return result;
}

