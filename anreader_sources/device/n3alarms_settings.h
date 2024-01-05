#ifndef N3ALARMSSETTINGS_H
#define N3ALARMSSETTINGS_H

#include <QByteArray>
#include <QString>
#include <vector>
#include <memory>

class N3AlarmsSettings
{
    enum class as_offsets: int      {currentFreeFallItem = 2,
                                     currentCanopyItem = 3,
                                     beginArray = 4
                                    };

    QByteArray m_data;
    std::unique_ptr<std::vector<std::unique_ptr<QString>>> m_names = std::make_unique<std::vector<std::unique_ptr<QString>>>();

public:
    explicit N3AlarmsSettings() {};
    virtual ~N3AlarmsSettings() {}

    QByteArray& data() {return m_data; }

protected:
    virtual QString byIndex(uint index) const = 0;
    virtual void setHighBit(uint index, uint byte_number, bool value) = 0;

};

#endif // N3ALARMSSETTINGS_H
