#ifndef N3ALARMSSETTINGS_H
#define N3ALARMSSETTINGS_H

#include <QByteArray>
//#include <QString>
//#include <vector>
//#include <memory>

class N3DeviceSettings;

class N3AlarmsSettings
{
    enum class as_offsets: int      {activeFreeFallItem = 2,
                                     activeCanopyItem = 3,
                                     beginArray = 4
                                    };

    QByteArray m_data;
    N3DeviceSettings *m_device_settings = nullptr;
    //std::unique_ptr<std::vector<std::unique_ptr<QString>>> m_names = std::make_unique<std::vector<std::unique_ptr<QString>>>();

public:
    explicit N3AlarmsSettings(N3DeviceSettings* device_settings = nullptr);
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
    N3DeviceSettings* deviceSettings() const { return m_device_settings; }

    friend bool operator==(const N3AlarmsSettings& left, const N3AlarmsSettings& right);
    //N3DeviceSettings& operator=(const N3DeviceSettings& right) noexcept;

protected:


};

#endif // N3ALARMSSETTINGS_H
