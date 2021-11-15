#include "lineeditdelegate.h"

//---------------------------------------------------------------------------------------------------------------------------
QWidget *LineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *le = new QLineEdit(parent);
    return le;
}

//---------------------------------------------------------------------------------------------------------------------------
void LineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    QString value = index.model()->data(index).toString();
    QLineEdit *le = static_cast<QLineEdit*>(editor);
    le->setText(value);
}

//---------------------------------------------------------------------------------------------------------------------------
void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *le = static_cast<QLineEdit*>(editor);
    model->setData(index, le->text());
}


//---------------------------------------------------------------------------------------------------------------------------
void LineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

