#ifndef COMBOEDITDELEGATE_H
#define COMBOEDITDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

#include "common/common.h"

class ComboEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboEditDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // COMBOEDITDELEGATE_H
