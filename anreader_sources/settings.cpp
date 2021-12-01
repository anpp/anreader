#include "settings.h"
#include <QDir>


const static QString sSettingKind[] = {"appearance", "misc", "screen", "environment", "device_types"};


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
                    std::make_shared<Setting>("N3", kindset::device_types, QString("Altimaster N3"), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("Atlas", kindset::device_types, QString("USB"), QVariant(QVariant::String), false),
                   };

    for(auto& s: vec_settings) {mapset[s.get()->title] = s.get();}
    for(auto& s: vec_settings) {k_mapset[s.get()] = s.get()->kind;}
}


//----------------------------------------------------------------------------------------------------------------------
Settings::~Settings()
{        
}



//----------------------------------------------------------------------------------------------------------------------
bool Settings::loadSettingsByKind(kindset ks)
{
    bool result = false;
    qsettings.beginGroup("/" + sSettingKind[static_cast<int>(ks)]);
    for(auto& s: vec_settings){
        if (s->kind == ks)
        {
            s->value = qsettings.value("/" + s->title, s->default_value);
            result = true;
        }
    }
    qsettings.endGroup();
    return result;
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::saveSettingsByKind(kindset ks) const
{
    qsettings.beginGroup("/" + sSettingKind[static_cast<int>(ks)]);
    for(const auto& s: vec_settings){
        if (s->kind == ks)
            qsettings.setValue("/" + s->title, s->value);
    }
    qsettings.endGroup();
}



//----------------------------------------------------------------------------------------------------------------------
void Settings::loadSettingsScreen()
{
    if(owner != nullptr)
    {
        if(loadSettingsByKind(kindset::screen))
        {
            owner->restoreGeometry(getSetting("geometry").toByteArray());
            owner->restoreState(getSetting("state").toByteArray());
        }
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
const QVariant& Settings::getSetting(const QString& title) const
{
    QMap<QString, Setting*>::const_iterator mit = mapset.find(title);
    if(mit != mapset.end())
    {
        const auto s = *mit;
        if(s->value.isNull() || !s->value.isValid())
            return s->default_value;
        return  s->value;
    }
    return default_return;
}


//----------------------------------------------------------------------------------------------------------------------
bool Settings::isChanged(const QString &title) const
{
    QMap<QString, Setting*>::const_iterator mit = mapset.find(title);
    if(mit != mapset.end())
    {
        auto s = *mit;
        return s->isChanged;
    }
    return false;
}


//----------------------------------------------------------------------------------------------------------------------
void Settings::setSetting(const QString& title, QVariant value)
{
    QMap<QString, Setting*>::iterator mit = mapset.find(title);
    if(mit != mapset.end())
    {
        auto s = *mit;
        s->isChanged = (s->value != value);
        s->value = value;        
    }
}


//----------------------------------------------------------------------------------------------------------------------
const QString& Settings::getSettingsName(const kindset ks) const
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

    qsettings.beginGroup("/" + sSettingKind[static_cast<int>(ks)]);

    for(auto& k: qsettings.allKeys())
        if(k != "." && k != "")
            mapset_by_kind[k] = qsettings.value(k, "").toString();

    qsettings.endGroup();

    //значения по-умолчанию
    if(mapset_by_kind.empty())
        for(const auto& s: k_mapset)
            if(ks == s.second)
                mapset_by_kind[s.first->title] = s.first->default_value;
}


