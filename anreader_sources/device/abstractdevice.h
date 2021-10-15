#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H


#include <QDebug>

#include <QObject>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDataStream>
#include <iostream>
#include <QStringList>
#include <QString>
#include <QMutex>
#include <QStateMachine>
#include <QFinalState>
#include <QCoreApplication>
#include <memory>
#include <algorithm>


#include "serialport.h"

//#include "constants.h"
#include "jumps/customjump.h"
#include "bytes_operations.h"
#include "asummaryinfo.h"
#include "adevicesettings.h"
#include "anames.h"



class AbstractDevice : public QObject
{    
    Q_OBJECT
public:
    enum  DeviceStates: int {Unk = -1, Disconnected = 0, Connected = 1, Initializing = 2, Ready = 3, Processing = 4, Receiving = 5, Error = 6};

    explicit AbstractDevice(QString portName, QObject *parent = nullptr);
    virtual ~AbstractDevice();


    virtual const QString &deviceName() const = 0;
    const QString& comPort() const { return m_COMPort; }
    virtual void initStateMachine();
    const QString& getErrorMessage() const;
    virtual std::unique_ptr<QByteArray> jump_at(uint index) const = 0;
    DeviceStates state() const;
    const t_jumps& jumps() const { return m_jumps; }

    virtual void read_details_jumps(unsigned int jump_index, unsigned int num_jumps) = 0;
    virtual void read_summary_jumps() = 0;
    virtual void read_settings() = 0;
    virtual void read_dropzones() = 0;
    virtual void read_airplanes() = 0;
    virtual void end_communication() = 0;
    virtual uint n_iterations_by_jumps(const uint n_jumps) = 0;
    void clearJumps() { m_jumps.clear(); }

    virtual const QString getSerialNumber() const = 0;

    const ASummaryInfo& summary() const;
    const ADeviceSettings& settings() const;
    const ANames& dropzones() const;
    const ANames& airplanes() const;
    const QString& state_str() const;

protected:    
    void startTimeoutTimer(const int ms) const;
    void stopTimeoutTimer() const;
    virtual void setupComPort();
    virtual void removeComPort();

    std::unique_ptr<SerialPortThread> sp;

    std::unique_ptr<ASummaryInfo> m_summary;
    std::unique_ptr<ADeviceSettings> m_settings;
    std::unique_ptr<ANames> m_dropzones;
    std::unique_ptr<ANames> m_airplanes;

    QStateMachine sm;
    QState *commonState = new QState();
    QState *disconnectedState = new QState(commonState);
    QState *connectedState = new QState(commonState);
    QState *initializingState = new QState(commonState);
    QState *readyState = new QState(commonState);
    QState *processingState = new QState(commonState);
    QState *receivingState = new QState(commonState);
    QState *errorState = new QState();

    t_jumps m_jumps;

    mutable QTimer timeout_timer;


private:
    QString m_ErrorMessage;
    QString m_COMPort;    

    QAbstractTransition *serialport_transition_common_disconnected{nullptr};
    QAbstractTransition *serialport_transition_error_disconnected{nullptr};

signals:
    void sendPacket( QByteArray packet, const uint delayms = 0);
    //void commandFinished();
    void errorSignal(const QString&) const;

    void errorStateSignal();
    void connectStateSignal();
    void initializeStateSignal();
    void readyStateSignal();
    void processingStateSignal();
    void receiveingStateSignal();
    void disconnectStateSignal();
    void allCommandsComplete();
    void changedState();

    void setProgress(const unsigned int value);
    void stepProgress();

    void sopen(QString com_port);

    void log(const QString&) const;

public slots:
    void connected();   
    void slotSerialPortError(QSerialPort::SerialPortError error);
    virtual void processData(QByteArray data) = 0;
    virtual void open();
    virtual void close();
    void destroy_port();
    void timeout() { emit errorSignal(QObject::tr("Timeout")); }

    virtual void slotConnected();
    virtual void slotInitializing();
    virtual void slotReady();
    virtual void slotReadyExit();
    virtual void slotProcessing();
    virtual void slotReceiving();
    virtual void slotError(const QString&);
    virtual void slotStateError();
    virtual void slotDisconnected();    
};


#endif // ABSTRACTDEVICE_H
