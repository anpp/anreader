#include "combo_popupwidget_delegate.h"
#include "log_widget.h"


//---------------------------------------------------------------------------------------------------------------------------
QWidget *ComboPopupWidgetDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QString data;
    if(index.isValid())
        data = index.model()->data(index, Qt::EditRole).toString();
    DDComboBox *pcb = new DDComboBox(parent, data);

    return pcb;
}


//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    DDComboBox *pcb = static_cast<DDComboBox*>(editor);
    pcb->setEditText(index.model()->data(index, Qt::EditRole).toString());
}


//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    DDComboBox *pcb = static_cast<DDComboBox*>(editor);
    model->setData(index, pcb->currentText());
}

//---------------------------------------------------------------------------------------------------------------------------
void ComboPopupWidgetDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}




//===========================================================================================================
DDComboBox::DDComboBox(QWidget *parent, const QString& strings) : QComboBox(parent), m_widget(strings), sg(this)
{
    m_current_text = strings;
    QListWidgetItem * item = new QListWidgetItem(&m_view);

    m_view.setItemWidget(item, &m_widget);
    //m_widget.installEventFilter(this);
    item->setSizeHint(QSize(m_widget.width(), m_widget.height()));

    this->setModel(m_view.model());
    this->setView(&m_view);
    this->view()->setCornerWidget(&sg);
    this->setMaxVisibleItems(1);
    this->setEditable(true);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    m_view.setSelectionMode( QAbstractItemView::NoSelection );
    //m_view.setStyleSheet("QListWidget{selection-background-color: transparent; background-color: transparent}");

    connect(&m_widget, &StringListPopup::finish, this, &DDComboBox::closePopup);
}

//---------------------------------------------------------------------------------------------------------------------------
QString DDComboBox::currentText() const
{
    return m_current_text;
}


//---------------------------------------------------------------------------------------------------------------------------
void DDComboBox::showPopup()
{
    m_widget.setFixedWidth(this->width());
    QComboBox::showPopup();
}

//---------------------------------------------------------------------------------------------------------------------------
void DDComboBox::hidePopup()
{
    m_current_text = (m_accepted ? m_widget.value() : m_current_text);
    setEditText(m_current_text);
    QComboBox::hidePopup();
}

//---------------------------------------------------------------------------------------------------------------------------
void DDComboBox::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
    return;
}

//---------------------------------------------------------------------------------------------------------------------------
bool DDComboBox::eventFilter(QObject *widget, QEvent *event)
{
    if (&m_widget == widget && event->type() == QEvent::Paint)
    {
        m_widget.setBackgroundRole(QPalette::ColorRole::Dark);
    }

    return QWidget::eventFilter(widget, event);
}

//---------------------------------------------------------------------------------------------------------------------------
void DDComboBox::closePopup(bool accepted)
{
    m_accepted = accepted;
    hidePopup();
}
