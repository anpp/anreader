#include "datalists.h"

static QString sDataKind[] = {"aircrafts", "dropzones", "canopies"};


//---------------------------------------------------------------------------------------------------
void DataLists::load()
{
    loadDataAircrafts();
}


//---------------------------------------------------------------------------------------------------
void DataLists::save() const
{
    saveDataAircrafts();
}


//---------------------------------------------------------------------------------------------------
void DataLists::loadDataAircrafts()
{
    qsettings.beginGroup("/" + sDataKind[static_cast<int>(datakind::aircrafts)]);
    foreach(auto& k, qsettings.allKeys())
        if(k != ".")
            m_aircrafts[k] = k;
    qsettings.endGroup();
}

//---------------------------------------------------------------------------------------------------
void DataLists::saveDataAircrafts() const
{
    qsettings.beginGroup("/" + sDataKind[static_cast<int>(datakind::aircrafts)]);
    for(auto it = m_aircrafts.begin(); it != m_aircrafts.end(); ++it)
        qsettings.setValue("/" + (*it).first, (*it).second);
    qsettings.endGroup();
}
