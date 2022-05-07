#ifndef COMENUMERATOR_H
#define COMENUMERATOR_H

//#include <QWidget>
#include <QThread>
#include <QTableWidget>

#include "device_widgets/dwidget.h"
#include "device_widgets/n3widget.h"
#include "settings.h"
#include "common/common.h"

Q_DECLARE_METATYPE(QSerialPortInfo)
Q_DECLARE_METATYPE(QList<QSerialPortInfo>)

typedef QPointer<DWidget> ptrWidget;


class WorkerEnumerator : public QObject
{
    Q_OBJECT
    enum Enumerator: int {DelayMs = 2000};
public:
    explicit WorkerEnumerator() :QObject() { }
    ~WorkerEnumerator() {}


private:
    bool isEnumerated = false;
    std::list<QSerialPortInfo> ports;
signals:
    void finished();
    void attachDevice(QSerialPortInfo port_info);
    void detachDevice(QSerialPortInfo port_info);

public slots:
    void enumerate();
    void stop();

};




//=========================================================================================================================
class DevicesWidget : public QWidget
{
    Q_OBJECT
    enum DevicesWidget_defs: int {default_width = 240};
public:
    explicit DevicesWidget(const Settings& asettings, QWidget *parent = nullptr);
    ~DevicesWidget();

    void start();
    void createDefaults();

private:
    void init();
    void createDevice(ptrWidget device_widget);
    dtype typeByDescription(const QString& description) const;

    QThread threadEnumerator;
    WorkerEnumerator worker;
    std::vector<ptrWidget> devices;

    QTableWidget table_devices;
    const Settings& settings;

protected:

signals:
    void stopEnumerate();
    void createStatusFrame(const int id);
    void log(const QString&);

private slots:    
    void attach(QSerialPortInfo port_info);
    void detach(QSerialPortInfo port_info);
    void setHeightRow(int height);

};

#endif // COMENUMERATOR_H
