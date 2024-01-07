#ifndef N3ALARMSNAMES_H
#define N3ALARMSNAMES_H

#include "n3names.h"

class N3AlarmsSettings;
class N3DeviceSettings;

class N3AlarmsNames : public N3Names
{
    std::unique_ptr<N3AlarmsSettings> m_settings;
public:
    explicit N3AlarmsNames(N3DeviceSettings* device_settings = nullptr);

    N3AlarmsSettings& settings() const { return *m_settings; };
    void calculateCheckSum() override;

    friend bool operator==(const N3AlarmsNames& left, const N3AlarmsNames& right);
    N3AlarmsNames& operator=(const N3AlarmsNames& right) noexcept;

};

#endif // N3ALARMSNAMES_H
