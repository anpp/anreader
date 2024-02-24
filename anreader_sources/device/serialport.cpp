#include "serialport.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>

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
SerialPortThread::SerialPortThread()
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

    this->moveToThread(&thread);
    worker.moveToThread(&worker_thread);

    connect(&worker_thread, &QThread::started, &worker, &WorkerPacketSender::sendPacket);
    connect(&worker, &WorkerPacketSender::finished, &worker_thread, &QThread::quit);
    connect(&worker, &WorkerPacketSender::finished, this, &SerialPortThread::finished);
    connect(&worker, &WorkerPacketSender::sendRatePacket, this, &SerialPortThread::sendRatePacket);

    connect(&thread, &QThread::started, this, &SerialPortThread::process);

    start();
    while(!serial_port) QThread::msleep(10);
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

    serial_port->setBaudRate(ps.baudRate);
    serial_port->setDataBits(ps.dataBits);
    serial_port->setParity(ps.parity);
    serial_port->setStopBits(ps.stopBits);
    serial_port->setFlowControl(ps.flowControl);
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
    working = false;
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
    working = true;
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::close()
{
    stop();
    if(this->serial_port->isOpen())
        serial_port->close();

}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sendRatePacket(QByteArray rate)
{
    if(this->serial_port->write(rate) == -1)
    {
        emit errorSignal(QObject::tr("Failed to write the data to port") + ": " + serial_port->errorString());
        stop();
    }
    else
        this->serial_port->flush();
}


//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sopen(QString com_port)
{
    serial_port->setPortName(com_port);
    if (!serial_port->open(QIODevice::ReadWrite)){
        emit errorSignal(serial_port->errorString() + " : " + com_port);
        return;
    }

    setPortSettings();

    emit connected();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::process()
{
    QEventLoop event_loop;
    serial_port = std::make_unique<QSerialPort>();
    connect(serial_port.get(), &QSerialPort::readyRead, this, &SerialPortThread::s_readyRead);

    while(working)
    {
        QThread::msleep(10);
        event_loop.processEvents();
    }
    emit finished();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::s_readyRead()
{
    QByteArray data = this->serial_port->readAll();
    emit readyData(data);

}



