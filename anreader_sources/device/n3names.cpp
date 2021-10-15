#include "n3names.h"

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::count() const
{
    if(data.size() > 0)
        return static_cast<uint>(N3DZValues::count);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::filled() const
{
    if(data.size() > static_cast<int>(N3DZValues::offset))
        return data[1];

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
QString N3Names::byIndex(const uint index) const
{
    QString result = "";

    uint offset = index * static_cast<uint>(N3DZValues::length) + static_cast<uint>(N3DZValues::offset);

    if(index < count())
    {
        for(uint i = offset; i < offset + static_cast<uint>(N3DZValues::length); ++i)
        {
            if('\0' == data[i]) break;
            result += (data[i] & 0b1111111);
        }
    }

    return result;
}

