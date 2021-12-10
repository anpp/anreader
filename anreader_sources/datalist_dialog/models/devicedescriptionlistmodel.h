#ifndef DEVICEDESCRIPTIONLISTMODEL_H
#define DEVICEDESCRIPTIONLISTMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QString>
#include <tuple>
#include <map>
#include <vector>
#include <memory>

#include "common/common.h"

typedef std::tuple<dtype, QString> t_device_description_item;
typedef std::vector<t_device_description_item> t_devicetypelist;

enum class DeviceDescriptionListModel_defs: int {NumColumns = 2, DeviceType = 0, DeviceDescription = 1 };

class DeviceDescriptionListModel : public QAbstractItemModel
{
    const int m_num_columns = static_cast<int>(DeviceDescriptionListModel_defs::NumColumns);
    t_devicetypelist &m_datalist;

public:
    explicit DeviceDescriptionListModel(t_devicetypelist& datalist, QObject *parent = nullptr):
        QAbstractItemModel(parent), m_datalist(datalist) {};

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override { return m_datalist.size(); }
    int columnCount(const QModelIndex&) const override {return m_num_columns; }

    void addItem(const t_device_description_item& item);
    void removeItem(const uint row);

};

#endif // DEVICEDESCRIPTIONLISTMODEL_H
