#ifndef COMBOLIST_MODEL_H
#define COMBOLIST_MODEL_H

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QVector>
#include "datalists.h"

typedef std::vector<std::pair<QString, QString>> t_combodata;

class Combolist_model : public QAbstractItemModel
{
    Q_OBJECT

    const int m_num_columns;
    t_combodata m_data;
public:
    explicit Combolist_model(const map_DataList& datalist, QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override { return m_data.size(); }
    int columnCount(const QModelIndex&) const override { return m_num_columns; }

};

#endif // COMBOLIST_MODEL_H
