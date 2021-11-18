#include "datalists.h"

const static QString sDataKind[] = {"aircrafts", "dropzones", "canopies"};


//---------------------------------------------------------------------------------------------------
DataLists::DataLists(const QString &organization, const QString &application): qsettings(organization, application)
{
    m_aircrafts[""] = "";
    m_dropzones[""] = "";
    m_canopies[""] = "";
}

void DataLists::load()
{
    loadDataAircrafts();
    loadDataDropZones();
    loadDataCanopies();
}


//---------------------------------------------------------------------------------------------------
void DataLists::save() const
{
    saveDataAircrafts();
    saveDataDropZones();
    saveDataCanopies();
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
const QString &DataLists::mappedValue(const datakind dk, const QString &key) const
{
    const map_DataList& mdl = (datakind::aircrafts == dk ? const_aircrafts() : (datakind::dropzones == dk ? const_dropzones() : const_canopies()));
    const auto& it = mdl.find(key.trimmed());
    if(it != mdl.end())
        return it->second;
    else
        return empty_string;
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataByKind(const datakind dk)
{
    map_DataList *ptr_mdl = nullptr;

    ptr_mdl = (datakind::dropzones == dk)? &m_dropzones : ((datakind::canopies == dk)? &m_canopies : &m_aircrafts);
    if(nullptr == ptr_mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);
    for(auto& k: qsettings.allKeys())
        if(k != ".")
            (*ptr_mdl)[k] = qsettings.value(k, "").toString();
    qsettings.endGroup();
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataByKind(const datakind dk) const
{
    const map_DataList *ptr_mdl = nullptr;

    ptr_mdl = (datakind::dropzones == dk)? &m_dropzones : ((datakind::canopies == dk)? &m_canopies : &m_aircrafts);
    if(nullptr == ptr_mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);

    //--удаление
    QStringList sl;
    for(auto& k: qsettings.allKeys())
        if(ptr_mdl->find(k) == ptr_mdl->end())
            sl << k;
    for(auto& key: sl)
        qsettings.remove(key);
    //--

    for(auto it = (*ptr_mdl).begin(); it != (*ptr_mdl).end(); ++it)
        qsettings.setValue("/" + (*it).first, (*it).second);

    qsettings.endGroup();
}
