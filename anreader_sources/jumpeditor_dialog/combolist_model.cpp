#include "combolist_model.h"

//-----------------------------------------------------------------------------------------------
Combolist_model::Combolist_model(const map_DataList &datalist, QObject *parent):
    QAbstractItemModel(parent), m_num_columns(2)
{
    for(auto it = datalist.begin(); it != datalist.end(); ++it)
        m_data.push_back(std::make_pair((*it).first, (*it).second));
}


//-----------------------------------------------------------------------------------------------
QVariant Combolist_model::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && Qt::DisplayRole == role) //&& index.row() < static_cast<int>(m_data.size()))
    {
        switch(index.column())
        {
        case 0:
            return m_data[index.row()].first;
        case 1:
            return m_data[index.row()].second;
        default:
            break;
        }
    }
    return QVariant();
}


//-----------------------------------------------------------------------------------------------
Qt::ItemFlags Combolist_model::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags &= ~Qt::ItemIsEditable;
    return flags;
}
