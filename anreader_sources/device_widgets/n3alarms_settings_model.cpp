#include "n3alarms_settings_model.h"
#include "device/n3alarms_names.h"
#include "device/n3alarms_settings.h"

#include <QFont>

//------------------------------------------------------------------------------------------
N3AlarmsSettingsModel::N3AlarmsSettingsModel(N3AlarmsNames& alarmsnames, QObject *parent)
    : QAbstractItemModel{parent}, m_data(alarmsnames.settings()), m_names(alarmsnames)
{

}

//------------------------------------------------------------------------------------------
QVariant N3AlarmsSettingsModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(Qt::EditRole == role || Qt::DisplayRole == role)
            return value(index.row(), index.column(), role);

        if(Qt::FontRole == role && m_data.active(index.row()))
        {
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    return QVariant();
}

//------------------------------------------------------------------------------------------
bool N3AlarmsSettingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    return false;
}

//------------------------------------------------------------------------------------------
QVariant N3AlarmsSettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        switch(section)
        {
        case N3AlarmsSettings_defs::Active:
            return tr("Active");
        case N3AlarmsSettings_defs::NameIndex:
            return tr("Name");
        case N3AlarmsSettings_defs::AlarmAltitude1:
            return tr("Altitude 1");
        case N3AlarmsSettings_defs::AlarmAltitude2:
            return tr("Altitude 2");
        case N3AlarmsSettings_defs::AlarmAltitude3:
            return tr("Altitude 3");
        default:
            return QVariant();
        }
    return QVariant();
}

//------------------------------------------------------------------------------------------
Qt::ItemFlags N3AlarmsSettingsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags |= Qt::ItemIsEditable;

    return flags;
}

//------------------------------------------------------------------------------------------
QVariant N3AlarmsSettingsModel::value(int row, int col, int role) const
{
    if(static_cast<uint>(row) >= 8)
        return QVariant();

    if(Qt::EditRole == role || Qt::DisplayRole == role)
        switch(col)
        {
        case N3AlarmsSettings_defs::Active:
            return m_data.active(row);
        case N3AlarmsSettings_defs::NameIndex:
            return (static_cast<uint>(m_data.nameIndex(row)) < m_names.Names().size() ? *m_names.Names()[m_data.nameIndex(row)] : "");
            break;
        case N3AlarmsSettings_defs::AlarmAltitude1:
            return m_data.altitude(row, 0);
        case N3AlarmsSettings_defs::AlarmAltitude2:
            return m_data.altitude(row, 1);
        case N3AlarmsSettings_defs::AlarmAltitude3:

            return m_data.altitude(row, 2);
        default:
            return QVariant();
        }

    return QVariant();
}


/*
//------------------------------------------------------------------------------------------
QVariant N3FilterAlarmsSettingsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    if (this->sourceModel())
        return this->sourceModel()->data(this->mapToSource(index), role);
    return false;
}


//------------------------------------------------------------------------------------------
bool N3FilterAlarmsSettingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()) return false;

    if (this->sourceModel())
        return this->sourceModel()->setData(this->mapToSource(index), value, role);
    return false;
}
*/

//------------------------------------------------------------------------------------------
QVariant N3FilterAlarmsSettingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;
    return this->sourceModel()->headerData(section, orientation, role);
}

//------------------------------------------------------------------------------------------
bool N3FilterAlarmsSettingsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    N3AlarmsSettingsModel* srcModel = static_cast<N3AlarmsSettingsModel*>(this->sourceModel());
    if (srcModel)
        return (srcModel->m_data.type(sourceRow) == static_cast<N3AlarmsSettings::alarm_type>(m_alarm_type));
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
