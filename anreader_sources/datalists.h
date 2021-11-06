#ifndef DATALISTS_H
#define DATALISTS_H

#include <QSettings>
#include <QVariant>
#include <memory>

#include "common.h"

enum class datakind: int {aircrafts = 0, dropzones, canopies};

class DataLists
{
    mutable QSettings qsettings;
    map_DataList m_aircrafts;
    map_DataList m_dropzones;
    map_DataList m_canopies;
public:
    DataLists(const QString &organization, const QString &application):
        qsettings(organization, application) {}
    ~DataLists() {}

    void load();
    void save() const;
    void loadDataAircrafts();
    void saveDataAircrafts() const;
    void loadDataDropZones();
    void saveDataDropZones() const;
    void loadDataCanopies();
    void saveDataCanopies() const;


    map_DataList& aircrafts() { return m_aircrafts; }
    map_DataList& dropzones() { return m_dropzones; }
    map_DataList& canopies() { return m_canopies; }

    const map_DataList& const_aircrafts() const { return m_aircrafts; }
    const map_DataList& const_dropzones() const { return m_dropzones; }
    const map_DataList& const_canopies() const { return m_canopies; }

private:
    void loadDataByKind(const datakind dk);
    void saveDataByKind(const datakind dk) const;
};

#endif // DATALISTS_H
