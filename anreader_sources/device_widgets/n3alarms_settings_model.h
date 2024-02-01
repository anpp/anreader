
#ifndef N3ALARMSSETTINGSMODEL_H
#define N3ALARMSSETTINGSMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class N3Names;
class N3AlarmsNames;
class N3AlarmsSettings;
class N3FilterAlarmsSettingsModel;

enum N3AlarmsSettings_defs: int {NumColumns = 5, Active = 0, NameIndex = 1, AlarmAltitude1 = 2,  AlarmAltitude2 = 3, AlarmAltitude3 = 4};

class N3AlarmsSettingsModel : public QAbstractItemModel
{    
    N3AlarmsSettings& m_data;
    const N3Names& m_names;
public:
    explicit N3AlarmsSettingsModel(N3AlarmsNames& alarmsnames, QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override { return 8; };
    int columnCount(const QModelIndex&) const override {return static_cast<int>(N3AlarmsSettings_defs::NumColumns); }
    QVariant value(int row, int col, int role = Qt::DisplayRole) const;

protected:
    friend class N3FilterAlarmsSettingsModel;

};

//===============================================================================================================
class N3FilterAlarmsSettingsModel : public QSortFilterProxyModel
{
    uint8_t m_alarm_type = 2;
public:
    N3FilterAlarmsSettingsModel(N3AlarmsSettingsModel* source_model, uint8_t alarm_type, QObject *parent = 0) :
        QSortFilterProxyModel(parent), m_alarm_type(alarm_type) { setSourceModel(source_model); };
    ~N3FilterAlarmsSettingsModel() = default;

    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

};


#endif // N3ALARMSSETTINGSMODEL_H
