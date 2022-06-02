#include "n3summaryinfo.h"
#include "bytes_operations.h"

#include <QDebug>


//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::odometerJumps() const
{
    return BytesOperations::getValue16(data, si_offsets::odometerJumps);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::totalJumps() const
{
    return BytesOperations::getValue16(data, si_offsets::totalJumps);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::totalJumps_not_del() const
{
    return BytesOperations::getValue16(data, si_offsets::totalJumps_not_del);
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t N3SummaryInfo::totalFreeFallTime() const
{
    return BytesOperations::getValue32(data, si_offsets::totalFreeFallTime);
}

//----------------------------------------------------------------------------------------------------------------------
uint32_t N3SummaryInfo::totalCanopyTime() const
{
    return BytesOperations::getValue32(data, si_offsets::totalCanopyTime);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::nextJumpNumber() const
{
    return BytesOperations::getValue16(data, si_offsets::nextJumpNumber);
}


//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::topJumpNumber() const
{
    return BytesOperations::getValue16(data, si_offsets::topJumpNumber);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::lastJumpOffset() const
{
    return BytesOperations::getValue16(data, si_offsets::lastJumpOffset);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::firstJumpNumber() const
{
    return BytesOperations::getValue16(data, si_offsets::firstJumpNumber);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::currentDZIndex() const
{
    return BytesOperations::getValue16(data, si_offsets::currentDZ);
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3SummaryInfo::currentAPIndex() const
{
    return BytesOperations::getValue16(data, si_offsets::currentAP);
}





