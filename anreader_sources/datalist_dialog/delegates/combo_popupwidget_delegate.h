#ifndef COMBOPOPUPWIDGETDELEGATE_H
#define COMBOPOPUPWIDGETDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <QListWidget>
#include <QWidget>
#include <QSizeGrip>

#include "common.h"





//===========================================================================================================
class PopupComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit PopupComboBox(QWidget *parent = nullptr);

    QListWidget m_view;
    QWidget m_widget;
    QSizeGrip sg;
/*
protected:
    void showPopup()
    {
        QComboBox::showPopup();
    }
    (*/
};


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

#endif // COMBOPOPUPWIDGETDELEGATE_H
