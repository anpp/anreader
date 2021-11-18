#ifndef JUMPSTABLEMODEL_H
#define JUMPSTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QColor>
#include <memory>


#include "device/jumps/n3jump.h"
#include "datalists.h"

typedef t_jumps t_rows;


class JumpsTableModel : public QAbstractItemModel
{
    Q_OBJECT

    std::unique_ptr<t_rows> m_rows = std::make_unique<t_rows>();
    int num_cols;
    QColor color_0{QColor(Qt::white)}, color_1{QColor(245, 245, 245)};
    const DataLists& ref_dl;
    const QString empty_string = "";
public:

    explicit JumpsTableModel(const DataLists& dl, QObject *parent = nullptr)
        : QAbstractItemModel(parent), num_cols(0),
          ref_dl(dl)
    {}


    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;    
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;    
    bool addItem(const std::shared_ptr<CustomJump>& jump);
    bool addItems(const t_rows& jumps);
    bool moveItems(std::unique_ptr<t_rows>& jumps);
    void removeItem(const uint row);
    void clear();
    std::shared_ptr<CustomJump> getItem(const uint row) const;
    const t_rows& items() const {return *m_rows; }

private:
    void setColumnCount(const int value);
    bool checkColumns(const int value);
    const QString& mappedValue(const map_DataList& mdl, const QString& key) const;

public slots:
    void takeLastJump(int &value);
};

#endif // JUMPSTABLEMODEL_H
