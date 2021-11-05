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
    map_APs m_aircrafts;
    map_DZs m_dropzones;
public:
    DataLists(const QString &organization, const QString &application):
        qsettings(organization, application) {}
    ~DataLists() {}

    void load();
    void save() const;
    void loadDataAircrafts();
    void saveDataAircrafts() const;

    map_APs& aircrafts() { return m_aircrafts; }
    map_APs& dropzones() { return m_dropzones; }
};

#endif // DATALISTS_H
