#ifndef DATALISTMODEL_H
#define DATALISTMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <tuple>
#include <map>
#include <vector>
#include <memory>

typedef std::tuple<bool, QString, QString> t_datalist_item;
typedef std::vector<t_datalist_item> t_datalist;

enum DataListModel_defs: int {NumColumns = 3, Used = 0, Key = 1, Value = 2};

class DataListModel : public QAbstractItemModel
{    
    const int num_columns = DataListModel_defs::NumColumns;
    t_datalist &m_datalist;
public:
    explicit DataListModel(t_datalist& datalist, QObject *parent = nullptr):
        QAbstractItemModel(parent), m_datalist(datalist) {};

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override { return m_datalist.size(); }
    int columnCount(const QModelIndex&) const override {return num_columns; }

    void addItem(const t_datalist_item& item);
    void removeItem(const uint row);
    bool isUsed(const uint row) const;
    bool isEmptyKey(const uint row) const;


};

#endif // DATALISTMODEL_H
