#ifndef SETTINGS
#define SETTINGS

#include <QSettings>
#include <QWidget>
#include <QSize>
#include <QPoint>
#include <QVariant>
#include <QMainWindow>
#include <vector>
#include <map>
#include <memory>

#include "common/common.h"


struct COM_settings {
    //QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
};



enum class kindset: int {all =0, appearance, misc, screen, environment, device_types, com_port};

class Settings;

class Setting
{
    QString title;
    kindset kind;
    QVariant default_value;
    QVariant value;
    bool isChanged = false;

public:
    Setting(QString atitle, kindset akind, QVariant adefault, QVariant avalue, bool aisChanged):
        title(atitle), kind(akind), default_value(adefault), value(avalue), isChanged(aisChanged) {}
    ~Setting() {}

    const QVariant& getValue() const { return (value.isNull() || !value.isValid())? default_value : value; }

protected:
    friend class Settings;
};


typedef std::shared_ptr<Setting> ptrSetting;
typedef std::map<QString, Setting*> mset;

class Settings {    
    QMainWindow* owner;
    mutable QSettings qsettings;
    mutable mset mapset_by_kind;

    std::vector<ptrSetting> vec_settings;        
    QVariant default_return;
    COM_settings com_settings;

    void setup_mapset(const kindset ks) const;
public:
    Settings(QMainWindow* widget_owner, const QString &organization, const QString &application);
    ~Settings();

    void loadSettingsByKind(kindset ks);
    void saveSettingsByKind(kindset ks) const;

    void loadSettingsScreen();
    void saveSettingsScreen();

    const QVariant& getSetting(const QString& title, const kindset ks = kindset::all) const;
    bool isChanged(const QString& title, const kindset ks = kindset::all) const;
    void setSetting(const QString& title, QVariant value, const kindset ks = kindset::all);
    void clear(const kindset ks);


    void load(){
        loadSettingsByKind(kindset::appearance);
        loadSettingsByKind(kindset::misc);
        loadSettingsByKind(kindset::environment);
        loadSettingsByKind(kindset::device_types);
        loadSettingsByKind(kindset::com_port);
        loadSettingsScreen();
    }

    void save() {
        saveSettingsByKind(kindset::appearance);
        saveSettingsByKind(kindset::misc);
        saveSettingsByKind(kindset::environment);
        saveSettingsByKind(kindset::device_types);
        saveSettingsByKind(kindset::com_port);
        saveSettingsScreen();
    }


    const QString& name(const kindset ks) const;
    const mset& map_set(const kindset ks) const;

    const COM_settings& COMSettings();
};

#endif // SETTINGS

