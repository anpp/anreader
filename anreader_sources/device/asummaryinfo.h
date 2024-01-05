#ifndef ASUMMARYINFO_H
#define ASUMMARYINFO_H

#include <QByteArray>


class ASummaryInfo
{
public:
    explicit ASummaryInfo() {};
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
    virtual void setDZIndex(uint16_t value) = 0;
    virtual void setAPIndex(uint16_t value) = 0;

    QByteArray& data() {return m_data; }

protected:
    QByteArray m_data;
};

#endif // ASUMMARYINFO_H
