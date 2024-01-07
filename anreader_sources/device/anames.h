#ifndef ANAMES_H
#define ANAMES_H

#include <QByteArray>
#include <QString>
#include <vector>
#include <memory>

class ANames
{    
public:
    explicit ANames() {};
    virtual ~ANames() {};

    virtual uint count() const = 0;
    virtual uint filled() const = 0;        
    const std::vector<std::unique_ptr<QString>>& Names() const;
    virtual void clear() =0;
    virtual bool used(uint index) const = 0;
    virtual bool hidden(uint index) const = 0;
    virtual bool active(uint index) const = 0;
    virtual uint active_index() const = 0;

    virtual void setActive(uint index, bool value = true) = 0;
    virtual void setHidden(uint index, bool value = true) = 0;
    virtual void setUsed(uint index, bool value) = 0;
    virtual void setName(uint index, const QString& value) = 0;
    virtual void setFilled(const uint value) = 0;

    QByteArray& data() {return m_data; };
    const QByteArray& data_const() const {return m_data; };

protected:
    virtual QString byIndex(uint index) const = 0;
    virtual void setHighBit(uint index, uint byte_number, bool value) = 0;

    QByteArray m_data;
    std::unique_ptr<std::vector<std::unique_ptr<QString>>> m_names = std::make_unique<std::vector<std::unique_ptr<QString>>>();
};

#endif // ANAMES_H
