#ifndef N3DROPZONES_H
#define N3DROPZONES_H

#include "adropzones.h"

enum class N3DZValues: uint {count = 32, length = 10, offset = 2};

class N3DropZones : public ADropZones
{
public:    
    explicit N3DropZones(const QByteArray& adata): ADropZones(adata) {};
    virtual ~N3DropZones() override {}

    uint count() const override;
    uint filled() const override;
    QString byIndex(const uint index) const override;
};

#endif // N3DROPZONES_H
