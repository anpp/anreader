#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT

    bool m_lat_only = false;
public:
    LineEditDelegate(QObject *parent = nullptr, bool lat_only = false) :
        QItemDelegate(parent),
        m_lat_only(lat_only)
    {};

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LINEEDITDELEGATE_H
