#include "datalistmodel.h"

const static QString sHorzHeader[] = {QObject::tr("Used"), QObject::tr("Key"), QObject::tr("Value")};

//---------------------------------------------------------------------------------------------------------------
QVariant DataListModel::data(const QModelIndex &index, int role) const
{

    if(index.isValid())
    {
        QString key, value;
        bool used;

        std::tie(used, key, value) = m_datalist[index.row()];

        if(role == Qt::DisplayRole && index.column() > 0)
        {
            switch(index.column())
            {
            case DataListModel_defs::Key: return key;
            case DataListModel_defs::Value: return value;
            default: break;
            }

        }
        if(role == Qt::CheckStateRole && index.isValid() && index.column() == DataListModel_defs::Used)
            return used ? Qt::Checked: Qt::Unchecked;
    }

/*
    if (role == Qt::BackgroundColorRole && index.isValid())
    {
        if (index.row() % 2 == 0){
            return color_0;
        }
        else {
            return color_1;
        }
    }
*/
    return QVariant();

}


//---------------------------------------------------------------------------------------------------------------
bool DataListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
        {
            emit dataChanged(index, index);
            return true;
        }
        return false;
}


//---------------------------------------------------------------------------------------------------------------
QVariant DataListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return (section >= num_columns ? "" : tr(sHorzHeader[section].toStdString().c_str()));

    return QVariant();
}


//---------------------------------------------------------------------------------------------------------------
Qt::ItemFlags DataListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags;
}

//---------------------------------------------------------------------------------------------------------------
void DataListModel::addItem(const t_datalist_item &item)
{
    this->beginInsertRows(QModelIndex(), m_datalist.size(), m_datalist.size());
    m_datalist.push_back(item);
    this->endInsertRows();
}

//---------------------------------------------------------------------------------------------------------------
void DataListModel::removeItem(const uint row)
{
    if(m_datalist.size() <= row)
        return;

    this->beginRemoveRows(QModelIndex(), row, row);
    m_datalist.erase(m_datalist.begin() + row);
    this->endRemoveRows();
}
