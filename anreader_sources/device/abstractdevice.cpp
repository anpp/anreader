#include "abstractdevice.h"

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
    initStateMachine();
    connect(this, &AbstractDevice::errorSignal, this, &AbstractDevice::slotError);

    sm.start();
}

//----------------------------------------------------------------------------------------------------------------------
AbstractDevice::~AbstractDevice()
{
}

//----------------------------------------------------------------------------------------------------------------------
void AbstractDevice::initStateMachine()
{
    commonState->setInitialState(disconnectedState);

    sm.addState(commonState);
    sm.addState(errorState);

    sm.setInitialState(commonState);

    commonState->addTransition(this, &AbstractDevice::errorStateSignal, errorState);
    commonState->addTransition(this, &AbstractDevice::disconnectStateSignal, disconnectedState);
    errorState->addTransition(this, &AbstractDevice::disconnectStateSignal, disconnectedState);

    disconnectedState->addTransition(this, &AbstractDevice::connectStateSignal, connectedState);
    connectedState->addTransition(this, &AbstractDevice::initializeStateSignal, initializingState);
    initializingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState);
    readyState->addTransition(this, &AbstractDevice::processingStateSignal, processingState);

    processingState->addTransition(this, &AbstractDevice::receiveingStateSignal, receivingState);
    processingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState);
    receivingState->addTransition(this, &AbstractDevice::readyStateSignal, readyState);
    receivingState->addTransition(this, &AbstractDevice::processingStateSignal, processingState);

    connect(connectedState, &QState::entered, this, &AbstractDevice::slotConnected);
    connect(initializingState, &QState::entered, this, &AbstractDevice::slotInitializing);
    connect(readyState, &QState::entered, this, &AbstractDevice::slotReady);
    connect(processingState, &QState::entered, this, &AbstractDevice::slotProcessing);
    connect(receivingState, &QState::entered, this, &AbstractDevice::slotReceiving);
    connect(errorState, &QState::entered, this, &AbstractDevice::slotStateError);
    connect(disconnectedState, &QState::entered, this, &AbstractDevice::slotDisconnected);

    connect(readyState, &QState::exited, this, &AbstractDevice::slotReadyExit);


    connectedState->assignProperty(&sm, "state", DeviceStates::Connected);
    initializingState->assignProperty(&sm, "state", DeviceStates::Initializing);
    readyState->assignProperty(&sm, "state", DeviceStates::Ready);
    processingState->assignProperty(&sm, "state", DeviceStates::Processing);
    receivingState->assignProperty(&sm, "state", DeviceStates::Receiving);
    errorState->assignProperty(&sm, "state", DeviceStates::Error);
    disconnectedState->assignProperty(&sm, "state", DeviceStates::Disconnected);

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
    serialport_transition_common_disconnected = commonState->addTransition(sp.get(), &SerialPortThread::aboutToClose, disconnectedState);
    serialport_transition_error_disconnected = errorState->addTransition(sp.get(), &SerialPortThread::aboutToClose, disconnectedState);    

    connect(sp.get(), &SerialPortThread::errorSignal, this, &AbstractDevice::errorSignal);
    connect(this, &AbstractDevice::sopen, sp.get(), &SerialPortThread::sopen);
    connect(sp.get(), &SerialPortThread::connected, this, &AbstractDevice::connected);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &AbstractDevice::disconnectStateSignal);

    #ifdef Q_OS_WIN64
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
const QString &AbstractDevice::getErrorMessage() const
{
    return m_ErrorMessage;
}

//----------------------------------------------------------------------------------------------------------------------
AbstractDevice::DeviceStates AbstractDevice::state() const
{
    if(sm.property("state").isValid())
        return static_cast<DeviceStates>(sm.property("state").toInt());
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
    if(!sm.isRunning()) return;

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
  removeComPort();  
  sp.reset();
  emit changedState();
}


