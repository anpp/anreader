#include "n3dropzones.h"


uint N3DropZones::count() const
{
   return static_cast<uint>(N3DZValues::count);
}

uint N3DropZones::filled() const
{
    if(data.size() > static_cast<int>(N3DZValues::offset))
        return data[1];

    return 0;
}

QString N3DropZones::byIndex(const uint index) const
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
