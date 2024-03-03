#ifndef N3ALARMSSETTINGS_H
#define N3ALARMSSETTINGS_H

#include <QObject>
#include <QByteArray>
#include <QString>

class N3DeviceSettings;

class N3AlarmsSettings
{
    enum class as_offsets: int {activeFreeFallItem = 2,
                                activeCanopyItem = 3,
                                beginArray = 4,
                                altitudeOffset = 4
                               };    

    QByteArray m_data;
    N3DeviceSettings *m_device_settings = nullptr;
    const QString m_feet = QObject::tr("Feet");
    const QString m_meters = QObject::tr("Meters");
public:
    enum class alarm_type: uint8_t {FreeFall = 0, Canopy = 1, unk = 2};

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
    uint16_t min_altitude(int index, int altindex) const;
    uint16_t max_altitude(int index, int altindex) const;
    bool active(int index) const;

    void setActiveFreeFallIndex(int index);
    void setActiveCanopyIndex(int index);
    void enableFreeFallAlarms(bool enable);
    void enableCanopyAlarms(bool enable);
    void setNameIndex(int index, int8_t name_index);
    void setAltitude(int index, int altindex, uint16_t value);

    QByteArray& data() {return m_data; };

    friend bool operator==(const N3AlarmsSettings& left, const N3AlarmsSettings& right);
    N3AlarmsSettings& operator=(const N3AlarmsSettings& right) noexcept;

    void clear();
    unsigned step(int index) const;
    const QString& alitudePostfix() const;

private:
    unsigned interval(int index) const;
    unsigned max(int index) const;
    unsigned min(int index) const;
    double metersIncs2feet(double value, int factor) const;
    int feet2metersIncs(double value) const;
    uint16_t correct_altitude(int index, int altindex, uint16_t altitude, uint16_t save_altitude);
    void setRawAlt(int index, int altindex, uint16_t altitude);
};

#endif // N3ALARMSSETTINGS_H
