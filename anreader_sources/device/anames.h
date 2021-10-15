#ifndef ANAMES_H
#define ANAMES_H

#include <QByteArray>
#include <QString>
#include <vector>
#include <memory>

class ANames
{    
public:
    explicit ANames(const QByteArray& adata): data(adata) {};
    virtual ~ANames() {}

    virtual uint count() const = 0;
    virtual uint filled() const = 0;        
    const std::vector<QString>& Names() const;

protected:
    virtual QString byIndex(const uint index) const = 0;

    const QByteArray& data;
    std::unique_ptr<std::vector<QString>> m_names = std::make_unique<std::vector<QString>>();
};

#endif // ANAMES_H