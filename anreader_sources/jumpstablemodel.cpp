#include "jumpstablemodel.h"


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
    if(!m_rows || !index.isValid()) return QVariant();

    std::unique_ptr<t_jump_attribute> j_atr;

    j_atr = m_rows->at(index.row())->getPairs();

    QVariant row_value = (*j_atr).at(index.column()).second;
    size_t inner_index = N3Jump::index((*j_atr).at(index.column()).first);

    if(Qt::DisplayRole == role && CustomJumpNames::Deleted != inner_index)
    {
        if(CustomJumpNames::AC == inner_index || CustomJumpNames::Canopy == inner_index || CustomJumpNames::DZ == inner_index)
        {
            const datakind dk = (CustomJumpNames::AC == inner_index ? datakind::aircrafts : (CustomJumpNames::DZ == inner_index ? datakind::dropzones : datakind::canopies));
            const QString& map_value = ref_dl.mappedValue(dk, row_value.toString());
            return map_value;

        }
        else
            return row_value;
    }

    if(Qt::CheckStateRole == role && CustomJumpNames::Deleted == inner_index)
        return m_rows->at(index.row())->isDeleted() ? Qt::Checked: Qt::Unchecked;

    if (role == Qt::BackgroundColorRole)
    {
        if (index.row() % 2 == 0){
            return color_0;
        }
        else {
            return color_1;
        }
    }

    return QVariant();
}


//----------------------------------------------------------------------------------------------------------------------
QVariant JumpsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!m_rows) return QVariant();

    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    if(role == Qt::DisplayRole && orientation == Qt::Horizontal && m_rows->size() > 0)
        return (*m_rows->at(0)->getPairs()).at(section).first;

    return QVariant();
}

//----------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags JumpsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags &= ~Qt::ItemIsEditable;
    return flags;
}

//----------------------------------------------------------------------------------------------------------------------
int JumpsTableModel::rowCount(const QModelIndex&) const
{
    return (m_rows ? static_cast<int>(m_rows->size()) : 0);
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
        //qDebug() << "Error columns number!";
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
        value = (*m_rows->at(m_rows->size() - 1)).getJumpNumber();
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::addItem(const std::shared_ptr<CustomJump>& jump)
{
    std::unique_ptr<t_jump_attribute> j_atr = jump.get()->getPairs();
    if(!checkColumns(static_cast<int>((*j_atr).size())))
        return false;

    this->beginInsertRows(QModelIndex(), static_cast<int>(m_rows->size()), static_cast<int>(m_rows->size()));

    m_rows->push_back(jump);

    this->endInsertRows();   
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
bool JumpsTableModel::addItems(const t_rows& jumps)
{
    if(!jumps.size()) return false;

    std::unique_ptr<t_jump_attribute> j_atr = jumps.at(0)->getPairs();
    if(!checkColumns(static_cast<int>((*j_atr).size())))
        return false;

    this->beginInsertRows(QModelIndex(), static_cast<int>(m_rows->size()), static_cast<int>(m_rows->size() + jumps.size() - 1));

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

    std::unique_ptr<t_jump_attribute> j_atr = jumps->at(0)->getPairs();
    if(!checkColumns(static_cast<int>((*j_atr).size())))
        return false;

    if(m_rows->size() > 0)
        clear();

    this->beginInsertRows(QModelIndex(), 0, static_cast<int>(jumps->size() - 1));

    m_rows = std::move(jumps);

    this->endInsertRows();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::removeItem(const uint row)
{
   if(!m_rows) return;
   if (m_rows->size() <= row)
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

    this->beginRemoveRows(QModelIndex(), 0, static_cast<int>(m_rows->size() - 1));
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


//----------------------------------------------------------------------------------------------------------------------
std::shared_ptr<CustomJump> JumpsTableModel::getItem(const uint row) const
{
    std::shared_ptr<CustomJump> result;

    if (m_rows->size() > row)
        result = m_rows->at(row);

    return result;
}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTableModel::sort_jumps()
{
    std::sort(m_rows->begin(), m_rows->end(), [](const auto& j1, const auto& j2){ return j1->getJumpNumber() < j2->getJumpNumber();} );
}
