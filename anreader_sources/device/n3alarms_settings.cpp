#include "n3alarms_settings.h"
#include "n3_constants.h"
#include "n3devicesettings.h"
#include "bytes_operations.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::calculateCheckSum()
{
    unsigned checksum = BytesOperations::calculateCheckSum(m_data, N3Constants::AlarmsSettingsSize, 2);
    m_data[0] = checksum & 0xFF;
    m_data[1] = (checksum >> 8) & 0xFF; //второй байт контрольной суммы
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeFreeFallIndex() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        return BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], false);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::activeCanopyIndex() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        return BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], false);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::enabledFreeFallAlarms() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        return !BytesOperations::checkBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], 7);
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::enabledCanopyAlarms() const
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        return !BytesOperations::checkBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], 7);
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
int8_t N3AlarmsSettings::nameIndex(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
        return (m_data[static_cast<int>(as_offsets::beginArray) + (index * 10)] >> 2);
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsSettings::alarm_type N3AlarmsSettings::type(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        uint8_t temp = m_data[static_cast<int>(as_offsets::beginArray) + (index * 10)];
        temp &= 0b00000011;
        return static_cast<alarm_type>(temp);
    }
    return alarm_type::unk;
}

//----------------------------------------------------------------------------------------------------------------------
int8_t N3AlarmsSettings::tone(int index, int altindex) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + 1 + altindex)
        return m_data[static_cast<int>(as_offsets::beginArray) + (index * 10) + 1 + altindex];
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3AlarmsSettings::altitude(int index, int altindex) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        int result =  BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2));
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        double stp = step(index);        
        int factor = type(index) == alarm_type::FreeFall ? 100 : 10;

        if(altitude_measure::meters == am)
        {            
            result = round(result / 2.0 / stp) * stp;
            return result;
        }
        result = round(metersIncs2feet(result, factor));
        return result;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3AlarmsSettings::min_altitude(int index, int altindex) const
{
    uint16_t result = 0;
    switch(altindex)
    {
    case 0:
        result = altitude(index, 1) + interval(index);
        break;
    case 1:
        result = altitude(index, 2) + interval(index);
        break;
    case 2:
        result = min(index);
    }

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3AlarmsSettings::max_altitude(int index, int altindex) const
{
    uint16_t result =  0;
    switch(altindex)
    {
    case 0:
        result = max(index);
        break;
    case 1:
        result = altitude(index, 0) - interval(index);
        break;
    case 2:
        result = altitude(index, 1) - interval(index);
    }

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3AlarmsSettings::active(int index) const
{
    return (activeFreeFallIndex() == index || activeCanopyIndex() == index);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setActiveFreeFallIndex(int index)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem) && index >= 0 && index < 8)
    {
        bool enabled_alarms = enabledFreeFallAlarms();
        m_data[static_cast<int>(as_offsets::activeFreeFallItem)] = index;
        enableFreeFallAlarms(enabled_alarms);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setActiveCanopyIndex(int index)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem) && index >= 0 && index < 8)
    {
        bool enabled_alarms = enabledCanopyAlarms();
        m_data[static_cast<int>(as_offsets::activeCanopyItem)] = index;
        enableFreeFallAlarms(enabled_alarms);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::enableFreeFallAlarms(bool enable)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeFreeFallItem))
        m_data[static_cast<int>(as_offsets::activeFreeFallItem)] = BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeFreeFallItem)], !enable);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::enableCanopyAlarms(bool enable)
{
    if(m_data.size() > static_cast<int>(as_offsets::activeCanopyItem))
        m_data[static_cast<int>(as_offsets::activeCanopyItem)] = BytesOperations::setHighBit(m_data[static_cast<int>(as_offsets::activeCanopyItem)], !enable);
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setAltitude(int index, int altindex, uint16_t value)
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        double device_altitude = value;
        uint16_t device_save_altitude = BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2));

        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        int factor = (type(index) == alarm_type::FreeFall ? 50 : 10);

        if(altitude_measure::meters == am)
            device_altitude = metersIncs2feet(device_altitude * 2, factor);

        device_altitude = feet2metersIncs(device_altitude, type(index));
        device_altitude = correct_altitude(index, altindex, device_altitude, device_save_altitude);

        setRawAlt(index, altindex, round(device_altitude));
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool operator==(const N3AlarmsSettings& left, const N3AlarmsSettings& right)
{
    return (left.m_data == right.m_data);
}

//----------------------------------------------------------------------------------------------------------------------
N3AlarmsSettings& N3AlarmsSettings::operator=(const N3AlarmsSettings &right) noexcept
{
    if (this == &right)
        return *this;

    m_data = right.m_data;
    m_device_settings = right.m_device_settings;    
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &N3AlarmsSettings::alitudePostfix() const
{
    if(nullptr == m_device_settings)
        return m_feet;
    return m_device_settings->altitudeMeasure() == altitude_measure::feet? m_feet: m_meters;
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::clear()
{
    m_data.clear();
}

//----------------------------------------------------------------------------------------------------------------------
unsigned int N3AlarmsSettings::step(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        if(altitude_measure::meters == am)
            return (type(index) == alarm_type::FreeFall ? 25 : 5);
        else
            return (type(index) == alarm_type::FreeFall ? 100 : 10);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
unsigned int N3AlarmsSettings::interval(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        if(altitude_measure::meters == am)
            return (type(index) == alarm_type::FreeFall ? 150 : 30);
        else
            return (type(index) == alarm_type::FreeFall ? 500 : 100);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
unsigned int N3AlarmsSettings::max(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        if(altitude_measure::meters == am)
            return (type(index) == alarm_type::FreeFall ? 6100 : 1220);
        else
            return (type(index) == alarm_type::FreeFall ? 20000 : 4000);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
unsigned int N3AlarmsSettings::min(int index) const
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10))
    {
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        if(altitude_measure::meters == am)
            return (type(index) == alarm_type::FreeFall ? 300 :30);
        else
            return (type(index) == alarm_type::FreeFall ? 1000 : 100);
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
double N3AlarmsSettings::metersIncs2feet(double value, int step) const
{
    // -0.05 чуть корректирует отображение футов (1200 - 3900, а не 4000) после сохранения метров
    double result = round((value / 2.0 * 1000.0 / 25.4 / 12.0 / step) - 0.05);
    return result * step;
}


//----------------------------------------------------------------------------------------------------------------------
int N3AlarmsSettings::feet2metersIncs(double value, alarm_type atype) const
{
    int factor = (atype == alarm_type::FreeFall ? 25 : 5);
    double result = value * 2.0 * 30.48 * factor;
    return round(result / (double)(100 * factor));
}

//----------------------------------------------------------------------------------------------------------------------
uint16_t N3AlarmsSettings::correct_altitude(int index, int altindex, uint16_t altitude, uint16_t save_altitude)
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        uint16_t alts[3];
        int minimum_interval = (type(index) == alarm_type::FreeFall ? 500 : 100);
        int max_value = (type(index) == alarm_type::FreeFall ? 12192 : 2438);

        alts[0] = BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (0 * 2));
        alts[1] = BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (1 * 2));
        alts[2] = BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (2 * 2));

        alts[altindex] = altitude;

        double koeff = 1000.0 / 25.4 / 12.0;

        int mt = round((((alts[1] - alts[2]) / 2.0) * koeff));
        if(mt < minimum_interval)
            alts[1] = feet2metersIncs((alts[2] / 2.0) * koeff + minimum_interval, type(index));

        mt = round((((alts[0] - alts[1]) / 2.0) * koeff));
        if(mt < minimum_interval)
            alts[0] = feet2metersIncs((alts[1] / 2.0) * koeff + minimum_interval, type(index));

        if(alts[0] > max_value)
            return save_altitude;

        setRawAlt(index, 0, alts[0]);
        setRawAlt(index, 1, alts[1]);
        setRawAlt(index, 2, alts[2]);

        return alts[altindex];
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setRawAlt(int index, int altindex, uint16_t altitude)
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        int i = static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2);
        QByteArray bytes = BytesOperations::UInt16ToBytes(altitude);
        m_data[i] = bytes[0];
        m_data[i + 1] = bytes[1];
    }
}



