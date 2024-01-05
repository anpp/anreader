#ifndef N3DEVICESETTINGS_H
#define N3DEVICESETTINGS_H

#include "adevicesettings.h"


class N3DeviceSettings : public ADeviceSettings
{
    enum class ds_offsets: int      {altitudeMeasure = 2,
                                     speedMeasure = 3,
                                     temperatureMeasure = 4,
                                     displayFlipped = 5,
                                     logbookEnabled = 6,
                                     timeFormat = 7,
                                     dateFormat = 8,
                                     canopyDisplayEnabled = 9,
                                     climbDisplayMode = 10,
                                     canopyAlarmsMode = 14};

public:
    N3DeviceSettings() {};
    virtual ~N3DeviceSettings() override {}
    void calculateCheckSum();

    altitude_measure    altitudeMeasure() const override;
    speed_measure       speedMeasure() const override;
    temperature_measure temperatureMeasure() const override;
    bool                dislpayIsFlipped() const override;
    bool                logbookEnabled() const override;
    time_format         timeFormat() const override;
    date_format         dateFormat() const override;
    canopy_display_mode canopyDisplayEnabled() const override;
    climb_display_mode  climbDisplayMode() const override;
    canopy_alarms_mode  canopyAlarmsMode() const override;

    void                setAltitudeMeasure(altitude_measure value) override;
    void                setSpeedMeasure(speed_measure value) override;
    void                setTemperatureMeasure(temperature_measure value) override;
    void                setDislpayIsFlipped(bool value) override;
    void                setLogbookEnabled(bool value) override;
    void                setTimeFormat(time_format value) override;
    void                setDateFormat(date_format value) override;
    void                setCanopyDisplayEnabled(canopy_display_mode value) override;
    void                setClimbDisplayMode(climb_display_mode value) override;
    void                setCanopyAlarmsMode(canopy_alarms_mode value) override;


private:    

    uint8_t getByte(ds_offsets offset) const;
    void setByte(uint8_t byte, ds_offsets offset);

};

#endif // N3DEVICESETTINGS_H
