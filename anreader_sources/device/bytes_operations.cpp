#include "bytes_operations.h"

//----------------------------------------------------------------------------------------------------------------------
BytesOperations::BytesOperations()
{

}

//----------------------------------------------------------------------------------------------------------------------
uint16_t BytesOperations::bytesToUInt16(const QByteArray &data, uint pos)
{
    uint16_t result = 0;

    for(uchar i = 0; i < 2; ++i)
        result += static_cast<uint16_t>(static_cast<uchar>(data[pos + i]) << (8 * i));

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t BytesOperations::bytesToUInt32(const QByteArray &data, uint pos)
{
    uint32_t result = 0;

    for(uchar i = 0; i < 4; ++i)
        result += static_cast<uint32_t>(static_cast<uchar>(data[pos + i]) << (8 * i));

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t BytesOperations::bytesToUInt(const char &byte0, const char &byte1, const char &byte2, const char &byte3)
{
    Bytes4 u;

    u.bytes.b0 = byte0;
    u.bytes.b1 = byte1;
    u.bytes.b2 = byte2;
    u.bytes.b3 = byte3;
    return u.value;
}

//----------------------------------------------------------------------------------------------------------------------
QByteArray BytesOperations::UIntToBytes(const quint32 &dword)
{
    QByteArray result(4, 0);
    Bytes4 u;

    u.value = dword;
    result[0] = u.bytes.b0;
    result[1] = u.bytes.b1;
    result[2] = u.bytes.b2;
    result[3] = u.bytes.b3;
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t BytesOperations::getValue16(const QByteArray &bytes, unsigned offset)
{
    if(static_cast<uint16_t>(bytes.size()) < offset + sizeof(uint16_t))
        return 0;

    return BytesOperations::bytesToUInt16(bytes, offset);
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t BytesOperations::getValue32(const QByteArray &bytes, unsigned offset)
{
    if(static_cast<uint16_t>(bytes.size()) < offset + sizeof (uint32_t))
        return 0;

    return BytesOperations::bytesToUInt32(bytes, offset);
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t BytesOperations::setHighBit(uint8_t byte, bool value)
{
    return setBit(byte, 7, value);
}

//----------------------------------------------------------------------------------------------------------------------
uint8_t BytesOperations::setBit(uint8_t byte, int index, bool value)
{
    if(value)
        return byte | (1 << index);
    else
        return byte & ~(1 << index);
}

//----------------------------------------------------------------------------------------------------------------------
bool BytesOperations::checkBit(u_int8_t byte, int index)
{
    return (byte & (1 << index)) != 0;
}

