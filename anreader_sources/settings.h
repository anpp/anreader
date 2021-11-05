#ifndef SETTINGS
#define SETTINGS

#include <QSettings>
#include <QWidget>
#include <QSize>
#include <QPoint>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QMainWindow>
#include <QDebug>
#include <memory>

#include "common.h"


enum class kindset: int {appearance = 0, misc, screen, environment, aircrafts};

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

protected:
    friend class Settings;
};


typedef std::shared_ptr<Setting> ptrSetting;


class Settings {    
    QMainWindow* owner;
    QSettings qsettings;
    QVector<ptrSetting> vec_settings;
    QMap<QString, Setting*> mapset;
    map_APs& map_aircrafts;
    QVariant default_return;

public:
    Settings(QMainWindow* widget_owner, const QString &organization, const QString &application, map_APs& aps);
    ~Settings();

    bool loadSettingsByKind(kindset ks);
    void saveSettingsByKind(kindset ks);

    void loadSettingsScreen();
    void saveSettingsScreen();
    void loadSettingsAircrafts();
    void saveSettingsAircrafts();

    const QVariant& getSetting(const QString& title);
    bool isChanged(const QString& title);
    void setSetting(const QString& title, QVariant value);


    void load(){
        loadSettingsByKind(kindset::appearance);
        loadSettingsByKind(kindset::misc);
        loadSettingsByKind(kindset::environment);
        loadSettingsAircrafts();
        loadSettingsScreen();
    }

    void save(){
        saveSettingsByKind(kindset::appearance);
        saveSettingsByKind(kindset::misc);
        saveSettingsByKind(kindset::environment);
        saveSettingsAircrafts();
        saveSettingsScreen();
    }

    //const QVector<ptrSetting>& getListSettings() {return vec_settings;}
    //const QMap<QString, ptrSetting>& getMapSettings() {return mapset;}

    const QString& getSettingsName(kindset ks);
};

#endif // SETTINGS

