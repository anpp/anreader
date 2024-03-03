#include "n3names_delegate.h"
#include "n3names_model.h"
#include "n3alarms_settings_model.h"
#include "device/n3names.h"

#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QApplication>
#include <QMouseEvent>
#include <QToolButton>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QSpinBox>

//------------------------------------------------------------------------------------------
QWidget *N3NamesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if(!index.isValid())
        return nullptr;

    switch(modelType(*index.model()))
    {
    case ModelType::N3Names:
        return createEditorN3Names(parent, option, index);
    case ModelType::N3AlarmsSettings:
        return createEditorN3AlarmsSettings(parent, option, index);
    default:
        return nullptr;
    }
}

//------------------------------------------------------------------------------------------
QWidget *N3NamesDelegate::createEditorN3Names(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if(!index.isValid())
        return nullptr;

    if(index.row() < static_cast<const N3NamesModel*>(index.model())->filledCount())
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
            le->setMaxLength(N3Names::N3NamesValues::length);

            if(index.row() == static_cast<const N3NamesModel*>(index.model())->filledCount() - 1 && static_cast<const N3NamesModel*>(index.model())->filledCount() > 0
                && !static_cast<const N3NamesModel*>(index.model())->value(index.row(), static_cast<int>(N3NamesModel_defs::Active), Qt::EditRole).toBool()
                && !static_cast<const N3NamesModel*>(index.model())->value(index.row(), static_cast<int>(N3NamesModel_defs::Used), Qt::EditRole).toBool())
            {
                //проверка если имя используется в сигналах - кнопку удалить не создавать
                if(!static_cast<const N3NamesModel*>(index.model())->selected(index.row()))
                {
                    QAction *adel = le->addAction(QIcon(":/images/icons/buttons/delete.png"), QLineEdit::TrailingPosition);
                    adel->setToolTip(tr("Delete this item"));

                    connect(adel, &QAction::triggered, this, &N3NamesDelegate::del);
                    connect(adel, &QAction::triggered, static_cast<const N3NamesModel*>(index.model()), &N3NamesModel::del);
                }
            }
            return le;
        }
        default:
            return nullptr;
        }
    }

    if(index.row() == static_cast<const N3NamesModel*>(index.model())->filledCount()
        && index.row() < static_cast<const N3NamesModel*>(index.model())->rowCount(QModelIndex())
        && index.column() == static_cast<int>(N3NamesModel_defs::Active))
    {
        QToolButton *tb = nullptr;
        tb = new QToolButton(parent);
        tb->setText(button_add);
        connect(tb, &QToolButton::clicked, this, &N3NamesDelegate::add);
        connect(tb, &QToolButton::clicked, static_cast<const N3NamesModel*>(index.model()), &N3NamesModel::add);
        return tb;

    }
    return nullptr;
}

//------------------------------------------------------------------------------------------
QWidget *N3NamesDelegate::createEditorN3AlarmsSettings(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    if(!index.isValid()) return nullptr;

    if(index.row() < index.model()->rowCount())
    {
        QRadioButton *rb = nullptr;
        QComboBox *cmb = nullptr;
        QSpinBox *spb = nullptr;

        switch(static_cast<N3AlarmsSettings_defs>(index.column()))
        {
        case N3AlarmsSettings_defs::Active:
            rb = new QRadioButton(parent);
            connect(rb, &QRadioButton::toggled, this, &N3NamesDelegate::radio_toggled);
            rb->setCheckable(true);
            return rb;

        case N3AlarmsSettings_defs::NameIndex:
            cmb = new QComboBox(parent);
            cmb->addItems(index.model()->data(index, N3AlarmsSettings_defs::NamesRole).toStringList());
            connect(cmb, &QComboBox::currentTextChanged, this, &N3NamesDelegate::combo_changed);
            return cmb;

        case N3AlarmsSettings_defs::AlarmAltitude1:
        case N3AlarmsSettings_defs::AlarmAltitude2:
        case N3AlarmsSettings_defs::AlarmAltitude3:
            spb = new QSpinBox(parent);
            spb->setSingleStep(index.model()->data(index, N3AlarmsSettings_defs::StepRole).toInt());
            spb->setRange(index.model()->data(index, N3AlarmsSettings_defs::MinAltRole).toInt(), index.model()->data(index, N3AlarmsSettings_defs::MaxAltRole).toInt());
            return spb;

        default:
            return nullptr;
        }
    }
    return nullptr;
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::setEditorDataN3Names(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    QRadioButton *rb = nullptr;
    QCheckBox *cb = nullptr;
    QLineEdit *le = nullptr;

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
#include <QDebug>
//------------------------------------------------------------------------------------------
void N3NamesDelegate::setEditorDataN3AlarmsSettings(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(index.row() < index.model()->rowCount())
    {
        QRadioButton *rb = nullptr;
        QComboBox *cmb = nullptr;
        QSpinBox *spb = nullptr;

        switch(static_cast<N3AlarmsSettings_defs>(index.column()))
        {
        case N3AlarmsSettings_defs::Active:
            rb = static_cast<QRadioButton*>(editor);
            if(nullptr != rb)
                rb->setChecked(index.model()->data(index, Qt::EditRole).toBool());
            break;

        case N3AlarmsSettings_defs::NameIndex:
            cmb = static_cast<QComboBox*>(editor);
            if(nullptr != cmb)
                cmb->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
            qDebug() << index.model()->data(index, Qt::EditRole).toString();
            break;

        case N3AlarmsSettings_defs::AlarmAltitude1:
        case N3AlarmsSettings_defs::AlarmAltitude2:
        case N3AlarmsSettings_defs::AlarmAltitude3:
            spb = static_cast<QSpinBox*>(editor);
            if(nullptr != spb)
                spb->setValue(index.model()->data(index, Qt::EditRole).toInt());
            break;


        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::setModelDataN3Names(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(index.column() == static_cast<int>(N3NamesModel_defs::Active)
        && static_cast<const N3NamesModel*>(index.model())->n3data().type() != N3NamesType::Alarms)
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
/*
    if(index.column() == static_cast<int>(N3NamesModel_defs::Used))
    {
        QCheckBox *cb = static_cast<QCheckBox*>(editor);
        if(nullptr != cb)
            model->setData(index, cb->isChecked() , Qt::EditRole);
    }
*/
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
void N3NamesDelegate::setModelDataN3AlarmsSettings(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(index.row() < index.model()->rowCount())
    {
        QRadioButton *rb = nullptr;
        QComboBox *cmb = nullptr;
        QSpinBox *spb = nullptr;

        switch(static_cast<N3AlarmsSettings_defs>(index.column()))
        {
        case N3AlarmsSettings_defs::Active:
            rb = static_cast<QRadioButton*>(editor);
            if(nullptr != rb)
                model->setData(index, rb->isChecked(), Qt::EditRole);
            break;

        case N3AlarmsSettings_defs::NameIndex:
            cmb = static_cast<QComboBox*>(editor);
            if(nullptr != cmb)
                model->setData(index, cmb->currentText(), Qt::EditRole);
            break;
        case N3AlarmsSettings_defs::AlarmAltitude1:
        case N3AlarmsSettings_defs::AlarmAltitude2:
        case N3AlarmsSettings_defs::AlarmAltitude3:
            spb = static_cast<QSpinBox*>(editor);
            if(nullptr != spb)
                model->setData(index, spb->value(), Qt::EditRole);

            break;

        default:
            break;
        }
    }

}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::paintN3Names(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if((index.column() == static_cast<int>(N3NamesModel_defs::Active) ||
         index.column() == static_cast<int>(N3NamesModel_defs::Used) ||
         index.column() == static_cast<int>(N3NamesModel_defs::Hidden) )
        && index.row() < static_cast<const N3NamesModel*>(index.model())->filledCount())
    {
        QStyle::PrimitiveElement pe =
            (index.column() == static_cast<int>(N3NamesModel_defs::Active) && static_cast<const N3NamesModel*>(index.model())->n3data().type() != N3NamesType::Alarms) ?
                QStyle::PE_IndicatorRadioButton : QStyle::PE_IndicatorItemViewItemCheck;

        drawPrimitive(painter, option, index, pe);
        return;
    }
    else
        if((index.column() == static_cast<int>(N3NamesModel_defs::Active)
             && (index.row() == static_cast<const N3NamesModel*>(index.model())->filledCount())
             && (index.row() < static_cast<const N3NamesModel*>(index.model())->rowCount(QModelIndex()))))
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
            opt.subControls = QStyle::SC_None;
            qApp->style()->drawPrimitive(pe, &opt, painter);
            qApp->style()->drawComplexControl(QStyle::CC_ToolButton, &opt, painter);
            return;
        }
    QItemDelegate::paint( painter, option, index );
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::paintN3AlarmsSettings(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    if(index.column() == static_cast<int>(N3AlarmsSettings_defs::Active))
    {
            QStyle::PrimitiveElement pe = QStyle::PE_IndicatorRadioButton;
            drawPrimitive(painter, option, index, pe);
            return;
    }
    QItemDelegate::paint( painter, option, index );
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::drawPrimitive(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, QStyle::PrimitiveElement pe) const
{
    if(!index.isValid()) return;

    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    QStyleOptionViewItem opt(option);
    opt.rect = calcRect(option, index);

    opt.state = QStyle::State_Enabled | (index.data(Qt::DisplayRole).toBool() ? QStyle::State_On : QStyle::State_Off);

    if(!(index.model()->flags(index) & Qt::ItemIsEditable))
    {
        if(QApplication::style()->objectName() == "fusion")
            opt.state |= QStyle::State_Sunken;
            else
        opt.state &= ~QStyle::State_Enabled;
    }
    qApp->style()->drawPrimitive(pe, &opt, painter);
}


//------------------------------------------------------------------------------------------
void N3NamesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    switch(modelType(*index.model()))
    {
        case ModelType::N3Names:
            setEditorDataN3Names(editor, index);
            break;
        case ModelType::N3AlarmsSettings:
            setEditorDataN3AlarmsSettings(editor, index);
            break;
        default:
            break;
    }
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    switch(modelType(*index.model()))
    {
        case ModelType::N3Names:
            setModelDataN3Names(editor, model, index);
            break;
        case ModelType::N3AlarmsSettings:
            setModelDataN3AlarmsSettings(editor, model, index);
            break;
        default:
            break;
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

    switch(modelType(*index.model()))
    {
    case ModelType::N3Names:
        paintN3Names(painter, option, index);
        break;
    case ModelType::N3AlarmsSettings:
        paintN3AlarmsSettings(painter, option, index);
        break;
    default:
        QItemDelegate::paint( painter, option, index );
        break;
    }
}

//------------------------------------------------------------------------------------------
bool N3NamesDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(!index.isValid())
        return false;

    if(event->type() == QEvent::MouseButtonPress)
    {
        if(model->flags(index) & Qt::ItemIsEditable)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QRect rect = calcRect(option, index);

            if(rect.contains(mouseEvent->pos()) && mouseEvent->button() == Qt::MouseButton::LeftButton)
            {
                if(modelType(*index.model()) == ModelType::N3Names)
                {
                    if((index.column() == static_cast<int>(N3NamesModel_defs::Active) ||
                         index.column() == static_cast<int>(N3NamesModel_defs::Hidden) )
                        && index.row() < static_cast<const N3NamesModel*>(index.model())->filledCount())
                        model->setData(index, !(index.model()->data(index, Qt::EditRole).toBool())  , Qt::EditRole);
                    else
                        if((index.column() == static_cast<int>(N3NamesModel_defs::Active)
                             && (index.row() == static_cast<const N3NamesModel*>(index.model())->filledCount())
                             && (index.row() < static_cast<const N3NamesModel*>(index.model())->rowCount(QModelIndex()))))
                            static_cast<N3NamesModel*>(model)->add();
                }

                if(modelType(*index.model()) == ModelType::N3AlarmsSettings && index.column() == static_cast<int>(N3AlarmsSettings_defs::Active))
                    model->setData(index, !(index.model()->data(index, Qt::EditRole).toBool())  , Qt::EditRole);
            }
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}


//------------------------------------------------------------------------------------------
QRect N3NamesDelegate::calcRect(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return option.rect;

    QStyle::SubElement se = QStyle::SE_RadioButtonIndicator;

    if(modelType(*index.model()) == ModelType::N3Names)
    {
        if(index.column() == static_cast<int>(N3NamesModel_defs::Name))
            return option.rect;

        if(index.row() == static_cast<const N3NamesModel*>(index.model())->filledCount()
            && index.row() < static_cast<const N3NamesModel*>(index.model())->rowCount(QModelIndex())
            && index.column() == static_cast<int>(N3NamesModel_defs::Active))
            return option.rect;

        se = (index.column() == static_cast<int>(N3NamesModel_defs::Active)) ? QStyle::SE_RadioButtonIndicator : QStyle::SE_CheckBoxIndicator;
    }

    if(modelType(*index.model()) == ModelType::N3AlarmsSettings &&
        (
            index.column() == static_cast<int>(N3AlarmsSettings_defs::NameIndex) ||
            index.column() == static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude1) ||
            index.column() == static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude2) ||
            index.column() == static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude3)
        )
       )
        return option.rect;

    QRect cr = qApp->style()->subElementRect(se, &option);
    int deltaX = (option.rect.width() - cr.width()) / 2;
    int deltaY = (option.rect.height() - cr.height()) / 2;
    return QRect(option.rect.left() + deltaX, option.rect.top() + deltaY, cr.width(), cr.height());
}

//------------------------------------------------------------------------------------------
N3NamesDelegate::ModelType N3NamesDelegate::modelType(const QAbstractItemModel &model) const
{
    QVariant model_type = model.property("model_type");
    if(model_type.isValid() && !model_type.isNull())
    {
        if(model_type.toString() == "N3Names")
            return ModelType::N3Names;
        if(model_type.toString() == "N3AlarmsSettings")
            return ModelType::N3AlarmsSettings;
    }
    return ModelType::Unk;
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::radio_toggled(bool value)
{
    if(!value)
        return;
    emit commitData(static_cast<QRadioButton*>(sender()));
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::check_toggled(bool value)
{
    Q_UNUSED(value);

    emit commitData(static_cast<QCheckBox*>(sender()));
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::combo_changed(const QString &value)
{
    Q_UNUSED(value);

    emit commitData(static_cast<QComboBox*>(sender()));
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::add()
{
    emit closeEditor(static_cast<QToolButton*>(sender()));
}

//------------------------------------------------------------------------------------------
void N3NamesDelegate::del()
{
    emit closeEditor(static_cast<QLineEdit*>(static_cast<QAction*>(sender())->parent()));
}
