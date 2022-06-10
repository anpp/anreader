#include "comboeditdelegate.h"

#include <QComboBox>

//---------------------------------------------------------------------------------------------------------------------------
QWidget *ComboEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *cb = new QComboBox(parent);

    for(const auto& item: sDeviceTypes)
        cb->addItem(item);

    return cb;
}

//---------------------------------------------------------------------------------------------------------------------------
void ComboEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    QComboBox *cb = static_cast<QComboBox*>(editor);
    cb->setCurrentIndex(index.model()->data(index, Qt::EditRole).toInt());
}

//---------------------------------------------------------------------------------------------------------------------------
void ComboEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = static_cast<QComboBox*>(editor);
    model->setData(index, cb->currentIndex());
}

//---------------------------------------------------------------------------------------------------------------------------
void ComboEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
