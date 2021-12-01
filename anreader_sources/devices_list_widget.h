#ifndef COMENUMERATOR_H
#define COMENUMERATOR_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QThread>
#include <QTimer>
#include <QSerialPortInfo>
#include <QTableWidget>
#include <QHeaderView>
#include <memory>

#include "device_widgets/dwidget.h"
#include "device_widgets/n3widget.h"
#include "settings.h"

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
    void newTextOfState(const int id, QString text);
    void setProgress(const int id, unsigned int value);
    void stepProgress(const int id);
    void receivedData(const DWidget& widget);
    void afterConnect(const DWidget& widget);
    void createStatusFrame(const int id);
    void controls_is_enabled(const bool);
    void giveLastJump(int &value);
    void log(const QString&);

private slots:    
    void attach(QSerialPortInfo port_info);
    void detach(QSerialPortInfo port_info);
    void setHeightRow(int height);

};

#endif // COMENUMERATOR_H
