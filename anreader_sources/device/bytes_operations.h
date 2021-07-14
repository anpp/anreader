#ifndef BYTES_OPERATIONS_H
#define BYTES_OPERATIONS_H

#include <QByteArray>

class BytesOperations
{
    union Bytes4
    {
        int32_t value;
        struct
        {
            unsigned char b0, b1, b2, b3;
        } bytes;
    };

public:
    BytesOperations();

    static uint16_t bytesToUInt16(const QByteArray &data, uint pos);
    static uint32_t bytesToUInt32(const QByteArray &data, uint pos);
    static uint32_t bytesToUInt(const char &byte0, const char &byte1, const char &byte2, const char &byte3);
    static QByteArray UIntToBytes(const quint32 &dword);
    static uint16_t getValue16(const QByteArray &bytes, const unsigned offset);
    static uint32_t getValue32(const QByteArray &bytes, const unsigned offset);


};

#endif // BYTES_OPERATIONS_H
