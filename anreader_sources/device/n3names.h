#ifndef N3NAMES_H
#define N3NAMES_H

#include "anames.h"

enum class N3DZValues: uint {count = 32, length = 10, offset = 2};

class N3Names : public ANames
{
public:    
    explicit N3Names(const QByteArray& adata): ANames(adata) {};
    virtual ~N3Names() override {}

    uint count() const override;
    uint filled() const override;

private:
    QString byIndex(const uint index) const override;

};

#endif // N3NAMES_H
