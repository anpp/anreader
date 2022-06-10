#ifndef N3NAMESDELEGAT_H
#define N3NAMESDELEGAT_H

#include <QItemDelegate>
#include <QObject>

class N3NamesDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    N3NamesDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QRect calcRect(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
    void radio_toggled(bool value);
};

#endif // N3NAMESDELEGAT_H
