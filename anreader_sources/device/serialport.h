#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QThread>
#include <memory>

class QMutex;

Q_DECLARE_METATYPE(QSerialPort::SerialPortError)


class SerialPortThread : public QObject
{
    Q_OBJECT

public:
    struct port_settings {
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;

    };

private:
    std::unique_ptr<QSerialPort> serial_port;
    QThread thread;
    int bytes_to_port = 1;
    unsigned long msDelay = 50;
    port_settings ps;
    std::unique_ptr<QMutex> mutex;

    void init();
    void delay(const unsigned long ms) const;
    void sendPacketToPort(const QByteArray &packet);
    void setPortSettings();

public:
    explicit SerialPortThread();
    ~SerialPortThread() override;

    void stop();
    void start();
    void setBytesToPort(const int value) { bytes_to_port = value; }
    void setDelay(const unsigned long value) { msDelay = value; }
    void close();

    QSerialPort& SerialPort() const { return *serial_port; } ;

signals:
    void finished();
    void errorSignal(const QString&);
    void readyData(QByteArray data);
    void connected();

public slots:            
    void s_readyRead();
    void sendPacket(QByteArray packet, const uint delayms = 0);
    void sendRatePacket(QByteArray rate);
    void sopen(QString com_port);
};

#endif // SERIALPORT_H
