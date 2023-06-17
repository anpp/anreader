#include "devicedescriptionlistmodel.h"

const static QString sHorzHeader[] = {QObject::tr("Device type"), QObject::tr("Description")};

//---------------------------------------------------------------------------------------------------------------
QVariant DeviceDescriptionListModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        dtype type;
        QString description;

        std::tie(type, description) = m_datalist[index.row()];

        if(role == Qt::DisplayRole)
        {
            switch(static_cast<DeviceDescriptionListModel_defs>((index.column())))
            {
            case DeviceDescriptionListModel_defs::DeviceType: return sDeviceTypes[static_cast<uint>(type)];
            case DeviceDescriptionListModel_defs::DeviceDescription: return description;
            default: break;
            }
        }
        if(role == Qt::EditRole)
        {
            switch(static_cast<DeviceDescriptionListModel_defs>((index.column())))
            {
            case DeviceDescriptionListModel_defs::DeviceType: return static_cast<int>(type);
            case DeviceDescriptionListModel_defs::DeviceDescription: return description;
            default: break;
            }
        }
    }
    return QVariant();
}


//---------------------------------------------------------------------------------------------------------------
bool DeviceDescriptionListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
    {
        switch(static_cast<DeviceDescriptionListModel_defs>(index.column()))
        {
        case DeviceDescriptionListModel_defs::DeviceType:
            if(std::find_if(m_datalist.begin(), m_datalist.end(), [&value] (const auto& item) {return std::get<static_cast<int>(DeviceDescriptionListModel_defs::DeviceType)>(item) == static_cast<dtype>(value.toInt()); }) != m_datalist.end())
                return false;

            std::get<static_cast<int>(DeviceDescriptionListModel_defs::DeviceType)>(m_datalist[index.row()]) = static_cast<dtype>(value.toInt());
            break;
        case DeviceDescriptionListModel_defs::DeviceDescription:
            std::get<static_cast<int>(DeviceDescriptionListModel_defs::DeviceDescription)>(m_datalist[index.row()]) = value.toString().trimmed();
            break;
        default: break;
        }

        emit dataChanged(index, index);
        return true;
    }
    return false;
}


//---------------------------------------------------------------------------------------------------------------
QVariant DeviceDescriptionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return (section >= m_num_columns ? "" : tr(sHorzHeader[section].toStdString().c_str()));

    return QVariant();
}


//---------------------------------------------------------------------------------------------------------------
Qt::ItemFlags DeviceDescriptionListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}


//---------------------------------------------------------------------------------------------------------------
void DeviceDescriptionListModel::addItem(const t_device_description_item &item)
{
    this->beginInsertRows(QModelIndex(), static_cast<int>(m_datalist.size()), static_cast<int>(m_datalist.size()));
    m_datalist.push_back(item);
    this->endInsertRows();
}

//---------------------------------------------------------------------------------------------------------------
void DeviceDescriptionListModel::removeItem(const uint row)
{
    if(m_datalist.size() <= row)
        return;

    this->beginRemoveRows(QModelIndex(), row, row);
    m_datalist.erase(m_datalist.begin() + row);
    this->endRemoveRows();
}

