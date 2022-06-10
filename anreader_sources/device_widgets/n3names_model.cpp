#include "n3names_model.h"
#include "device/n3names.h"

//------------------------------------------------------------------------------------------
QVariant N3NamesModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        if(static_cast<uint>(index.row()) >= m_data.filled())
            return QVariant();

        if(Qt::EditRole == role)
            switch(static_cast<N3NamesModel_defs>(index.column()))
            {
            case N3NamesModel_defs::Active:
                return m_data.active(index.row());
            case N3NamesModel_defs::Used:
                return m_data.used(index.row());
            case N3NamesModel_defs::Hidden:
                return m_data.hidden(index.row());
            case N3NamesModel_defs::Name:
                return (static_cast<uint>(index.row()) < m_data.Names().size() ? *m_data.Names()[index.row()] : "");
            default:
                return QVariant();
            }
        if(Qt::DisplayRole == role && static_cast<int>(N3NamesModel_defs::Name) == index.column())
            return (static_cast<uint>(index.row()) < m_data.Names().size() ? *m_data.Names()[index.row()] : "");
    }
    return QVariant();
}

//------------------------------------------------------------------------------------------
bool N3NamesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(index.column() == static_cast<int>(N3NamesModel_defs::Active) && Qt::EditRole == role)
    {
        if(value.toBool())
        {
            for(uint i = 0; i < m_data.filled(); ++i)
                if(i != static_cast<uint>(index.row()))
                    m_data.setActive(i, false);
                else
                    m_data.setActive(i);

            emit dataChanged(QModelIndex(), QModelIndex());
            return true;
        }
    }
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
        if(index.column() == static_cast<int>(N3NamesModel_defs::Active))
            flags |= Qt::ItemIsEditable;

        //if(index.column() == static_cast<int>(N3NamesModel_defs::Hidden))
        //    flags |= Qt::ItemIsEditable;

    }

    return flags;
}

//------------------------------------------------------------------------------------------
int N3NamesModel::rowCount(const QModelIndex &) const
{
    return m_data.count();
}

//------------------------------------------------------------------------------------------
int N3NamesModel::filledCount() const
{
    return m_data.filled();
}
