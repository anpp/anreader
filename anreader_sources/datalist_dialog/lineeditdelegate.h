#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    LineEditDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {};

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LINEEDITDELEGATE_H
