#include "n3alarms_settings.h"
#include "n3_constants.h"
#include "n3devicesettings.h"
#include "bytes_operations.h"
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::calculateCheckSum()
{
    BytesOperations::calculateCheckSum(m_data, N3Constants::AlarmsSettingsSize, 2);

    //второй байт контрольной суммы (здесь почему-то так)
    unsigned chksum2 = 1;
    for(unsigned i = 2; i < N3Constants::AlarmsSettingsSize; i++)
        chksum2 = chksum2 + (unsigned char)m_data[i];

    m_data[1] = ((chksum2 >> 8) & 0xFF);
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
        double result =  BytesOperations::getValue16(m_data, static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2));
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;
        double stp = step(index);
        //double factor = (altitude_measure::meters == am ? 10.0 : 3.0);
        unsigned factor = (type(index) == alarm_type::FreeFall ? 100 : 10);
        //return result;


        if(altitude_measure::meters == am)
        {
            unsigned t = (unsigned)result % (unsigned)stp;
            //result = floor(result / factor) * factor / 2.0;
            result = result - t / 2;
            result = round(result / 2 / stp) * stp;
            return result;
        }
        result = meters2feet(round(result / 2));
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

#include <QDebug>
//----------------------------------------------------------------------------------------------------------------------
void N3AlarmsSettings::setAltitude(int index, int altindex, uint16_t value)
{
    if(index >= 0 && index < 8 && m_data.size() > static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2))
    {
        double device_altitude = value;
        altitude_measure am = (nullptr != m_device_settings) ? m_device_settings->altitudeMeasure() : altitude_measure::feet;

        if(altitude_measure::meters == am)
            device_altitude = meters2feet(device_altitude);

        device_altitude = feet2meters(device_altitude, type(index));
        qDebug() << device_altitude;

        int i = static_cast<int>(as_offsets::beginArray) + (index * 10) + static_cast<int>(as_offsets::altitudeOffset) + (altindex * 2);
        QByteArray bytes = BytesOperations::UInt16ToBytes(round(device_altitude));
        m_data[i] = bytes[0];
        m_data[i + 1] = bytes[1];

        //if((int)device_altitude % 127 == 0)
        //    m_data[1] = m_data[1] | 1;
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
void N3AlarmsSettings::init()
{
    for(int i = 0; i < 8; ++ i)
    {
        setAltitude(i, 0, altitude(i, 0));
        setAltitude(i, 1, altitude(i, 1));
        setAltitude(i, 2, altitude(i, 2));
    }
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
double N3AlarmsSettings::meters2feet(double value) const
{
    return value * (1000.0 / 25.4 / 12.0);
}

//----------------------------------------------------------------------------------------------------------------------
double N3AlarmsSettings::feet2meters(double value, alarm_type atype) const
{
    int factor = (atype == alarm_type::FreeFall ? 50 : 10);

    double result = round(value / factor) * factor;

    qDebug() << value << " " << result << " ";

    return round(result / (1000.0 / 25.4 / 12.0) * 2);
}



