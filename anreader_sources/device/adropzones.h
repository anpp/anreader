#ifndef ADROPZONES_H
#define ADROPZONES_H

#include <QByteArray>
#include <QString>

class ADropZones
{    
public:
    explicit ADropZones(const QByteArray& adata): data(adata) {};
    virtual ~ADropZones() {}

    virtual uint count() const = 0;
    virtual uint filled() const = 0;
    virtual QString byIndex(const uint index) const = 0;
protected:
    const QByteArray& data;

};

#endif // ADROPZONES_H
