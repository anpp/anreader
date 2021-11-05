#include "settings.h"
#include <QDebug>
#include <QDir>

static QString sSettingKind[] = {"appearance", "misc", "screen", "environment"};


//----------------------------------------------------------------------------------------------------------------------
Settings::Settings(QMainWindow* widget_owner, const QString& organization, const QString& application) :
    owner(widget_owner), qsettings(organization, application), default_return(false)
{
    vec_settings = {
                    std::make_shared<Setting>("geometry", kindset::screen, 0, QVariant(QVariant::Int), false),
                    std::make_shared<Setting>("state", kindset::screen, 0, QVariant(QVariant::Int), false),
                    std::make_shared<Setting>("directory_for_save", kindset::environment, QDir::homePath(), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("current_file", kindset::environment, QString(), QVariant(QVariant::String), false),
                    std::make_shared<Setting>("current_file_delimiter", kindset::environment, QString(";"), QVariant(QVariant::String), false)
                   };

    foreach(auto& s, vec_settings) {mapset[s.get()->title] = s.get();}
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
    foreach(auto& s, vec_settings){
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
void Settings::saveSettingsByKind(kindset ks)
{
    qsettings.beginGroup("/" + sSettingKind[static_cast<int>(ks)]);
    foreach(auto& s, vec_settings){
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
        auto s = *mit;
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
    QMap<QString, Setting*>::const_iterator mit = mapset.find(title);
    if(mit != mapset.end())
    {
        auto s = *mit;
        s->isChanged = (s->value != value);
        s->value = value;        
    }
}


//----------------------------------------------------------------------------------------------------------------------
const QString& Settings::getSettingsName(kindset ks) const
{
    return sSettingKind[static_cast<int>(ks)];
}
