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
        if(Qt::EditRole == role || Qt::DisplayRole == role ||
            N3AlarmsSettings_defs::StepRole == role || N3AlarmsSettings_defs::MinAltRole == role ||
            N3AlarmsSettings_defs::MaxAltRole == role ||
            N3AlarmsSettings_defs::NamesRole == role)
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

    if(Qt::EditRole == role)
    {
        uint16_t altitude;
        QString name;

        switch(static_cast<N3AlarmsSettings_defs>(index.column()))
        {
        case N3AlarmsSettings_defs::Active:
            if(value.toBool())
            {
                m_data.type(index.row()) == N3AlarmsSettings::alarm_type::FreeFall ? m_data.setActiveFreeFallIndex(index.row()) : m_data.setActiveCanopyIndex(index.row());
                m_names.setActives();
                emit dataChanged(QModelIndex(), QModelIndex());
                return true;
            }
            break;
        case N3AlarmsSettings_defs::NameIndex:
            name = value.toString();
            if(!name.isEmpty())
            {
                m_data.setNameIndex(index.row(), name.left(name.indexOf(" -")).toUShort() - 1);
                m_names.setSelected();
                m_names.setActives();
                emit dataChanged(QModelIndex(), QModelIndex());
            }
            break;
        case N3AlarmsSettings_defs::AlarmAltitude1:
        case N3AlarmsSettings_defs::AlarmAltitude2:
        case N3AlarmsSettings_defs::AlarmAltitude3:
            altitude = (value.toUInt() / m_data.step(index.row())) * m_data.step(index.row());
            m_data.setAltitude(index.row(), index.column() - 2, altitude);
            emit dataChanged(QModelIndex(), QModelIndex());
            return true;
        default:
            break;
        }
    }
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

    if(index.isValid() && N3AlarmsSettings_defs::Active == index.column())
    {
        if(m_names.hidden(m_data.nameIndex(index.row())))
            flags &= ~Qt::ItemIsEditable;
    }

    return flags;
}

//------------------------------------------------------------------------------------------
QVariant N3AlarmsSettingsModel::value(int row, int col, int role) const
{
    if(static_cast<uint>(row) >= 8)
        return QVariant();

    QString index_name = "";
    if(Qt::EditRole == role || Qt::DisplayRole == role)
        switch(col)
        {
        case N3AlarmsSettings_defs::Active:
            return m_data.active(row);
        case N3AlarmsSettings_defs::NameIndex:
            if(Qt::EditRole == role)
                index_name = QString::number(m_data.nameIndex(row) + 1) + " - ";
            return (static_cast<uint>(m_data.nameIndex(row)) < m_names.Names().size() ? index_name + *m_names.Names()[m_data.nameIndex(row)] : "");
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

    if(N3AlarmsSettings_defs::StepRole == role)
        switch(col)
        {
        case N3AlarmsSettings_defs::AlarmAltitude1:
        case N3AlarmsSettings_defs::AlarmAltitude2:
        case N3AlarmsSettings_defs::AlarmAltitude3:
            return m_data.step(row);
        default:
            return QVariant();
        }

    if(N3AlarmsSettings_defs::MinAltRole == role)
        switch(col)
        {
        case N3AlarmsSettings_defs::AlarmAltitude1:
            return m_data.min_altitude(row, 0);
        case N3AlarmsSettings_defs::AlarmAltitude2:
            return m_data.min_altitude(row, 1);
        case N3AlarmsSettings_defs::AlarmAltitude3:
            return m_data.min_altitude(row, 2);
        default:
            return QVariant();
        }

    if(N3AlarmsSettings_defs::MaxAltRole == role)
        switch(col)
        {
        case N3AlarmsSettings_defs::AlarmAltitude1:
            return m_data.max_altitude(row, 0);
        case N3AlarmsSettings_defs::AlarmAltitude2:
            return m_data.max_altitude(row, 1);
        case N3AlarmsSettings_defs::AlarmAltitude3:
            return m_data.max_altitude(row, 2);
        default:
            return QVariant();
        }
    if(N3AlarmsSettings_defs::NamesRole == role && N3AlarmsSettings_defs::NameIndex == col)
    {
        QStringList names_list;
        int i = 0;
        for(const auto& name: m_names.Names())
        {
            if((!m_names.selected(i) && !m_names.hidden(i)) || m_data.nameIndex(row) == i)
                names_list << QString::number(i + 1) + " - " + *name;
            i++;
        }
        return names_list;
    }
    return QVariant();
}




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
