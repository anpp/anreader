#ifndef N3DEVICESETTINGS_H
#define N3DEVICESETTINGS_H

#include "adevicesettings.h"

enum ds_offsets: int      {altitudeMeasure = 0,
                           speedMeasure = 1,
                           temperatureMeasure = 2,
                           displayFlipped = 3,
                           logbookEnabled = 4,
                           timeFormat = 5,
                           dateFormat = 6,
                           canopyDisplayEnabled = 7,
                           climbDisplayMode = 8};


class N3DeviceSettings : public ADeviceSettings
{
public:
    N3DeviceSettings(const QByteArray& adata) : ADeviceSettings(adata) {};
    virtual ~N3DeviceSettings() override {}

    altitude_measure    altitudeMeasure() const override;
    speed_measure       speedMeasure() const override;
    temperature_measure temperatureMeasure() const override;
    bool                dislpayIsFlipped() const override;
    bool                logbookEnabled() const override;
    int                 timeFormat() const override;
    date_format         dateFormat() const override;
    bool                canopyDisplayEnabled() const override;
    climb_display_mode  climbDisplayMode() const override;

    //void setInfo(const QByteArray &info) override {bytes = info; }

private:
    //QByteArray& bytes;

    uint8_t getByte(const int offset) const;

};

#endif // N3DEVICESETTINGS_H
