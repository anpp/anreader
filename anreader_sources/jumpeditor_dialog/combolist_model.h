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

    t_combodata m_data;
    const int m_num_columns;
    const datakind m_dk;
    const DataLists& m_dl;
public:
    explicit Combolist_model(const datakind dk, const DataLists& ref_dl, QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override { return m_data.size(); }
    int columnCount(const QModelIndex&) const override { return m_num_columns; }
    int indexByKey(const QString& key);

};

#endif // COMBOLIST_MODEL_H
