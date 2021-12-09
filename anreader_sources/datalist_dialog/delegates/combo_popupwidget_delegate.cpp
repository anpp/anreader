#include "combo_popupwidget_delegate.h"
#include "log_widget.h"
#include <QPushButton>
#include <QLineEdit>


//---------------------------------------------------------------------------------------------------------------------------
QWidget *ComboPopupWidgetDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    PopupComboBox *pcb = new PopupComboBox(parent);

    return pcb;
}


//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    PopupComboBox *pcb = static_cast<PopupComboBox*>(editor);
    pcb->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
}


//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    PopupComboBox *pcb = static_cast<PopupComboBox*>(editor);
}

//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}



PopupComboBox::PopupComboBox(QWidget *parent) : QComboBox(parent), sg(this)
{
    QListWidgetItem * item = new QListWidgetItem(&m_view);

    m_view.setItemWidget(item, &m_widget);
    item->setSizeHint(QSize(0, 200));

    QHBoxLayout *l = new QHBoxLayout(&m_widget);
    l->setMargin(0);

    this->setModel(m_view.model());
    this->setView(&m_view);
    this->view()->setCornerWidget(&sg);
    this->setMaxVisibleItems(1);

}
