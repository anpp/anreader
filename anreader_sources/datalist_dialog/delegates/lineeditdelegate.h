#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QItemDelegate>
#include <QLineEdit>

#include "common/common.h"

enum class chars_type: int {all = 0, lat_only};

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT

    chars_type m_ct = chars_type::all;
public:
    LineEditDelegate(QObject *parent = nullptr, chars_type ct = chars_type::all) :
        QItemDelegate(parent),
        m_ct(ct)
    {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LINEEDITDELEGATE_H
