#ifndef COMBOPOPUPWIDGETDELEGATE_H
#define COMBOPOPUPWIDGETDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QListWidget>
#include <QWidget>
#include <QSizeGrip>
#include <QKeyEvent>
#include <QStyledItemDelegate>

#include "common/stringlist_popup.h"



//===========================================================================================================
class ComboPopupWidgetDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboPopupWidgetDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};




//===========================================================================================================
//Device description combobox with popup widget
class DDComboBox: public QComboBox
{
    Q_OBJECT

    StringListPopup m_widget;
    QSizeGrip sg;
    QString m_current_text;
    bool m_accepted = false;

public:

    class PopupItemDelegate: public QStyledItemDelegate
    {
        int  m_ViewPortHeight;
    public:
        explicit PopupItemDelegate(int  ViewPortHeight, QWidget *parent = nullptr);
        using QStyledItemDelegate::QStyledItemDelegate;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {
            QSize s = QStyledItemDelegate::sizeHint(option, index);
            s.setHeight(m_ViewPortHeight);
            return s;
        }
    };

    explicit DDComboBox(QWidget *parent = nullptr, const QString& strings = "");

    QString currentText() const;

protected:
    void showPopup() override;
    void hidePopup() override;
    void keyPressEvent(QKeyEvent *e) override;
    virtual bool eventFilter(QObject *, QEvent *) override;

public slots:
    void closePopup(bool accepted);
};


#endif // COMBOPOPUPWIDGETDELEGATE_H
