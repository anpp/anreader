#include "n3names_model.h"
#include "device/n3names.h"
#include <QFont>

//------------------------------------------------------------------------------------------
QVariant N3NamesModel::data(const QModelIndex &index, int role) const
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
QVariant N3NamesModel::value(int row, int col , int role) const
{
    if(static_cast<uint>(row) >= m_data.filled())
        return QVariant();

    if(Qt::EditRole == role)
        switch(static_cast<N3NamesModel_defs>(col))
        {
        case N3NamesModel_defs::Active:
            return m_data.active(row);
        case N3NamesModel_defs::Used:
            return m_data.used(row);
        case N3NamesModel_defs::Hidden:
            return m_data.hidden(row);
        case N3NamesModel_defs::Name:
            return (static_cast<uint>(row) < m_data.Names().size() ? *m_data.Names()[row] : "");
        default:
            return QVariant();
        }
    if(Qt::DisplayRole == role && static_cast<int>(N3NamesModel_defs::Name) == col)
        return (static_cast<uint>(row) < m_data.Names().size() ? *m_data.Names()[row] : "");
    return QVariant();
}

//------------------------------------------------------------------------------------------
bool N3NamesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    QString name = "";
    if(Qt::EditRole == role)
        switch(static_cast<N3NamesModel_defs>(index.column()))
        {
        case N3NamesModel_defs::Active:
            if(value.toBool())
            {
                m_data.setActive(index.row());
                emit dataChanged(QModelIndex(), QModelIndex());
                return true;
            }
            break;

        case N3NamesModel_defs::Hidden:
            m_data.setHidden(index.row(), value.toBool());
            emit dataChanged(QModelIndex(), QModelIndex());
            return true;

        case N3NamesModel_defs::Name:
            name = value.toString();
            if(name.isEmpty())
                name = "---";
            m_data.setName(index.row(), name);
            emit dataChanged(QModelIndex(), QModelIndex());
            return true;

        default:
            break;
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
        if(index.column() == static_cast<int>(N3NamesModel_defs::Active)
                && ((!m_data.hidden(index.row()) && m_data.type() != N3NamesType::Alarms) || index.row() == static_cast<int>(m_data.filled())))
            flags |= Qt::ItemIsEditable;

        if(index.column() == static_cast<int>(N3NamesModel_defs::Hidden)
                && !m_data.used(index.row())
                && !m_data.active(index.row()))
            flags |= Qt::ItemIsEditable;

        if(index.column() == static_cast<int>(N3NamesModel_defs::Name)
                && !m_data.used(index.row()))
            flags |= Qt::ItemIsEditable;

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

//------------------------------------------------------------------------------------------
bool N3NamesModel::used(uint index) const
{
    return m_data.used(index);
}

//------------------------------------------------------------------------------------------
bool N3NamesModel::hidden(uint index) const
{
    return m_data.hidden(index);
}

//------------------------------------------------------------------------------------------
bool N3NamesModel::active(uint index) const
{
    return m_data.active(index);
}

//------------------------------------------------------------------------------------------
void N3NamesModel::add()
{
    if(m_data.filled() < m_data.count())
    {
        m_data.setFilled(m_data.filled() + 1);

        emit dataChanged(createIndex(m_data.filled() - 1, static_cast<int>(N3NamesModel_defs::Active)),
                         createIndex(m_data.filled(), static_cast<int>(N3NamesModel_defs::Name)));
    }
}

//------------------------------------------------------------------------------------------
void N3NamesModel::del()
{
    if(m_data.filled() <= m_data.count() && m_data.filled() > 0)
    {
        m_data.setName(m_data.filled() - 1, "");
        m_data.setHidden(m_data.filled() - 1, false);
        m_data.setFilled(m_data.filled() - 1);

        emit dataChanged(createIndex(m_data.filled() - 1, static_cast<int>(N3NamesModel_defs::Active)),
                         createIndex(m_data.filled(), static_cast<int>(N3NamesModel_defs::Name)));
    }
}

