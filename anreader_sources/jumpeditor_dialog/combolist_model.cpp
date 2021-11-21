#include "combolist_model.h"

//-----------------------------------------------------------------------------------------------
Combolist_model::Combolist_model(const datakind dk, const DataLists& ref_dl, QObject *parent):
    QAbstractItemModel(parent), m_num_columns(2), m_dk(dk), m_dl(ref_dl)
{
    const map_DataList& datalist = m_dl.datalist_by_kind(m_dk);

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
            return m_dl.mappedValue(m_dk, m_data[index.row()].first); //m_data[index.row()].second;
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


//-----------------------------------------------------------------------------------------------
int Combolist_model::indexByKey(const QString &key) const
{
    int index = 0;
    bool found = false;

    for(const auto& item: m_data)
    {
        if(item.first == key)
        {
            found = true;
            break;
        }
        index++;
    }
    return found? index : 0;
}


//-----------------------------------------------------------------------------------------------
const QString &Combolist_model::keyByIndex(int index) const
{
    if(index < 0 || static_cast<size_t>(index) >= m_data.size())
        index = 0;
    return m_data[index].first;
}
