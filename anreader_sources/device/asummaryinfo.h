#ifndef ASUMMARYINFO_H
#define ASUMMARYINFO_H

#include <QByteArray>


class ASummaryInfo
{
public:
    explicit ASummaryInfo(const QByteArray& adata): data(adata) {};
    virtual ~ASummaryInfo() {}

    virtual uint16_t odometerJumps() const = 0;
    virtual uint16_t totalJumps() const = 0;
    virtual uint16_t totalJumps_not_del() const = 0;
    virtual uint32_t totalFreeFallTime() const = 0;
    virtual uint32_t totalCanopyTime() const = 0;
    virtual uint16_t nextJumpNumber() const = 0;
    virtual uint16_t topJumpNumber() const = 0;
    virtual uint16_t lastJumpOffset() const = 0;
    virtual uint16_t firstJumpNumber() const = 0;
    virtual uint16_t currentDZIndex() const = 0;
    virtual uint16_t currentAPIndex() const = 0;

protected:
    const QByteArray& data;
};

#endif // ASUMMARYINFO_H
