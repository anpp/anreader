#ifndef N3SUMMARYINFO_H
#define N3SUMMARYINFO_H

#include "asummaryinfo.h"


enum si_offsets: unsigned {odometerJumps = 2,
                           totalJumps = 4,
                           totalJumps_not_del = 6,
                           totalFreeFallTime = 8,
                           totalCanopyTime = 12,
                           nextJumpNumber = 16,
                           topJumpNumber = 18,
                           lastJumpOffset = 20,
                           firstJumpNumber = 22,
                           currentDZ = 24,
                           currentAP = 26};


class N3SummaryInfo : public ASummaryInfo
{
public:
    explicit N3SummaryInfo(QByteArray& adata): ASummaryInfo(adata) {};
    virtual ~N3SummaryInfo() override {}
    void calculateCheckSum();

    virtual uint16_t odometerJumps() const override;
    virtual uint16_t totalJumps() const override;
    virtual uint16_t totalJumps_not_del() const override;
    virtual uint32_t totalFreeFallTime() const override;
    virtual uint32_t totalCanopyTime() const override;
    virtual uint16_t nextJumpNumber() const override;
    virtual uint16_t topJumpNumber() const override;
    virtual uint16_t lastJumpOffset() const override;
    virtual uint16_t firstJumpNumber() const override;
    virtual uint16_t currentDZIndex() const override;
    virtual uint16_t currentAPIndex() const override;
    void setDZIndex(uint16_t value) override;
    void setAPIndex(uint16_t value) override;

private:
    void setWord(uint16_t value, int offset);

};

#endif // N3SUMMARYINFO_H
