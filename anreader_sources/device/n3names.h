#ifndef N3NAMES_H
#define N3NAMES_H

#include "anames.h"

#include <QMap>

enum class N3NamesValues: uint {count = 32, length = 10, offset = 2};

class N3Names : public ANames
{
    mutable QMap<uint, bool> m_map_used;
    mutable QMap<uint, bool> m_map_hidden;
public:    
    explicit N3Names(QByteArray& adata): ANames(adata) {};
    virtual ~N3Names() override {}

    uint count() const override;
    uint filled() const override;
    virtual void clear() override { m_names->clear(); }    
    bool used(uint index) const override;
    bool hidden(uint index) const override;

private:
    QString byIndex(uint index) const override;

};

#endif // N3NAMES_H
