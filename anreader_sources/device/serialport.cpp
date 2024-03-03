#include "serialport.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QMutex>
#include <QMutexLocker>

#include "../settings.h"
#include <QDebug>
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

    serial_port = std::make_unique<QSerialPort>();
    mutex = std::make_unique<QMutex>(QMutex::RecursionMode::Recursive);

    this->moveToThread(&thread);
    serial_port->moveToThread(&thread);

    connect(serial_port.get(), &QSerialPort::readyRead, this, &SerialPortThread::s_readyRead);

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

    qDebug() << packet.toHex();
    //QMutexLocker locker(mutex.get());
    while (packet.size() > 0)
    {
        QThread::msleep(msDelay);
        sendRatePacket(packet.mid(0, bytes_to_port));
        QCoreApplication::processEvents();
        packet = packet.mid(bytes_to_port);
    }
    emit finished();
}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::stop()
{
    thread.quit();
    thread.wait();
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
    if(serial_port->isOpen())
        serial_port->close();

}

//----------------------------------------------------------------------------------------------------------------------
void SerialPortThread::sendRatePacket(QByteArray rate)
{
    if(serial_port->write(rate) == -1)
    {
        emit errorSignal(QObject::tr("Failed to write the data to port") + ": " + serial_port->errorString());
        stop();
    }
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
void SerialPortThread::s_readyRead()
{
    QByteArray data = this->serial_port->readAll();
    qDebug() << data.toHex();
    emit readyData(data);
}



