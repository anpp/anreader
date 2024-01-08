#ifndef N3ALARMSSETTINGS_H
#define N3ALARMSSETTINGS_H

#include <QByteArray>
//#include <QString>
//#include <vector>
//#include <memory>

class N3DeviceSettings;

class N3AlarmsSettings
{
    enum class as_offsets: int {activeFreeFallItem = 2,
                                activeCanopyItem = 3,
                                beginArray = 4,
                                altitudeOffset = 4
                               };
    enum class alarm_type: uint8_t {FreeFall = 0, Camopy = 1, unk = 2};

    QByteArray m_data;
    N3DeviceSettings *m_device_settings = nullptr;
    //std::unique_ptr<std::vector<std::unique_ptr<QString>>> m_names = std::make_unique<std::vector<std::unique_ptr<QString>>>();
public:
    explicit N3AlarmsSettings(N3DeviceSettings* device_settings = nullptr) : m_device_settings(device_settings) {};
    virtual ~N3AlarmsSettings() {};

    void calculateCheckSum();
    int activeFreeFallIndex() const;
    int activeCanopyIndex() const;
    bool enabledFreeFallAlarms() const;
    bool enabledCanopyAlarms() const;
    int8_t nameIndex(int index) const;
    alarm_type type(int index) const;
    int8_t tone(int index, int altindex) const;
    uint16_t altitude(int index, int altindex) const;

    void setActiveFreeFallIndex(int index);
    void setActiveCanopyIndex(int index);
    void enableFreeFallAlarms(bool enable);
    void enableCanopyAlarms(bool enable);

    QByteArray& data() {return m_data; };

    friend bool operator==(const N3AlarmsSettings& left, const N3AlarmsSettings& right);
    N3AlarmsSettings& operator=(const N3AlarmsSettings& right) noexcept;

};

#endif // N3ALARMSSETTINGS_H
