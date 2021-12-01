#ifndef SETTINGS
#define SETTINGS

#include <QSettings>
#include <QWidget>
#include <QSize>
#include <QPoint>
#include <QMap>
#include <QVariant>
#include <QMainWindow>
#include <vector>
#include <map>
#include <memory>

#include "common.h"


enum class kindset: int {appearance = 0, misc, screen, environment, device_types};

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
typedef std::map<QString, QVariant> mset;

class Settings {    
    QMainWindow* owner;
    mutable QSettings qsettings;
    mutable mset mapset_by_kind;

    std::vector<ptrSetting> vec_settings;
    QMap<QString, Setting*> mapset;
    std::map<Setting*, kindset> k_mapset;
    QVariant default_return;       

    void setup_mapset(const kindset ks) const;
public:
    Settings(QMainWindow* widget_owner, const QString &organization, const QString &application);
    ~Settings();

    bool loadSettingsByKind(kindset ks);
    void saveSettingsByKind(kindset ks) const;

    void loadSettingsScreen();
    void saveSettingsScreen();

    const QVariant& getSetting(const QString& title) const;
    bool isChanged(const QString& title) const;
    void setSetting(const QString& title, QVariant value);


    void load(){
        loadSettingsByKind(kindset::appearance);
        loadSettingsByKind(kindset::misc);
        loadSettingsByKind(kindset::environment);
        loadSettingsByKind(kindset::device_types);
        loadSettingsScreen();
    }

    void save() {
        saveSettingsByKind(kindset::appearance);
        saveSettingsByKind(kindset::misc);
        saveSettingsByKind(kindset::environment);
        saveSettingsByKind(kindset::device_types);
        saveSettingsScreen();
    }

    //const QVector<ptrSetting>& getListSettings() {return vec_settings;}
    //const QMap<QString, ptrSetting>& getMapSettings() {return mapset;}

    const QString& getSettingsName(const kindset ks) const;
    const mset& map_set(const kindset ks) const;
};

#endif // SETTINGS

