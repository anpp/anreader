#include "n3summaryinfo.h"
#include "bytes_operations.h"
#include "n3_constants.h"

#include <QDebug>


//----------------------------------------------------------------------------------------------------------------------
void N3SummaryInfo::calculateCheckSum()
{
    if(m_data.size() < static_cast<int>(N3Constants::SummarySize)) return;
    m_data[0] = 1;
    for(unsigned int i = 2; i < N3Constants::SummarySize; i++)
        m_data[0] = m_data[0] + m_data[i];
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::odometerJumps() const
{
    return BytesOperations::getValue16(m_data, si_offsets::odometerJumps);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::totalJumps() const
{
    return BytesOperations::getValue16(m_data, si_offsets::totalJumps);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::totalJumps_not_del() const
{
    return BytesOperations::getValue16(m_data, si_offsets::totalJumps_not_del);
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t N3SummaryInfo::totalFreeFallTime() const
{
    return BytesOperations::getValue32(m_data, si_offsets::totalFreeFallTime);
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t N3SummaryInfo::totalCanopyTime() const
{
    return BytesOperations::getValue32(m_data, si_offsets::totalCanopyTime);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::nextJumpNumber() const
{
    return BytesOperations::getValue16(m_data, si_offsets::nextJumpNumber);
}


//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::topJumpNumber() const
{
    return BytesOperations::getValue16(m_data, si_offsets::topJumpNumber);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::lastJumpOffset() const
{
    return BytesOperations::getValue16(m_data, si_offsets::lastJumpOffset);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::firstJumpNumber() const
{
    return BytesOperations::getValue16(m_data, si_offsets::firstJumpNumber);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::currentDZIndex() const
{    
    uint16_t index = BytesOperations::getValue16(m_data, si_offsets::currentDZ);
    return index >= 32 ? 0 : index;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::currentAPIndex() const
{
    uint16_t index = BytesOperations::getValue16(m_data, si_offsets::currentAP);
    return index >= 32 ? 0 : index;
}

//----------------------------------------------------------------------------------------------------------------------
void N3SummaryInfo::setDZIndex(uint16_t value)
{
    setWord(value, static_cast<int>(si_offsets::currentDZ));
    calculateCheckSum();
}

//----------------------------------------------------------------------------------------------------------------------
void N3SummaryInfo::setAPIndex(uint16_t value)
{
    setWord(value, static_cast<int>(si_offsets::currentAP));
    calculateCheckSum();
}

//----------------------------------------------------------------------------------------------------------------------
void N3SummaryInfo::setWord(uint16_t value, int offset)
{
    if(m_data.size() >= offset)
    {
        QByteArray bytes(BytesOperations::UIntToBytes(value));

        m_data[offset + 0] = bytes[0];
        m_data[offset + 1] = bytes[1];
    }
}





