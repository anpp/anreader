#include "n3names.h"
#include "n3_constants.h"
#include "bytes_operations.h"

//----------------------------------------------------------------------------------------------------------------------
N3Names::N3Names(N3NamesType atype) : m_type(atype)
{
}

void N3Names::calculateCheckSum()
{
    BytesOperations::calculateCheckSum(m_data, N3Constants::N3NamesSize);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::init()
{
    //заполнение массива чистыми данными, разделенными нулями (в приборе встречается мусор между данными)
    for(uint i = 0; i < N3NamesValues::size; ++i)
        setName(i, byIndex(i));
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::isEqual(const ANames &right) const
{
    return (m_data == static_cast<const N3Names&>(right).m_data
            && m_map_active == static_cast<const N3Names&>(right).m_map_active
            && m_type == static_cast<const N3Names&>(right).m_type);
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::count() const
{
    if(m_data.size() > 0)
        return N3NamesValues::size;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::filled() const
{
    if(m_data.size() > N3NamesValues::offset)
        return m_data[1] > N3NamesValues::size ? N3NamesValues::size :m_data[1];

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::clear()
{
    m_names->clear();
    m_data.clear();
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::used(uint index) const
{
    int offset = (index * N3NamesValues::length) + N3NamesValues::offset;

    if(index < filled())
        return BytesOperations::checkBit(m_data[offset + 1], 7);
    return false;

}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::hidden(uint index) const
{
    int offset = (index * N3NamesValues::length) + N3NamesValues::offset;

    if(index < filled())
        return BytesOperations::checkBit(m_data[offset], 7);
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::active(uint index) const
{
    if(m_map_active.contains(index))
        return m_map_active[index];
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
uint N3Names::active_index() const
{
    return m_map_active.key(true);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setActive(uint index, bool value)
{
    m_map_active.clear();
    m_map_active[index] = value;
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setHidden(uint index, bool value)
{
    setHighBit(index, 0, value);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setUsed(uint index, bool value)
{
    setHighBit(index, 1, value);
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setName(uint index, const QString& value)
{
    bool saved_used = used(index);
    bool saved_hidden = hidden(index);

    if(index <= count())
    {
        int offset = (index * N3NamesValues::length) + N3NamesValues::offset;
        for(int i = 0; i < static_cast<int>(N3NamesValues::length); ++i)
        {
            m_data[i + offset] = 0;
            if(i < value.size())
                m_data[i + offset] = value.at(i).toLatin1();
        }

        setUsed(index, saved_used);
        setHidden(index, saved_hidden);

        m_names->clear(); //очистка списка имен, при первом обращении заполнится вновь из сырых данных
    }
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setFilled(const uint value)
{
    if(m_data.size() > N3NamesValues::offset)
        m_data[1] = value;
}

//----------------------------------------------------------------------------------------------------------------------
N3Names &N3Names::operator=(const N3Names &right) noexcept
{
    if (this == &right)
        return *this;

    m_data = right.m_data;
    m_map_active = right.m_map_active;
    m_type = right.m_type;
    m_map_selected_in_alarms = right.m_map_selected_in_alarms;
    return *this;
}

//----------------------------------------------------------------------------------------------------------------------
bool N3Names::selected(uint index) const
{
    if(m_map_selected_in_alarms.contains(index))
        return m_map_selected_in_alarms[index];
    return false;
}

//----------------------------------------------------------------------------------------------------------------------
bool operator==(const N3Names& left, const N3Names& right)
{
    return left.isEqual(right);
}

//----------------------------------------------------------------------------------------------------------------------
QString N3Names::byIndex(uint index) const
{    
    QString result = "";

    int offset = (index * N3NamesValues::length) + N3NamesValues::offset;

    if(index < filled())
    {
        QByteArray bytes_name(N3NamesValues::length, 0);
        for(int i = offset; i < offset + N3NamesValues::length; ++i)
            bytes_name[i - offset] = m_data[i];

        bytes_name[0] = BytesOperations::setHighBit(bytes_name[0], false);
        bytes_name[1] = BytesOperations::setHighBit(bytes_name[1], false);
        result = QString::fromLatin1(bytes_name);
    }
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
void N3Names::setHighBit(uint index, uint byte_number, bool value)
{
    int offset = (index * N3NamesValues::length) + N3NamesValues::offset + byte_number;

    if(index < count())
        m_data[offset] = BytesOperations::setHighBit(m_data[offset], value);
}

