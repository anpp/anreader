#include "n3names_model.h"
#include "device/n3names.h"

//------------------------------------------------------------------------------------------
QVariant N3NamesModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(static_cast<uint>(index.row()) >= m_data.filled())
            return QVariant();

        if(Qt::DisplayRole == role && static_cast<N3NamesModel_defs>(index.column()) == N3NamesModel_defs::Name)
            return (static_cast<uint>(index.row()) < m_data.Names().size() ? *m_data.Names()[index.row()] : "");

        if(Qt::CheckStateRole == role && static_cast<N3NamesModel_defs>(index.column()) != N3NamesModel_defs::Name)

            switch(static_cast<N3NamesModel_defs>(index.column()))
            {
            case N3NamesModel_defs::Active:
                return m_data.active(index.row()) ? Qt::Checked : Qt::Unchecked;
            case N3NamesModel_defs::Used:
                return m_data.used(index.row()) ? Qt::Checked : Qt::Unchecked;
            case N3NamesModel_defs::Hidden:
                return m_data.hidden(index.row()) ? Qt::Checked : Qt::Unchecked;
            default:
                return QVariant();
            }
    }
    return QVariant();
}

//------------------------------------------------------------------------------------------
bool N3NamesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

//------------------------------------------------------------------------------------------
QVariant N3NamesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        switch(static_cast<N3NamesModel_defs>(section))
        {
        case N3NamesModel_defs::Active:
            return tr("Active");
        case N3NamesModel_defs::Used:
            return tr("Used");
        case N3NamesModel_defs::Hidden:
            return tr("Hidden");
        case N3NamesModel_defs::Name:
            return tr("Name");
        default:
            return QVariant();
        }
    return QVariant();
}

//------------------------------------------------------------------------------------------
Qt::ItemFlags N3NamesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags &= ~Qt::ItemIsEditable;
    if(index.isValid())
    {
    }

    return flags;
}

//------------------------------------------------------------------------------------------
int N3NamesModel::rowCount(const QModelIndex &) const
{
    return m_data.count();
}
