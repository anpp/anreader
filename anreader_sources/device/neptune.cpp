#include "neptune.h"
#include "bytes_operations.h"

#include <QThread>
#include <math.h>

#include "jumps/n3jump.h"
#include "n3summaryinfo.h"
#include "n3devicesettings.h"
#include "n3names.h"


const static QString N3TypeNames[] =
{QObject::tr("Unknown"),
 QObject::tr("Neptune"),
 QObject::tr("Wave"),
 QObject::tr("Tracker"),
 QObject::tr("Data Logger"),
 QObject::tr("N3"),
 QObject::tr("N3A"),
 QObject::tr("Atlas"),
 QObject::tr("?"),
 QObject::tr("?"),
 QObject::tr("?"),
 QObject::tr("?"),
 QObject::tr("Atlas 2")
};

//----------------------------------------------------------------------------------------------------------------------
void WorkerKeepAlive::keepAlive()
{
    if(working)
    {
        emit sendPacket(keep_alive_command);
        n_keeps++;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void WorkerKeepAlive::process()
{
    working = true;

    while(working)
    {
        keepAlive();
        QThread::msleep(N3Constants::KeepAliveDelay);
    }
    emit finished();
}

//----------------------------------------------------------------------------------------------------------------------
void WorkerKeepAlive::stop()
{
    working = false;
    if(thread != nullptr)
        thread->wait();
}

//----------------------------------------------------------------------------------------------------------------------
void WorkerKeepAlive::start()
{
    clear();
    if(thread != nullptr)
    {
        if(!thread->isRunning())
            thread->start();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void WorkerKeepAlive::clear()
{
    n_keeps = 0;
}




//===================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
Neptune::Neptune(QString portName, QObject *parent) : AbstractDevice(portName, parent)
{
    keep_alive_worker.setThread(&keep_alive_thread);
    keep_alive_worker.moveToThread(&keep_alive_thread);

    connect(&keep_alive_thread, &QThread::started, &keep_alive_worker, &WorkerKeepAlive::process);
    connect(&keep_alive_worker, &WorkerKeepAlive::finished, &keep_alive_thread, &QThread::quit, Qt::DirectConnection);

    m_summary = std::make_unique<N3SummaryInfo>(rawDataSummary);
    m_settings = std::make_unique<N3DeviceSettings>(rawDataSettings);
    m_dropzones = std::make_unique<N3Names>(rawDZNames);
    m_airplanes = std::make_unique<N3Names>(rawAPNames);
    m_alarms_names = std::make_unique<N3Names>(rawAlarmsNames);
}

//----------------------------------------------------------------------------------------------------------------------
Neptune::~Neptune()
{
    keep_alive_worker.stop();
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Neptune::deviceName() const
{
    return N3TypeNames[static_cast<int>(m_product_type)];
}

//----------------------------------------------------------------------------------------------------------------------
unsigned int Neptune::n_jumps_readed() const
{
    unsigned int n_jumps = (static_cast<uint>(rawDataDetails.size()) / N3Constants::JumpRecordSize);
    bool last_jump_is_valid = false;

    if(n_jumps > 0)
    {
        last_jump_is_valid = (BytesOperations::bytesToUInt16(*jump_at(n_jumps - 1), 0) != 65535);
        n_jumps =  n_jumps - (last_jump_is_valid ? 0 : 1);
    }
    return n_jumps;
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::open()
{
    AbstractDevice::open();

    m_dropzones->clear();
    m_airplanes->clear();
    m_alarms_names->clear();
}


//----------------------------------------------------------------------------------------------------------------------
const QString Neptune::getSerialNumber() const
{
    QString result = "";
    if(Type0Record.size() > 16){
        for(auto i = 5; i <= 13; ++i)
            result += Type0Record[i];
    }
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::set_date_time(const QDateTime &a_datetime)
{
    AbstractDevice::set_date_time(a_datetime);

    executeCommand(N3Commands::SetDateTime);
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::setupComPort()
{
    AbstractDevice::setupComPort();

    sp->setBytesToPort(N3Constants::BytesToPort);
    sp->setDelay(N3Constants::msByteSendingDelay);

    connect(sp.get(), &SerialPortThread::finished, this, &Neptune::finishedWriteData);
    connect(sp.get(), &SerialPortThread::readyData, this, &Neptune::processData);

    connect(this, &Neptune::sendPacket, sp.get(), &SerialPortThread::sendPacket);
    connect(&keep_alive_worker, &WorkerKeepAlive::sendPacket, sp.get(), &SerialPortThread::sendPacket);
}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<QByteArray> Neptune::jump_at(uint index) const
{
    uint raw_index = index * N3Constants::JumpRecordSize;
    std::unique_ptr<QByteArray> raw_jump = std::make_unique<QByteArray>(N3Constants::JumpRecordSize, 0);

    if ((raw_index + N3Constants::JumpRecordSize) > static_cast<uint>(rawDataDetails.size()))
        raw_jump->clear();
    else {
        *raw_jump = rawDataDetails.mid(static_cast<int>(raw_index), N3Constants::JumpRecordSize);
    }
    return raw_jump;
}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<CustomJump> Neptune::jump_from_raw(uint index) const
{
    std::unique_ptr<QByteArray> raw_jump = jump_at(index);
    if(static_cast<uint>(raw_jump->size()) < N3Constants::JumpRecordSize)
        return nullptr;

    uint jump_number = BytesOperations::bytesToUInt16(*raw_jump, 0);

    int month = ((raw_jump->at(2) & 0x7F));
    month = (m_product_type == N3Types::Atlas && m_software_revision == 1)? month + 128 : month; //для Atlas ревизии 1 + 128 месяцев

    month = month + m_correct_date_koeff * 128; //коэффициент приходит извне, рассчитывается от текущей даты по желанию пользователя
    month = month == 0 ? 1 : month;

    //для старых прыжков прошитого n3 надо отнимать 96 месяцев (это надо сделать в интерфейсе)
    //Непрошитые N3 и Atlas отсчитывают месяцы с 2007 года, только Atlas еще + 128 месяцев (то есть с 2017.08)
    //Atlas 2 отсчитывает дату с 2015 года как и прошитый N3 и Atlas 1 ревизии 3
    //прошитый N3 отсчитывает с 2015
    QDate date(((m_software_revision >= 4 && m_product_type == N3Types::N3) || (m_product_type == N3Types::Atlas2) ||
                              (m_software_revision == 3 && m_product_type == N3Types::Atlas)? 2015 : 2007) + (month - 1) / 12,
               month % 12 == 0 ? 12 : month % 12,
               (raw_jump->at(13) >> 2) & 0b11111);
    QTime time((BytesOperations::bytesToUInt16(*raw_jump, 6) >> 6) & 0b11111,
               BytesOperations::bytesToUInt16(*raw_jump, 6) & 0x3F);


    QDateTime jump_date =  QDateTime(date, time);

    uint freefall_time = BytesOperations::bytesToUInt16(*raw_jump, 4) & 0x03FF;
    uint canopy_time = BytesOperations::bytesToUInt16(*raw_jump, 16) & 0x0FFF;

    float altitude_koeff = (m_settings->altitudeMeasure() == altitude_measure::feet ? 52.4934 : 16);
    uint exit_alt = (BytesOperations::bytesToUInt16(*raw_jump, 12) & 0x03ff) * altitude_koeff;
    uint delp_alt = (BytesOperations::bytesToUInt16(*raw_jump, 14) & 0x03ff) * altitude_koeff;

    float speed_koeff = (m_settings->speedMeasure() == speed_measure::mph ? 2.236936 : 3.6);
    uint speed3K = static_cast<uint>(floor((raw_jump->at(8) & 0x7F) * speed_koeff + 0.5));
    uint speed6K = static_cast<uint>(floor(((BytesOperations::bytesToUInt16(*raw_jump, 8) >> 7) & 0x7F) * speed_koeff + 0.5));
    uint speed9K = static_cast<uint>(floor(((BytesOperations::bytesToUInt16(*raw_jump, 9) >> 6) & 0x7F) * speed_koeff + 0.5));
    uint speed12K = static_cast<uint>(floor(((BytesOperations::bytesToUInt16(*raw_jump, 10) >> 5) & 0x7F) * speed_koeff + 0.5));

    bool is_deleted = (raw_jump->at(2) >> 7) & 1;

    QString dz = *dropzones().Names().at(((BytesOperations::bytesToUInt16(*raw_jump, 15) >> 2) & 0b11111));
    QString ap = *airplanes().Names().at(((BytesOperations::bytesToUInt16(*raw_jump, 11) >> 4) & 0b1111));

    return std::make_unique<N3Jump>(jump_number, jump_date, dz, ap, exit_alt, delp_alt, freefall_time, canopy_time, speed12K, speed9K, speed6K, speed3K, is_deleted);
}



//----------------------------------------------------------------------------------------------------------------------
void Neptune::executeCommand(N3Commands command, unsigned int address, unsigned int length, char *wbytes, quint16 delay_ms)
{
    startTimeoutTimer(N3Constants::TimeoutInMs);    

    // Пока оставил инициализацию тут, но надо бы перенести в переход в состояние Initializing... Работает и так, но может выстрелить.        
    if((command == N3Commands::InitCommand) && (state() == DeviceStates::Connected))
    {
        last_command = {command, address, length, delay_ms, wbytes};
        emit initializeStateSignal();
        emit log("Command: " + QString::number(command, 16).toUpper() + "h");

        Type0Record.clear();
        emit sendPacket("018080", N3Constants::DelayInMs);
    }
    else        
        if(state() == DeviceStates::Ready)
        {
            last_command = {command, address, length, delay_ms, wbytes};
            emit processingStateSignal(); //при переходе в состояние Processing вызовется метод sendLastCommand()
        }
        else
        {
            if(state() == DeviceStates::Sending && m_NumBlocks > 0 && N3Commands::WriteMemory == command)
            {
                last_command = {command, address, length, delay_ms, wbytes};
                sendLastCommand();
            }
            else
                if(command != N3Commands::KeepAlive)
                {
                    queue_command c = {command, address, length, delay_ms, wbytes};
                    m_commands.enqueue(c);
                }
        }
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::sendLastCommand()
{
    QByteArray command_bytes(32, 0);
    QByteArray address_bytes;
    QByteArray length_bytes;
    quint32 data_length = 0;

    emit log("Command: " + QString::number(last_command.m_command, 16).toUpper() + "h Address: " + QString::number(last_command.m_address, 16).toUpper() + "h" + " Length: " + QString::number(last_command.m_length, 16).toUpper() + "h");

    inBuffer.clear();

    switch(last_command.m_command){
    case N3Commands::EndComm:
        outBuffer = makeSingleByteCommand(last_command.m_command);
        emit sendPacket(*outBuffer);
        break;

    case N3Commands::KeepAlive:
        // не используется, есть поток WorkerKeepAlive
        outBuffer = makeSingleByteCommand(last_command.m_command);
        emit sendPacket(*outBuffer, last_command.m_delay_ms);
        break;

    case N3Commands::ReadDateTime:
        m_NumBlocks = 1;
        rawDateTime.clear();
        outBuffer = makeSingleByteCommand(last_command.m_command);
        emit sendPacket(*outBuffer, last_command.m_delay_ms);
        break;

    case N3Commands::SetDateTime:
        command_bytes[0] = 9;
        command_bytes[1] = static_cast<char>(last_command.m_command);
        command_bytes[2] = (dateTime().date().year() & 0xFF);
        command_bytes[3] = ((dateTime().date().year() >> 8) & 0xFF);
        command_bytes[4] = dateTime().date().month();
        command_bytes[5] = dateTime().date().day();
        command_bytes[6] = 0;
        command_bytes[7] = dateTime().time().hour();
        command_bytes[8] = dateTime().time().minute();
        command_bytes[9] = dateTime().time().second();
        command_bytes[10] = calculateChecksum(command_bytes);

        outBuffer = cryptPacket(command_bytes, true);
        emit sendPacket(*outBuffer, last_command.m_delay_ms);

/*
  emit readyStateSignal();

        command_bytes = command_bytes.fromHex("98c8e423f5dd4b4228f04b4e49ab75f603b6631e0c0a52a6afd24590be916076");
        outBuffer = cryptPacket(command_bytes, false);
        qDebug() << outBuffer->toHex();
*/
        break;

    case N3Commands::ReadMemory:
        memory_block_length = static_cast<quint16>(last_command.m_length + sizeof(uint32_t));
        rawData = getRawData(last_command.m_address);

        if(nullptr == rawData)
        {
            emit errorSignal("Unknown address");
            break;
        }

        rawData->reserve(memory_block_length);
        rawData->clear();

        address_bytes = BytesOperations::UIntToBytes(last_command.m_address);
        length_bytes = BytesOperations::UIntToBytes(memory_block_length);

        //для расчета количества ожидаемых блоков надо прибавить еще 4 байта к размеру данных
        m_NumBlocks = ((memory_block_length + sizeof(uint32_t)) / N3Constants::BlockSize) + (((memory_block_length + sizeof(uint32_t)) % N3Constants::BlockSize) == 0 ? 0 : 1);

        command_bytes[0] = 7;
        command_bytes[1] = static_cast<char>(last_command.m_command);
        command_bytes[2] = address_bytes[0];
        command_bytes[3] = address_bytes[1];
        command_bytes[4] = address_bytes[2];
        command_bytes[5] = address_bytes[3];
        command_bytes[6] = length_bytes[0];
        command_bytes[7] = length_bytes[1];
        command_bytes[8] = calculateChecksum(command_bytes);

        outBuffer = cryptPacket(command_bytes, true);
        emit sendPacket(*outBuffer, last_command.m_delay_ms);
        break;

    case N3Commands::WriteMemory:

        if(nullptr == last_command.m_bytes_to_write)
        {
            emit log("Error: nothing to write!");
            emit readyStateSignal();
            break;
        }

        if(last_command.m_length > N3Constants::WriteRateDataSize)
        {
            emit log(tr("Error: too huge data size!"));
            emit readyStateSignal();
            break;
        }                

        address_bytes = BytesOperations::UIntToBytes(last_command.m_address);

        data_length = last_command.m_length + 4 + 1; //4 - размер адреса, 1 - размер кода команды
        command_bytes[0] = data_length;
        command_bytes[1] = static_cast<char>(last_command.m_command);
        command_bytes[2] = address_bytes[0];
        command_bytes[3] = address_bytes[1];
        command_bytes[4] = address_bytes[2];
        command_bytes[5] = address_bytes[3];
        for(unsigned int i = 0; i < last_command.m_length; ++i)
            command_bytes[6 + i] = last_command.m_bytes_to_write[i];
        command_bytes[data_length + 1] = calculateChecksum(command_bytes);

        outBuffer = cryptPacket(command_bytes, true);

        if(state() != DeviceStates::Sending)
            emit sendingStateSignal();
        emit sendPacket(*outBuffer, last_command.m_delay_ms);

        break;

    default:
        emit errorSignal("Unknown command");
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool Neptune::checkInBuffer()
{
    if(state() == DeviceStates::Processing)
    {
        if(checkAcknowledgment(inBuffer))
        {
            inBuffer = inBuffer.mid(2, inBuffer.size() - 2);
            emit receiveingStateSignal(); //надо перенести получение данных туда
        }
        else
        {
            if(inBuffer.size() == 1) //костыль!
            {
                inBuffer.clear();
                return false;
            }
            emit errorSignal("Acknowledgement error");
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::processData(QByteArray data)
{
    //qDebug() << data.toHex();

    stopTimeoutTimer();

    if(data.size() == 1 && (data.at(0) == 0 || data.at(0) == 0x30))
        destroy_port();

    setAckBuffer(data);

    if(!(state() == DeviceStates::Processing || state() == DeviceStates::Receiving
         || state() == DeviceStates::Initializing || state() == DeviceStates::Sending))
    {
        if(ackBuffer.size() < 2)
            return;

        if(!checkAcknowledgment(data, true))
        {
            keep_alive_worker.stop();
            //qDebug() << "Unknown: " << data.toHex();
        }
        else //если пришло подтверждение в состоянии Ready - это от keep_alive_worker
        {
            keep_alive_worker.receiveAck();
            ackBuffer.clear();
        }
        return;
    }

    //если не на все сигналы keep alive пришли подтверждения, лишнее подтверждение снимается здесь
    //Это только для состояния Processing
    if(keep_alive_worker.get_n_keeps() > 0)
    {
        if(checkAcknowledgment(ackBuffer))
        {
            keep_alive_worker.receiveAck();
            ackBuffer.clear();
        }
        return;
    }

    //здесь нормальная обработка данных
    switch (last_command.m_command) {
    case N3Commands::InitCommand:
        processType0Record(data);
        break;
    case N3Commands::ReadMemory:
        processReadMemory(data);
        break;
    case N3Commands::ReadDateTime:
        processReadDateTime(data);
        break;
    case N3Commands::KeepAlive:
        processDefault(data);
        break;
    case N3Commands::SetDateTime:
        processDefault(data);
        break;
    case N3Commands::WriteMemory:
        processWriteMemory(data);
        break;
    case N3Commands::EndComm:
        destroy_port();
        break;

    default:
        processDefault(data);
    }

}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::processType0Record(const QByteArray &data)
{
    Type0Record.append(data);

    if (static_cast<unsigned int>(Type0Record.size()) >= N3Constants::Type0RecordSizeInRaw)
    {
        emit log("Type0 record: " + Type0Record);
        if (verifyType0Record()) {
            QVector<s_key_item> &vec_key = (m_commutication_type == N3CommTypes::CT3 ? ct3_key :
                                                (m_commutication_type == N3CommTypes::CT4 ? ct4_key : ct5_key));

            setEncryptionKey(vec_key);

            emit log("s/n: " + getSerialNumber());

            keep_alive_worker.keep_alive_command = *makeSingleByteCommand(N3Commands::KeepAlive); //один раз за коннект пусть копируется
            emit readyStateSignal();
        }
        else
            emit errorSignal("Type0 record error or unknown device");
    }

}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::processReadMemory(const QByteArray &data)
{
    inBuffer.append(data);

    //qDebug() << data.toHex();
    if(!checkInBuffer())
        return;

    if(static_cast<unsigned int>(inBuffer.size()) >= N3Constants::BlockSize)
    {
        emit sendPacket(QByteArray::fromHex("31"));

        rawData->append(inBuffer);
        inBuffer.clear();
        emit stepProgress();

        if(static_cast<unsigned int>(rawData->size()) >= m_NumBlocks * N3Constants::BlockSize)
        {
            *rawData = *cryptPacket(*rawData, false); //тоже копирование. можно убрать (нужно).
            *rawData = rawData->mid(sizeof(uint32_t), memory_block_length - sizeof(uint32_t));

            if(rawData == &rawDataDetails)
                addJumpsToVector();

            if(rawData != &rawDataDetails)
            {
                emit log("Readed data: <FONT color=#006b00>" + rawData->toHex() + "</FONT>");
                emit log("Number blocks: " + QString::number(m_NumBlocks));
            }

            emit readyStateSignal();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::processWriteMemory(const QByteArray &data)
{
    inBuffer.append(data);

    if(state() == DeviceStates::Sending)
    {
        if(checkAcknowledgment(inBuffer))
        {
            inBuffer.clear();
            emit stepProgress();
            emit log(tr("Sended data") + ": <FONT color=#006b00>" + QByteArray::fromRawData(last_command.m_bytes_to_write, last_command.m_length).toHex() + "</FONT>");

            if(--m_NumBlocks <= 0)
                emit readyStateSignal();
            else
            {
                if(m_commands.count() == 0)
                {
                    emit errorSignal("Iteration count mismatch");
                    return;
                }
                dequeueCommand();
            }
        }
        else
        {
            if(inBuffer.size() == 1)
                return;

            emit errorSignal("Acknowledgement error");
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::processReadDateTime(const QByteArray &data)
{
    inBuffer.append(data);

    if(!checkInBuffer())
        return;

    if(static_cast<unsigned int>(inBuffer.size()) >= N3Constants::BlockSize)
    {
        emit sendPacket(QByteArray::fromHex("31"));

        rawDateTime.append(inBuffer);
        inBuffer.clear();
        emit stepProgress();

        rawDateTime = *cryptPacket(rawDateTime, false);

        QDateTime dt;
        dt.setDate(QDate((uchar)rawDateTime[0] + (uchar)rawDateTime[1] * 256, (uchar)rawDateTime[2], (uchar)rawDateTime[3]));
        dt.setTime(QTime((uchar)rawDateTime[5], (uchar)rawDateTime[6], (uchar)rawDateTime[7]));
        AbstractDevice::set_date_time(dt);

        emit readyStateSignal();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::processDefault(const QByteArray &data)
{
    inBuffer.append(data);

    if(state() == DeviceStates::Processing)
    {
        if(checkAcknowledgment(inBuffer))
        {
            inBuffer.clear();
            emit readyStateSignal();
        }
        else
        {
            if(inBuffer.size() == 1)
                return;

            emit errorSignal("Acknowledgement error");
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool Neptune::checkAcknowledgment(const QByteArray &buffer, const bool single) const
{
    if(buffer.size() == 1 && single)
        return ((buffer.at(0) == N3Commands::AckSuccess) || (buffer.at(0) == N3Commands::AckReady));
    else
        return ((2 <= buffer.size()) && (buffer.at(0) == N3Commands::AckSuccess) && (buffer.at(1) == N3Commands::AckReady));
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::setAckBuffer(const QByteArray &data)
{
    if(data.size() == 1 && ackBuffer.size() == 0 && data.at(0) == N3Commands::AckSuccess)
        ackBuffer.append(data);

    if(data.size() == 1 && ackBuffer.size() == 1 && data.at(0) == N3Commands::AckReady && ackBuffer.at(0) != N3Commands::AckSuccess)
        emit errorSignal("Acknowledgement error");

    if(data.size() == 1 && ackBuffer.size() == 1 && data.at(0) == N3Commands::AckReady)
        ackBuffer.append(data);

    if(data.size() >= 2 && data.at(0) == N3Commands::AckSuccess && data.at(1) == N3Commands::AckReady)
        ackBuffer = data.mid(0, 2);
}

//----------------------------------------------------------------------------------------------------------------------
QByteArray *Neptune::getRawData(const unsigned int address)
{
    QByteArray *result = nullptr;

    if((address >= N3Addresses::Summary) && (address < N3Addresses::Summary + N3Constants::SummarySize))
        result = &rawDataSummary;

    if((address >= N3Addresses::JumpDetails) && (address < N3Addresses::JumpDetails + N3Constants::JumpDetailsSize))
        result = &rawDataDetails;

    if((address >= N3Addresses::DeviceSettings) && (address < N3Addresses::DeviceSettings + N3Constants::DeviceSettingsSize))
        result = &rawDataSettings;

    if((address >= N3Addresses::DropZones) && (address < N3Addresses::DropZones + N3Constants::N3NamesSize))
        result = &rawDZNames;

    if((address >= N3Addresses::Airplanes) && (address < N3Addresses::Airplanes + N3Constants::N3NamesSize))
        result = &rawAPNames;

    if((address >= N3Addresses::AlarmNames) && (address < N3Addresses::AlarmNames + N3Constants::N3NamesSize))
        result = &rawAlarmsNames;

    return result;
}



//----------------------------------------------------------------------------------------------------------------------
void Neptune::addJumpsToVector()
{
    uint n_jumps = n_jumps_readed();
    for(uint i = 0; i < n_jumps; ++i)
        m_jumps.emplace_back(jump_from_raw(i));
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::encrypt(uint32_t &dword0, uint32_t &dword1, uint8_t NumRounds) const
{
    uint32_t sum = 0;

    for (auto i = 0; i < NumRounds; ++i)
    {
        dword0 += (((dword1 << 4) ^ (dword1 >> 5)) + dword1) ^ (sum + m_key[sum & 3]);
        sum += XTEA::key_schedule_constant;
        dword1 += (((dword0 << 4) ^ (dword0 >> 5)) + dword0) ^ (sum + m_key[(sum >> 11) & 3]);
    }

}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::decrypt(uint32_t &dword0, uint32_t &dword1, uint8_t NumRounds) const
{
    uint32_t sum = NumRounds * XTEA::key_schedule_constant;

    for (auto i = 0; i < NumRounds; ++i)
    {
        dword1 -= (((dword0 << 4) ^ (dword0 >> 5)) + dword0) ^ (sum + m_key[(sum >> 11) & 3]);
        sum -= XTEA::key_schedule_constant;
        dword0 -= (((dword1 << 4) ^ (dword1 >> 5)) + dword1) ^ (sum + m_key[sum & 3]);
    }
}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<QByteArray> Neptune::cryptPacket(const QByteArray &packet, bool b_encrypt) const
{
    int packet_size = packet.size(); //всегда должно быть кратно N3Constants::BlockSize
    QByteArray local_packet;
    std::unique_ptr<QByteArray> result = std::make_unique<QByteArray>();

    if(packet_size % N3Constants::BlockSize > 0)
        packet_size = packet_size + N3Constants::BlockSize - (packet_size % N3Constants::BlockSize);

    QByteArray const *ref_packet = &packet;
    if(packet.size() < packet_size)
    {
        local_packet = packet; //копирование... (в эту ветку не заходит, но на всякий случай)

#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
        local_packet.append(0, packet_size - packet.size()); //для XP
#else
        char zero = '\0';
        local_packet.append(zero, packet_size - packet.size());
#endif

        ref_packet = &local_packet;
        emit log(tr("Warning: data alignment! Packet size = ") + QString::number(packet.size()));
    }

    for (auto i = 0; i < packet_size; i += 8)
    {
        uint32_t dword0 = BytesOperations::bytesToUInt(ref_packet->at(i + 0), ref_packet->at(i + 1), ref_packet->at(i + 2), ref_packet->at(i + 3));
        uint32_t dword1 = BytesOperations::bytesToUInt(ref_packet->at(i + 4), ref_packet->at(i + 5), ref_packet->at(i + 6), ref_packet->at(i + 7));

        if(b_encrypt)
            encrypt(dword0, dword1, XTEA::NumRounds);
        else
            decrypt(dword0, dword1, XTEA::NumRounds);

        result->append(BytesOperations::UIntToBytes(dword0));
        result->append(BytesOperations::UIntToBytes(dword1));

    }
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<QByteArray> Neptune::makeSingleByteCommand(const N3Commands command) const
{    
    std::unique_ptr<QByteArray> result = std::make_unique<QByteArray>(32, 0);

    (*result)[0] = 1;
    (*result)[1] = static_cast<char>(command);
    (*result)[2] = static_cast<char>(command); //checksum
    result = cryptPacket(*result, true);

    return result;
}


//----------------------------------------------------------------------------------------------------------------------
char Neptune::calculateChecksum(const QByteArray &packet) const
{
    char length = packet[0];
    char checksum = 0;
    for (uchar i = 1; i <= length; ++i)
        checksum += packet[i];

    return checksum;
}

//----------------------------------------------------------------------------------------------------------------------
bool Neptune::verifyType0Record()
{   
    Type0Record = QByteArray::fromHex(Type0Record);

    if (static_cast<uint>(Type0Record[1]) != 0) {
        return false;
    }
    type0Size = static_cast<uint>(Type0Record[0]);
    uint checksum = 0;
    for (uint i = 1; i < type0Size + 1; ++i) {
        checksum += static_cast<uint>(Type0Record[i]);
    }

    checksum %= 256;

    uint packet_checksum = static_cast<uint>(Type0Record[type0Size + 1]) & 0xFF;
    if (checksum != packet_checksum) {
        return false;
    }

    if(Type0Record.at(15) <= 12)
        m_product_type = static_cast<N3Types>(Type0Record.at(15));

    if(Type0Record.at(2) <= 3)
        m_commutication_type = N3CommTypes::CT3;
    else
        if(4 == Type0Record.at(2) || 5 == Type0Record.at(2))
            m_commutication_type = static_cast<N3CommTypes>(Type0Record.at(2));

    m_software_revision = Type0Record[4];


    emit log("Software revision: " + QString::number(m_software_revision));
    emit log("Product type: " + QString::number(static_cast<int>(m_product_type)) + " - " + N3TypeNames[static_cast<int>(m_product_type)]);
    emit log("Communication type: " + QString::number(static_cast<int>(m_commutication_type)));
    return (N3Types::Unknown != m_product_type);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::setEncryptionKey(const QVector<s_key_item> &vec_key)
{    
    QByteArray arrby;
    m_key[0] = 0;
    m_key[1] = 0;
    m_key[2] = 0;
    m_key[3] = 0;

    const int index = 2;

    if(vec_key.size() == 16)
    {
        for(auto key_item: vec_key)
            arrby.append(key_item.absolute_value ? key_item.offset : Type0Record[index + key_item.offset]);

        for(auto i = 0; i < 4; i++)
            for(auto j = 0; j < 4; j++)
                m_key[i] = m_key[i] + ((static_cast<uint>(arrby[i * 4 + j]) & 0xFF) << 8 * j);
    }
    else
        emit errorSignal(QObject::tr("key size!"));
    emit log("Key: " + arrby.toHex());
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::finishedWriteData()
{
    //emit log("END WRITE");
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotConnected()
{
    AbstractDevice::slotConnected();
    keep_alive_worker.clear();
    executeCommand(N3Commands::InitCommand);
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotReady()
{ 
    AbstractDevice::slotReady();

    m_NumBlocks = 0; //!!!

    if (m_commands.count())
        dequeueCommand();
    else
    {
        keep_alive_worker.start();
        emit allCommandsComplete();
    }
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotReadyExit()
{
    AbstractDevice::slotReadyExit();
    keep_alive_worker.stop();
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotReceiving()
{
    AbstractDevice::slotReceiving();
    emit setProgress(m_NumBlocks);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotDisconnected()
{
    AbstractDevice::slotDisconnected();
    m_commands.clear();
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune::slotProcessing()
{
    AbstractDevice::slotProcessing();
    sendLastCommand();
}


//----------------------------------------------------------------------------------------------------------------------
void Neptune::dequeueCommand()
{
    queue_command command;
    if (m_commands.count())
    {
        command = m_commands.dequeue();
        executeCommand(command.m_command, command.m_address, command.m_length, command.m_bytes_to_write, command.m_delay_ms);
    }
}



