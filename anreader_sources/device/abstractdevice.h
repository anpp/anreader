#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H

#include <QTimer>

#include "serialport.h"

#include "jumps/customjump.h"
#include "asummaryinfo.h"
#include "adevicesettings.h"
#include "anames.h"


class QDateTime;
class QStateMachine;
class QState;
class QAbstractTransition;

class AbstractDevice : public QObject
{    
    Q_OBJECT
public:
    enum DeviceStates: int {Unk = -1, Disconnected = 0, Connected = 1, Initializing = 2, Ready = 3, Processing = 4,
                            Receiving = 5, Sending = 6, Error = 7};

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
    virtual void read_datetime() = 0;
    virtual void end_communication() = 0;    
    virtual uint n_iterations_by_jumps(const uint n_jumps) = 0;
    virtual void set_date_time(const QDateTime& a_datetime);
    virtual void write_settings() = 0;
    virtual void write_dropzones() = 0;
    virtual void write_airplanes() = 0;
    virtual void write_summary_jumps() = 0;

    virtual int revision() const = 0;
    virtual int product_type() const = 0;
    void clearJumps() { m_jumps.clear(); }

    virtual const QString getSerialNumber() const = 0;

    ASummaryInfo& summary() const;
    ADeviceSettings& settings() const;
    ANames& dropzones() const;
    ANames& airplanes() const;
    const QString& state_str() const;
    const QDateTime& dateTime() const;

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

    std::unique_ptr<QStateMachine> sm;
    std::unique_ptr<QState> commonState;
    std::unique_ptr<QState> disconnectedState;
    std::unique_ptr<QState> connectedState;
    std::unique_ptr<QState> initializingState;
    std::unique_ptr<QState> readyState;
    std::unique_ptr<QState> processingState;
    std::unique_ptr<QState> receivingState;
    std::unique_ptr<QState> sendingState;
    std::unique_ptr<QState> errorState;

    t_jumps m_jumps;    

    mutable QTimer timeout_timer;


private:
    QString m_ErrorMessage;
    QString m_COMPort;    

    QAbstractTransition *serialport_transition_common_disconnected{nullptr};
    QAbstractTransition *serialport_transition_error_disconnected{nullptr};

    std::unique_ptr<QDateTime> m_datetime; //дата, прочитанная из прибора
    std::unique_ptr<QDateTime> m_datetime_clock; // дата-время чтения даты из прибора
    mutable std::unique_ptr<QDateTime> m_datetime_temp;

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
    void sendingStateSignal();
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
    virtual void slotSending();
    virtual void slotError(const QString&);
    virtual void slotStateError();
    virtual void slotDisconnected();    
};


#endif // ABSTRACTDEVICE_H
