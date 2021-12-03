#include "settings.h"
#include <QDir>


const static QString sSettingKind[] = {"", "appearance", "misc", "screen", "environment", "device_types"};


//----------------------------------------------------------------------------------------------------------------------
Settings::Settings(QMainWindow* widget_owner, const QString& organization, const QString& application) :
    owner(widget_owner), qsettings(organization, application), default_return(false)
{
    vec_settings = {
                    std::make_shared<Setting>("geometry", kindset::screen, 0, QVariant(QVariant::Int), false),
                    std::make_shared<Setting>("state", kindset::screen, 0, QVariant(QVariant::Int), false),
                    std::make_shared<Setting>("directory_for_save", kindset::environment, QDir::homePath(), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("current_file", kindset::environment, QString(), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("current_file_delimiter", kindset::environment, QString(";"), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("N3", kindset::device_types, QString("USB Serial Port Driver for Altimaster N3"), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("Atlas", kindset::device_types, QString("USB Serial Port Driver"), QVariant(QVariant::String), false),
                   };
}


//----------------------------------------------------------------------------------------------------------------------
Settings::~Settings()
{        
}



//----------------------------------------------------------------------------------------------------------------------
void Settings::loadSettingsByKind(kindset ks)
{        
    qsettings.beginGroup("/" + name(ks));

    setup_mapset(ks);

    for(auto& key: qsettings.allKeys())
        if(key != "." && key != "")
        {
            auto s = mapset_by_kind.find(key);
            if(s != mapset_by_kind.end())
            {
                Setting *setting = s->second;
                setting->value = qsettings.value("/" + key, setting->getValue());
            }
        }
    qsettings.endGroup();
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::saveSettingsByKind(kindset ks) const
{    
    qsettings.beginGroup("/" + name(ks));
    for(const auto& item: map_set(ks))
        qsettings.setValue("/" + item.second->title, item.second->getValue());

    qsettings.endGroup();
}



//----------------------------------------------------------------------------------------------------------------------
void Settings::loadSettingsScreen()
{    
    if(owner != nullptr)
    {
        loadSettingsByKind(kindset::screen);
        owner->restoreGeometry(getSetting("geometry").toByteArray());
        owner->restoreState(getSetting("state").toByteArray());
    }
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::saveSettingsScreen()
{
    if(owner != nullptr)
    {
        setSetting("geometry", owner->saveGeometry());
        setSetting("state", owner->saveState());
        saveSettingsByKind(kindset::screen);
    }
}


//----------------------------------------------------------------------------------------------------------------------
const QVariant& Settings::getSetting(const QString& title, const kindset ks) const
{    
    const auto& item = map_set(ks).find(title);
    if(item != mapset_by_kind.end())
        return item->second->getValue();

    return default_return;
}


//----------------------------------------------------------------------------------------------------------------------
bool Settings::isChanged(const QString &title, const kindset ks) const
{
    const auto& item = map_set(ks).find(title);
    if(item != mapset_by_kind.end())
        return item->second->isChanged;
    return false;
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::setSetting(const QString& title, QVariant value, const kindset ks)
{    
    auto item = map_set(ks).find(title);
    if(item != mapset_by_kind.end())
    {
        item->second->isChanged = (item->second->value != value);
        item->second->value = value;
    }
}


//----------------------------------------------------------------------------------------------------------------------
const QString& Settings::name(const kindset ks) const
{
    return sSettingKind[static_cast<int>(ks)];
}


//----------------------------------------------------------------------------------------------------------------------
const mset &Settings::map_set(const kindset ks) const
{
    setup_mapset(ks);
    return mapset_by_kind;
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::setup_mapset(const kindset ks) const
{    
    mapset_by_kind.clear();

    for(const auto& setting: vec_settings)
        if(ks == setting->kind || ks == kindset::all)
            mapset_by_kind[setting.get()->title] = setting.get();
}


