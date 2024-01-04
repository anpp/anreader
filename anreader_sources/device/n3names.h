#ifndef N3NAMES_H
#define N3NAMES_H

#include "anames.h"

#include <QMap>

enum class N3NamesValues: uint {count = 32, length = 10, offset = 2};

class N3Names : public ANames
{
    mutable QMap<uint, bool> m_map_active;
public:    
    explicit N3Names(QByteArray& adata): ANames(adata){ };
    virtual ~N3Names() override {}
    void calculateCheckSum();

    uint count() const override;
    uint filled() const override;
    virtual void clear() override;
    bool used(uint index) const override;
    bool hidden(uint index) const override;    
    bool active(uint index) const override;
    uint active_index() const override;

    void setActive(uint index, bool value = true) override;
    void setHidden(uint index, bool value = true) override;
    void setUsed(uint index, bool value) override;
    void setName(uint index, const QString value) override;
    void setFilled(const uint value) override;

    friend bool operator==(const N3Names& left, const N3Names& right);
    N3Names& operator=(const N3Names& right);
    const QMap<uint, bool>& map_active() const { return m_map_active; };

private:
    QString byIndex(uint index) const override;
    void setHighBit(uint index, uint byte_number, bool value) override;

};

#endif // N3NAMES_H
