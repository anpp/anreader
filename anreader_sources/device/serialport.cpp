#include "serialport.h"
#include <QCoreApplication>
#include <QElapsedTimer>

#include "../settings.h"

//----------------------------------------------------------------------------------------------------------------------
void WorkerPacketSender::sendPacket()
{
    while (packet.size() > 0)
    {
        QThread::msleep(msDelay);
        emit sendRatePacket(packet.mid(0, bytes_to_port));
        packet = packet.mid(bytes_to_port);
    }
    emit finished();
}

void WorkerPacketSender::setPacket(QByteArray data)
{
    packet = data;
}



//=======================================================================================================

//----------------------------------------------------------------------------------------------------------------------
SerialPortThread::SerialPortThread(QObject *parent) :
    QSerialPort (parent)
{
    init();
}

//----------------------------------------------------------------------------------------------------------------------
SerialPortThread::SerialPortThread() :
    QSerialPort (nullptr)
{
    init();
}

//----------------------------------------------------------------------------------------------------------------------
SerialPortThread::~SerialPortThread()
{
    stop();   
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::init()
{
    qRegisterMetaType <QSerialPort::SerialPortError> ();

    worker.moveToThread(&worker_thread);
    connect(&worker_thread, &QThread::started, &worker, &WorkerPacketSender::sendPacket);
    connect(&worker, &WorkerPacketSender::finished, &worker_thread, &QThread::quit);
    connect(&worker, &WorkerPacketSender::finished, this, &SerialPortThread::finished);
    connect(&worker, &WorkerPacketSender::sendRatePacket, this, &SerialPortThread::sendRatePacket);
    connect(this, &SerialPortThread::readyRead, this, &SerialPortThread::s_readyRead);

    start();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::delay(const unsigned long ms) const
{
    QElapsedTimer timer;
    timer.start();
    while (!timer.hasExpired(ms))
    {
        QThread::msleep(10);
        QCoreApplication::processEvents();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::setPortSettings()
{
    //default settings
    ps.baudRate = QSerialPort::Baud57600;
    ps.dataBits = QSerialPort::Data8;
    ps.parity = QSerialPort::NoParity;
    ps.stopBits = QSerialPort::OneStop;
    ps.flowControl = QSerialPort::HardwareControl;

    if(Settings::Instance())
    {
        const COM_settings &com_settings = Settings::Instance()->COMSettings();
        ps.baudRate = com_settings.baudRate;
        ps.dataBits = com_settings.dataBits;
        ps.parity = com_settings.parity;
        ps.stopBits = com_settings.stopBits;
        ps.flowControl = com_settings.flowControl;
    }

    setBaudRate(ps.baudRate);
    setDataBits(ps.dataBits);
    setParity(ps.parity);
    setStopBits(ps.stopBits);
    setFlowControl(ps.flowControl);
}


//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::moveToInnerThread()
{
    this->moveToThread(&thread);
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sendPacket(QByteArray packet, const uint delayms)
{
    if(delayms > 0)
        delay(delayms);

    if(packet.size() <= bytes_to_port)
    {
        sendRatePacket(packet);
        emit finished();
    }
    else
    {
        worker_thread.wait();
        worker.setPacket(packet);
        worker_thread.start();
    }

}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::stop()
{
    thread.quit();
    thread.wait();
    worker_thread.quit();
    worker_thread.wait();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::start()
{
    if(!thread.isRunning())
        thread.start(QThread::Priority::HighestPriority);
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::close()
{
    stop();
    if(this->isOpen())
        QSerialPort::close();

}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sendRatePacket(QByteArray rate)
{    

    if(this->write(rate) == -1)
    {
        emit errorSignal(QObject::tr("Failed to write the data to port") + ": " + errorString());
        stop();
    }
    else
        this->flush();
}


//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sopen(QString com_port)
{
    setPortName(com_port);
    if (!open(QIODevice::ReadWrite)){
        emit errorSignal(errorString());
        return;
    }

    setPortSettings();

    emit connected();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::s_readyRead()
{
    QByteArray data = this->readAll();
    emit readyData(data);
}



