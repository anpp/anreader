#include "datalists.h"

const static QString sDataKind[] = {"aircrafts", "dropzones", "canopies", "jumptypes"};


//---------------------------------------------------------------------------------------------------
DataLists::DataLists(const QString &organization, const QString &application): qsettings(organization, application)
{
    m_aircrafts[""] = "";
    m_dropzones[""] = "";
    m_canopies[""] = "";
    m_jumptypes[""] = "";
}

void DataLists::load()
{
    loadDataAircrafts();
    loadDataDropZones();
    loadDataCanopies();
    loadDataJumpTypes();
}


//---------------------------------------------------------------------------------------------------
void DataLists::save() const
{
    saveDataAircrafts();
    saveDataDropZones();
    saveDataCanopies();
    saveDataJumpTypes();
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataAircrafts()
{
    loadDataByKind(datakind::aircrafts);
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataAircrafts() const
{
    saveDataByKind(datakind::aircrafts);
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataDropZones()
{
    loadDataByKind(datakind::dropzones);
}


//---------------------------------------------------------------------------------------------------
void DataLists::saveDataDropZones() const
{
    saveDataByKind(datakind::dropzones);
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataCanopies()
{
    loadDataByKind(datakind::canopies);
}


//---------------------------------------------------------------------------------------------------
void DataLists::saveDataCanopies() const
{
    saveDataByKind(datakind::canopies);
}

//---------------------------------------------------------------------------------------------------
void DataLists::loadDataJumpTypes()
{
    loadDataByKind(datakind::jump_types);
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataJumpTypes() const
{
    saveDataByKind(datakind::jump_types);
}


//---------------------------------------------------------------------------------------------------
map_DataList &DataLists::datalist_by_kind(const datakind dk)
{
    switch (static_cast<int>(dk)) {
    case (static_cast<int>(datakind::dropzones)): return dropzones();
    case (static_cast<int>(datakind::aircrafts)): return aircrafts();
    case (static_cast<int>(datakind::canopies)): return canopies();
    case (static_cast<int>(datakind::jump_types)): return jumptypes();
    default:
        break;
    }
    return m_empty;
}

//---------------------------------------------------------------------------------------------------

const map_DataList &DataLists::const_datalist_by_kind(const datakind dk) const
{
    switch (static_cast<int>(dk)) {
    case (static_cast<int>(datakind::dropzones)): return const_dropzones();
    case (static_cast<int>(datakind::aircrafts)): return const_aircrafts();
    case (static_cast<int>(datakind::canopies)): return const_canopies();
    case (static_cast<int>(datakind::jump_types)): return const_jumptypes();
    default:
        break;
    }
    return m_empty;
}



//---------------------------------------------------------------------------------------------------
const QString &DataLists::mappedValue(const datakind dk, const QString &key) const
{
    const map_DataList& mdl = const_datalist_by_kind(dk);
    const auto& it = mdl.find(key.trimmed());
    if(it != mdl.end())
        return (it->second.isEmpty() ? it->first : it->second);
    else
        return empty_string;
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataByKind(const datakind dk)
{
    map_DataList& mdl = datalist_by_kind(dk);
    if(m_empty == mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);
    for(auto& k: qsettings.allKeys())
        if(k != ".")
            mdl[k] = qsettings.value(k, "").toString();
    qsettings.endGroup();
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataByKind(const datakind dk) const
{
    const map_DataList& mdl = const_datalist_by_kind(dk);

    if(m_empty == mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);

    //--удаление
    QStringList sl;
    for(auto& k: qsettings.allKeys())
        if(mdl.find(k) == mdl.end())
            sl << k;
    for(auto& key: sl)
        qsettings.remove(key);
    //--

    for(auto it = mdl.begin(); it != mdl.end(); ++it)
        qsettings.setValue("/" + (*it).first, (*it).second);

    qsettings.endGroup();
}
