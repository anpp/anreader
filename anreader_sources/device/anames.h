#ifndef ANAMES_H
#define ANAMES_H

#include <QByteArray>
#include <QString>

class ANames
{    
public:
    explicit ANames(const QByteArray& adata): data(adata) {};
    virtual ~ANames() {}

    virtual uint count() const = 0;
    virtual uint filled() const = 0;
    virtual QString byIndex(const uint index) const = 0;
protected:
    const QByteArray& data;

};

#endif // ANAMES_H
