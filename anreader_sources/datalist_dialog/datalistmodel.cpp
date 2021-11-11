#include "datalistmodel.h"


//---------------------------------------------------------------------------------------------------------------
QVariant DataListModel::data(const QModelIndex &index, int role) const
{

    if(role == Qt::DisplayRole && index.isValid())
    {
        switch(index.column())
        {
        case 1:
            return std::get<1>(m_datalist[index.row()]);
            break;
        case 2:
            return std::get<2>(m_datalist[index.row()]);
            break;
        default:
            break;
        }
    }
    if(role == Qt::CheckStateRole && index.isValid() && index.row() == 0)
        return std::get<0>(m_datalist[index.row()]) ? Qt::Checked: Qt::Unchecked;

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
        return (section == 0? tr("Used") : (section == 1? tr("Key") : tr("Value")));

    return QVariant();
}


//---------------------------------------------------------------------------------------------------------------
Qt::ItemFlags DataListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags;
}
