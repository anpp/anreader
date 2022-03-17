#ifndef DWIDGET_H
#define DWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QSerialPortInfo>
#include <QPushButton>
#include <memory>

#include "device/abstractdevice.h"
#include "common/common.h"


class DWidget : public QFrame
{    
    Q_OBJECT
    enum dw_consts: int {base_spacing = 4, base_margin = 4, connect_button_height = 29};
public:
    explicit DWidget(QWidget *parent = nullptr);
    explicit DWidget(dtype device_type, QWidget *parent = nullptr);
    virtual ~DWidget();

    const AbstractDevice& device() const { return *m_device; }
    void setImage(const QString& image_name);
    virtual void setPortName(const QString& port_name);
    virtual void open() = 0;
    virtual void close() = 0;
    dtype getDeviceType() const { return device_type; }
    const QString getPortName() const;
    virtual void create() = 0;
    void clear();
    int getId() const { return m_id; }

    static dtype typeByName(const QString& type_name);


private:
    void init();
    void setVisibleCB(const bool value);

    QLabel lblIcon;
    QLabel lblPort;

    static int s_idGen;

    int m_id;
    int m_wtop_height = 0;
    int m_icon_height = 0;

protected:    
    virtual void setupConnects();
    void addConnectButton();
    void deleteConnectButton();
    void setButton(bool bconnect);
    void setIconHeight(const int icon_height);

    std::unique_ptr<AbstractDevice> m_device;
    QString m_port_name;

    dtype device_type = dtype::N3;    

    QVBoxLayout *lMain;

    QVBoxLayout *lTopVert;
    QHBoxLayout *lTopHorz;
    QHBoxLayout *lConnectButon;

    bool is_visible_connect_button = false;
    QPushButton connect_button;

    QPointer<QWidget> wtop;

signals:
    void newTextOfState(const int id, QString text);
    void setProgress(const int id, unsigned int value);
    void stepProgress(const int id);
    void setHeight(int height);
    void receivedData(const DWidget& widget);
    void afterConnect(const DWidget& widget);
    void controls_is_enabled(const bool);    
    void giveLastJump(int &value);
    void log(const QString&);

public slots:
    virtual void stateChanged();
    void slotReceivedData();
    void slotOpen();
    void slotClose();    

    void slotLoger(const QString& value);
};

#endif // DWIDGET_H
