#include "jumpstablemodel.h"
#include <QColor>

//----------------------------------------------------------------------------------------------------------------------
QModelIndex JumpsTableModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}

//----------------------------------------------------------------------------------------------------------------------
QModelIndex JumpsTableModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}


//----------------------------------------------------------------------------------------------------------------------
QVariant JumpsTableModel::data(const QModelIndex &index, int role) const
{
    if(!m_rows) return QVariant();

    QString ColumnName = "";
    if(index.isValid())
        ColumnName = m_rows->at(0).at(index.column()).first;

    if(role == Qt::DisplayRole && index.isValid() and ColumnName != "Deleted")
         return m_rows->at(index.row()).at(index.column()).second;

    if (role == Qt::BackgroundColorRole && index.isValid())
    {
        if (index.row() % 2 == 0){
            return color_0;
        }
        else {
            return color_1;
        }
    }

    if(role == Qt::CheckStateRole  && index.isValid() and ColumnName == "Deleted")
        return m_rows->at(index.row()).at(index.column()).second.toBool() ? Qt::Checked: Qt::Unchecked;

    return QVariant();
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_rows)
        return false;
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
        {
            emit dataChanged(index, index);
            return true;
        }
        return false;
}

//----------------------------------------------------------------------------------------------------------------------
QVariant JumpsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!m_rows) return QVariant();

    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal && m_rows->size() > 0)
        return m_rows->at(0).at(section).first;

    return QVariant();
}

//----------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags JumpsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags;
}

//----------------------------------------------------------------------------------------------------------------------
int JumpsTableModel::rowCount(const QModelIndex&) const
{
    return (m_rows ? m_rows->size() : 0);
}

//----------------------------------------------------------------------------------------------------------------------
int JumpsTableModel::columnCount(const QModelIndex&) const
{
    return num_cols;
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::setColumnCount(const int value)
{
    this->beginInsertColumns(QModelIndex(), num_cols, num_cols + value - 1);
    this->insertColumns(num_cols, value);
    this->endInsertColumns();
    num_cols += value;
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::checkColumns(const int value)
{
    if(num_cols == 0)
        setColumnCount(value);
    if(num_cols != value)
    {
        qDebug() << "Error columns number!";
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::takeLastJump(int &value)
{
    value = 0;
    if(!m_rows) return;
    if(m_rows->size())
        value = m_rows->at(m_rows->count() - 1).at(0).second.toInt();
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::addItem(const t_jump_attribute& jump)
{
    if(!checkColumns(jump.size()))
        return false;

    this->beginInsertRows(QModelIndex(), m_rows->size(), m_rows->size());

    m_rows->push_back(jump);

    this->endInsertRows();   
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::addItems(const t_rows& jumps)
{
    if(!jumps.size()) return false;
    if(!checkColumns(jumps.at(0).size()))
        return false;

    this->beginInsertRows(QModelIndex(), m_rows->size(), m_rows->size() + jumps.size() - 1);

    for(const auto& jump: jumps)
        m_rows->push_back(jump);

    this->endInsertRows();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::moveItems(std::unique_ptr<t_rows> &jumps)
{    
    if(!jumps) return false;
    if(!jumps->size()) return false;

    if(!checkColumns(jumps->at(0).size()))
        return false;

    if(m_rows->size() > 0)
        clear();

    this->beginInsertRows(QModelIndex(), 0, jumps->size() - 1);

    m_rows = std::move(jumps);

    this->endInsertRows();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::removeItem(const int row)
{
   if(!m_rows) return;
   if (m_rows->size() < row)
           return;

   this->beginRemoveRows(QModelIndex(), row, row);
   m_rows->erase(m_rows->begin() + row);
   this->endRemoveRows();
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::clear()
{
    if (m_rows->size() == 0)
            return;

    this->beginRemoveRows(QModelIndex(), 0, m_rows->size() - 1);
    m_rows->clear();
    this->endRemoveRows();

    if(num_cols)
    {
        this->beginRemoveColumns(QModelIndex(), 0, num_cols - 1);
        this->removeColumns(0, num_cols - 1);
        num_cols = 0;
        this->endRemoveColumns();
    }
}
