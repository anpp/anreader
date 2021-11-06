#include "datalists.h"

static QString sDataKind[] = {"aircrafts", "dropzones", "canopies"};


//---------------------------------------------------------------------------------------------------
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
void DataLists::loadDataByKind(const datakind dk)
{
    map_DataList *ptr_mdl = nullptr;

    ptr_mdl = (datakind::dropzones == dk)? &m_dropzones : ((datakind::canopies == dk)? &m_canopies : &m_aircrafts);
    if(nullptr == ptr_mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);
    foreach(auto& k, qsettings.allKeys())
        if(k != ".")
            (*ptr_mdl)[k] = k;
    qsettings.endGroup();
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataByKind(const datakind dk) const
{
    const map_DataList *ptr_mdl = nullptr;

    ptr_mdl = (datakind::dropzones == dk)? &m_dropzones : ((datakind::canopies == dk)? &m_canopies : &m_aircrafts);
    if(nullptr == ptr_mdl) return;

    qsettings.beginGroup("/" + sDataKind[static_cast<int>(dk)]);
    for(auto it = (*ptr_mdl).begin(); it != (*ptr_mdl).end(); ++it)
        qsettings.setValue("/" + (*it).first, (*it).second);
    qsettings.endGroup();
}
