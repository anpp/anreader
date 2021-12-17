#ifndef SETTINGSCOMPORT_H
#define SETTINGSCOMPORT_H

#include <QWidget>

#include "ui_settings_com_port.h"
#include "common/common.h"

class SettingsCOMPort : public QWidget,
                        protected Ui::COMPortWidget
{
    Q_OBJECT

    void fillPortsParameters();

     QIntValidator *m_intValidator = nullptr;
public:
    explicit SettingsCOMPort(QWidget *parent = nullptr);

    void updateSettings(COM_settings& settings);

signals:

private slots:
     void checkCustomBaudRatePolicy(int idx);
};

#endif // SETTINGSCOMPORT_H
