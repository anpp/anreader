#include "abstractdevice.h"

#include <QCoreApplication>
#include <QStateMachine>
#include <QSignalTransition>
#include <QFinalState>
#include <QDateTime>
#include <algorithm>

const static QString StateNames[] = {QObject::tr("Disconnected"),
                               QObject::tr("Connected"),
                               QObject::tr("Initializing..."),
                               QObject::tr("Ready"),
                               QObject::tr("Processing..."),
                               QObject::tr("Receiving data..."),
                               QObject::tr("Error"),
                              };


//----------------------------------------------------------------------------------------------------------------------
AbstractDevice::AbstractDevice(QString portName, QObject *parent) : QObject(parent), m_ErrorMessage(""), m_COMPort(portName)
{
    m_datetime_temp = std::make_unique<QDateTime>();
    m_datetime_clock = std::make_unique<QDateTime>();
    m_datetime = std::make_unique<QDateTime>();

    sm = std::make_unique<QStateMachine>();

    commonState = std::make_unique<QState>();
    disconnectedState = std::make_unique<QState>(commonState.get());
    connectedState = std::make_unique<QState>(commonState.get());
    initializingState = std::make_unique<QState>(commonState.get());
    readyState = std::make_unique<QState>(commonState.get());
    processingState = std::make_unique<QState>(commonState.get());
    receivingState = std::make_unique<QState>(commonState.get());
    errorState = std::make_unique<QState>();


    initStateMachine();
    connect(this, &AbstractDevice::errorSignal, this, &AbstractDevice::slotError);

    sm->start();
}

//----------------------------------------------------------------------------------------------------------------------
AbstractDevice::~AbstractDevice()
{
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::initStateMachine()
{
    commonState->setInitialState(disconnectedState.get());

    sm->addState(commonState.get());
    sm->addState(errorState.get());

    sm->setInitialState(commonState.get());

    commonState->addTransition(this, &AbstractDevice::errorStateSignal, errorState.get());
    commonState->addTransition(this, &AbstractDevice::disconnectStateSignal, disconnectedState.get());
    errorState->addTransition(this, &AbstractDevice::disconnectStateSignal, disconnectedState.get());

    disconnectedState->addTransition(this, &AbstractDevice::connectStateSignal, connectedState.get());
    connectedState->addTransition(this, &AbstractDevice::initializeStateSignal, initializingState.get());
    initializingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState.get());
    readyState->addTransition(this, &AbstractDevice::processingStateSignal, processingState.get());

    processingState->addTransition(this, &AbstractDevice::receiveingStateSignal, receivingState.get());
    processingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState.get());
    receivingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState.get());
    receivingState->addTransition(this, &AbstractDevice::processingStateSignal, processingState.get());

    connect(connectedState.get(), &QState::entered, this, &AbstractDevice::slotConnected);
    connect(initializingState.get(), &QState::entered, this, &AbstractDevice::slotInitializing);
    connect(readyState.get(), &QState::entered, this, &AbstractDevice::slotReady);
    connect(processingState.get(), &QState::entered, this, &AbstractDevice::slotProcessing);
    connect(receivingState.get(), &QState::entered, this, &AbstractDevice::slotReceiving);
    connect(errorState.get(), &QState::entered, this, &AbstractDevice::slotStateError);
    connect(disconnectedState.get(), &QState::entered, this, &AbstractDevice::slotDisconnected);

    connect(readyState.get(), &QState::exited, this, &AbstractDevice::slotReadyExit);


    connectedState->assignProperty(sm.get(), "state", DeviceStates::Connected);
    initializingState->assignProperty(sm.get(), "state", DeviceStates::Initializing);
    readyState->assignProperty(sm.get(), "state", DeviceStates::Ready);
    processingState->assignProperty(sm.get(), "state", DeviceStates::Processing);
    receivingState->assignProperty(sm.get(), "state", DeviceStates::Receiving);
    errorState->assignProperty(sm.get(), "state", DeviceStates::Error);
    disconnectedState->assignProperty(sm.get(), "state", DeviceStates::Disconnected);

}


//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::startTimeoutTimer(const int ms) const
{
    timeout_timer.setInterval(ms);
    connect(&timeout_timer, &QTimer::timeout, this, &AbstractDevice::timeout);
    timeout_timer.start();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::stopTimeoutTimer() const
{
    timeout_timer.stop();
    disconnect(&timeout_timer, &QTimer::timeout, this, &AbstractDevice::timeout);
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::setupComPort()
{
    serialport_transition_common_disconnected = commonState->addTransition(sp.get(), &SerialPortThread::aboutToClose, disconnectedState.get());
    serialport_transition_error_disconnected = errorState->addTransition(sp.get(), &SerialPortThread::aboutToClose, disconnectedState.get());

    connect(sp.get(), &SerialPortThread::errorSignal, this, &AbstractDevice::errorSignal);
    connect(this, &AbstractDevice::sopen, sp.get(), &SerialPortThread::sopen);
    connect(sp.get(), &SerialPortThread::connected, this, &AbstractDevice::connected);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &AbstractDevice::disconnectStateSignal);
    #if QT_VERSION > QT_VERSION_CHECK(5, 6, 3)
        connect(sp.get(), &SerialPortThread::errorOccurred, this, &AbstractDevice::slotSerialPortError); //Для XP (Qt 5.6) закомментировать
    #endif
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::removeComPort()
{
    if(serialport_transition_common_disconnected != nullptr)
        commonState->removeTransition(serialport_transition_common_disconnected);

    if(serialport_transition_error_disconnected != nullptr)
        errorState->removeTransition(serialport_transition_error_disconnected);

    serialport_transition_common_disconnected = nullptr;
    serialport_transition_error_disconnected = nullptr;

    disconnect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &AbstractDevice::disconnectStateSignal);

    if(sp) sp->disconnect();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::set_date_time(const QDateTime &a_datetime)
{
    *m_datetime = a_datetime;
    *m_datetime_clock = QDateTime::currentDateTime();
}

//----------------------------------------------------------------------------------------------------------------------
const QString &AbstractDevice::getErrorMessage() const
{
    return m_ErrorMessage;
}

//----------------------------------------------------------------------------------------------------------------------
AbstractDevice::DeviceStates AbstractDevice::state() const
{
    if(sm->property("state").isValid())
        return static_cast<DeviceStates>(sm->property("state").toInt());
    else {
        return  DeviceStates::Unk;
    }

}

//----------------------------------------------------------------------------------------------------------------------
const ASummaryInfo &AbstractDevice::summary() const
{
    return *m_summary;
}

//----------------------------------------------------------------------------------------------------------------------
const ADeviceSettings &AbstractDevice::settings() const
{
    return *m_settings;
}

const ANames &AbstractDevice::dropzones() const
{
    return *m_dropzones;
}

const ANames &AbstractDevice::airplanes() const
{
    return *m_airplanes;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &AbstractDevice::state_str() const
{
    return StateNames[state()];
}

//----------------------------------------------------------------------------------------------------------------------
//дата и время читаются из прибора один раз, потом возвращается как бы "живое" время, расчитанное
const QDateTime &AbstractDevice::dateTime() const
{
    *m_datetime_temp = m_datetime->addSecs(m_datetime_clock->secsTo(QDateTime::currentDateTime()));

    return *m_datetime_temp;
}


//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotSerialPortError(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        if(sp) sp->stop();

        switch(error)
        {
        case QSerialPort::DeviceNotFoundError:
        emit errorSignal(QObject::tr("Device not found"));
        break;

        case QSerialPort::PermissionError:
        emit errorSignal(QObject::tr("Permission error"));
        break;

        case QSerialPort::OpenError:
        emit errorSignal(QObject::tr("Open error"));
        break;

        case QSerialPort::NotOpenError:
        emit errorSignal(QObject::tr("Not open error"));
        break;

        case QSerialPort::ParityError:
        emit errorSignal(QObject::tr("Parity error"));
        break;

        case QSerialPort::FramingError:
        emit errorSignal(QObject::tr("Framing error"));
        break;

        case QSerialPort::BreakConditionError:
        emit errorSignal(QObject::tr("Break condition error"));
        break;

        case QSerialPort::WriteError:
        emit errorSignal(QObject::tr("Write error"));
        break;

        case QSerialPort::ReadError:
        emit errorSignal(QObject::tr("Read error"));
        break;

        case QSerialPort::ResourceError:
        emit errorSignal(QObject::tr("Resource error"));
        break;

        case QSerialPort::UnsupportedOperationError:
        emit errorSignal(QObject::tr("Unsupported operation error"));
        break;

        case QSerialPort::TimeoutError:
        emit errorSignal(QObject::tr("Timeout error"));
        break;

        case QSerialPort::UnknownError:
        emit errorSignal(QObject::tr("Unknown error"));
        break;

        default:
            break;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::open()
{
    if(!sm->isRunning()) return;

    if(!sp)
    {
        sp = std::make_unique<SerialPortThread>();
        sp->moveToInnerThread();
        setupComPort();
    }
    emit log("COM Port: " + m_COMPort);
    emit sopen(m_COMPort);
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::close()
{
    stopTimeoutTimer();
    if(sp) sp->close();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::destroy_port()
{
    stopTimeoutTimer();
    emit disconnectStateSignal();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::connected()
{
    emit connectStateSignal();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotConnected()
{
    emit log("STATE MACHINE: Connected");
    emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotInitializing()
{    
    emit log("STATE MACHINE: Initializing...");
    emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotReady()
{    
    emit log("STATE MACHINE: Ready");
    emit log("");
    emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotReadyExit()
{
    //emit toLog("STATE MACHINE: Ready exit");
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotProcessing()
{    
    emit log("STATE MACHINE: Processing...");
    emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotReceiving()
{    
    emit log("STATE MACHINE: Receiving data...");
    emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotError(const QString& s)
{
  if(s != "")
    m_ErrorMessage = s;  
  emit errorStateSignal();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotStateError()
{
   emit log("STATE MACHINE: Error");
   emit log("Error: " + getErrorMessage());
   emit changedState();
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::slotDisconnected()
{  
  emit log("STATE MACHINE: Disconnected");
  m_jumps.clear();
  m_dropzones->clear();
  m_airplanes->clear();
  removeComPort();  
  sp.reset();
  emit changedState();
}


