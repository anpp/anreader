#include "n3names_delegate.h"
#include "n3names_model.h"
#include "device/n3names.h"

#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QApplication>
#include <QMouseEvent>
#include <QToolButton>

const static QString button_add = QObject::tr("Add");

//------------------------------------------------------------------------------------------
QWidget *N3NamesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if(!index.isValid())
        return nullptr;

    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    if(index.row() < m_model->filledCount())
    {
        QRadioButton *rb = nullptr;
        QLineEdit *le = nullptr;
        QCheckBox *cb = nullptr;

        switch(static_cast<N3NamesModel_defs>(index.column()))
        {
        case N3NamesModel_defs::Active:
            rb = new QRadioButton(parent);
            connect(rb, &QRadioButton::toggled, this, &N3NamesDelegate::radio_toggled);
            rb->setCheckable(true);
            return rb;

        case N3NamesModel_defs::Used:
            return new QCheckBox(parent);

        case N3NamesModel_defs::Hidden:
            cb = new QCheckBox(parent);
            connect(cb, &QCheckBox::toggled, this, &N3NamesDelegate::check_toggled);
            return cb;

        case N3NamesModel_defs::Name:
        {
            le = new QLineEdit(parent);
            le->setValidator(new QRegExpValidator(QRegExp("^[a-zA-Z0-9:;/-,. !]{0,20}$")));
            le->setMaxLength(static_cast<uint>(N3NamesValues::length));
            return le;
        }
        default:
            return nullptr;
        }
    }

    if(index.row() == m_model->filledCount()
            && index.row() < m_model->rowCount(QModelIndex())
            && index.column() == static_cast<int>(N3NamesModel_defs::Active))
    {
        QToolButton *tb = nullptr;
        tb = new QToolButton(parent);
        tb->setText(button_add);
        connect(tb, &QToolButton::clicked, this, &N3NamesDelegate::add);
        return tb;

    }
    return nullptr;
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    QRadioButton *rb = nullptr;
    QCheckBox *cb = nullptr;
    QLineEdit *le = nullptr;

    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    switch(static_cast<N3NamesModel_defs>(index.column()))
    {
    case N3NamesModel_defs::Active:
        rb = static_cast<QRadioButton*>(editor);
        if(nullptr != rb)
            rb->setChecked(index.model()->data(index, Qt::EditRole).toBool());
        break;

    case N3NamesModel_defs::Used:
        cb = static_cast<QCheckBox*>(editor);
        cb->setChecked(index.model()->data(index, Qt::EditRole).toBool());
        break;

    case N3NamesModel_defs::Hidden:
        cb = static_cast<QCheckBox*>(editor);
        if(nullptr != cb)
            cb->setChecked(index.model()->data(index, Qt::EditRole).toBool());
        break;

    case N3NamesModel_defs::Name:
        le = static_cast<QLineEdit*>(editor);
        if(nullptr != le)
        {
            le->setText(index.model()->data(index, Qt::EditRole).toString());
        }
        break;

    default:
        break;
    }
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    if(index.column() == static_cast<int>(N3NamesModel_defs::Active))
    {
        QRadioButton *rb = static_cast<QRadioButton*>(editor);
        if(nullptr != rb)
            model->setData(index, rb->isChecked() , Qt::EditRole);
    }

    if(index.column() == static_cast<int>(N3NamesModel_defs::Hidden))
    {
        QCheckBox *cb = static_cast<QCheckBox*>(editor);
        if(nullptr != cb)
            model->setData(index, cb->isChecked() , Qt::EditRole);
    }

    if(index.column() == static_cast<int>(N3NamesModel_defs::Name))
    {
        QLineEdit *le = static_cast<QLineEdit*>(editor);
        if(nullptr != le)
        {
            model->setData(index, le->text() , Qt::EditRole);
        }
    }

}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;

    editor->setGeometry(calcRect(option, index));
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    if((index.column() == static_cast<int>(N3NamesModel_defs::Active) ||
        index.column() == static_cast<int>(N3NamesModel_defs::Used) ||
        index.column() == static_cast<int>(N3NamesModel_defs::Hidden) )
            && index.row() < m_model->filledCount())
    {
        QStyle::PrimitiveElement pe = (index.column() == static_cast<int>(N3NamesModel_defs::Active)) ? QStyle::PE_IndicatorRadioButton : QStyle::PE_IndicatorItemViewItemCheck;        

        drawBackground(painter, option, index);
        drawFocus(painter, option, option.rect);

        QStyleOptionViewItem opt(option);
        opt.rect = calcRect(option, index);

        //opt.state = opt.state & ~QStyle::State_HasFocus;
        opt.state |= (index.data(Qt::EditRole).toBool() ? QStyle::State_On : QStyle::State_Off);

        if(index.column() == static_cast<int>(N3NamesModel_defs::Used))
            opt.state |= QStyle::State_Sunken;

        if(index.column() != static_cast<int>(N3NamesModel_defs::Active)
                && static_cast<const N3NamesModel*>(index.model())->used(index.row()))
            opt.state |= QStyle::State_Sunken;

        if(index.column() == static_cast<int>(N3NamesModel_defs::Active)
                && static_cast<const N3NamesModel*>(index.model())->hidden(index.row()))
            opt.state |= QStyle::State_Sunken;

        if(index.column() == static_cast<int>(N3NamesModel_defs::Hidden)
                && static_cast<const N3NamesModel*>(index.model())->active(index.row()))
            opt.state |= QStyle::State_Sunken;

        qApp->style()->drawPrimitive(pe, &opt, painter);
        return;
    }
    else
        if((index.column() == static_cast<int>(N3NamesModel_defs::Active)
            && (index.row() == m_model->filledCount())
            && (index.row() < m_model->rowCount(QModelIndex()))))
        {
            QStyle::PrimitiveElement pe = QStyle::PE_PanelButtonTool;

            drawBackground(painter, option, index);
            drawFocus(painter, option, option.rect);

            QStyleOptionToolButton opt;
            opt.rect = calcRect(option, index);
            opt.text = button_add;
            opt.toolButtonStyle = Qt::ToolButtonStyle::ToolButtonTextOnly;
            opt.features = QStyleOptionToolButton::None;
            opt.arrowType = Qt::NoArrow;
            opt.subControls = 0;
            qApp->style()->drawPrimitive(pe, &opt, painter);
            qApp->style()->drawComplexControl(QStyle::CC_ToolButton, &opt, painter);
            return;
        }
    QItemDelegate::paint( painter, option, index );
}

//------------------------------------------------------------------------------------------
bool N3NamesDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(model);

    if(!index.isValid())
        return false;

    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    if(event->type() == QEvent::MouseButtonPress)
    {
        if(model->flags(index) & Qt::ItemIsEditable)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QRect rect = calcRect(option, index);

            if(rect.contains(mouseEvent->pos()))
            {
                if((index.column() == static_cast<int>(N3NamesModel_defs::Active) ||
                    index.column() == static_cast<int>(N3NamesModel_defs::Hidden) )
                        && index.row() < m_model->filledCount())
                    m_model->setData(index, !(index.model()->data(index, Qt::EditRole).toBool())  , Qt::EditRole);
                else
                    if((index.column() == static_cast<int>(N3NamesModel_defs::Active)
                        && (index.row() == m_model->filledCount())
                        && (index.row() < m_model->rowCount(QModelIndex()))))
                        m_model->add();
            }
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}


//------------------------------------------------------------------------------------------
QRect N3NamesDelegate::calcRect(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!m_model)
        m_model = static_cast<N3NamesModel*>(const_cast<QAbstractItemModel*>(index.model()));

    if(index.column() == static_cast<int>(N3NamesModel_defs::Name))
        return option.rect;

    if(index.row() == m_model->filledCount()
            && index.row() < m_model->rowCount(QModelIndex())
            && index.column() == static_cast<int>(N3NamesModel_defs::Active))
        return option.rect;

    QStyle::SubElement se = (index.column() == static_cast<int>(N3NamesModel_defs::Active)) ? QStyle::SE_RadioButtonIndicator : QStyle::SE_CheckBoxIndicator;

    QRect cr = qApp->style()->subElementRect(se, &option);
    int deltaX = (option.rect.width() - cr.width()) / 2;
    int deltaY = (option.rect.height() - cr.height()) / 2;
    return QRect(option.rect.left() + deltaX, option.rect.top() + deltaY, cr.width(), cr.height());
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::radio_toggled(bool value)
{
    if(!value)
        return;
    QRadioButton *rb = static_cast<QRadioButton*>(sender());
    if(rb)
        commitData(rb);
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::check_toggled(bool value)
{
    Q_UNUSED(value);

    QCheckBox *cb = static_cast<QCheckBox*>(sender());
    if(cb)
        commitData(cb);
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::add()
{
    if(m_model)
    {
        closeEditor(static_cast<QToolButton*>(sender()));
        m_model->add();
    }
}
