#ifndef N3ALARMSSETTINGS_H
#define N3ALARMSSETTINGS_H

#include <QByteArray>
//#include <QString>
//#include <vector>
//#include <memory>

class N3AlarmsSettings
{
    enum class as_offsets: int      {activeFreeFallItem = 2,
                                     activeCanopyItem = 3,
                                     beginArray = 4
                                    };

    QByteArray m_data;
    //std::unique_ptr<std::vector<std::unique_ptr<QString>>> m_names = std::make_unique<std::vector<std::unique_ptr<QString>>>();

public:
    explicit N3AlarmsSettings() {};
    virtual ~N3AlarmsSettings() {}

    void calculateCheckSum();
    int activeFreeFallIndex(N3AlarmsSettings) const;
    int activeCanopyIndex(N3AlarmsSettings) const;
    bool enabledFreeFallAlarms() const;
    bool enabledCanopyAlarms() const;

    void setActiveFreeFallIndex(int index);
    void setActiveCanopyIndex(int index);
    void enableFreeFallAlarms(bool enable);
    void enableCanopyAlarms(bool enable);

    QByteArray& data() {return m_data; }

protected:


};

#endif // N3ALARMSSETTINGS_H
