#ifndef N3NAMESMODEL_H
#define N3NAMESMODEL_H

#include <QAbstractItemModel>

class N3Names;

enum class N3NamesModel_defs: int {NumColumns = 4, Active = 0, Used = 1, Hidden = 2, Name = 3 };

class N3NamesModel : public QAbstractItemModel
{
    N3Names& m_data;
public:
    explicit N3NamesModel(N3Names& data, QObject *parent = nullptr):
        QAbstractItemModel(parent), m_data(data) {};

    QModelIndex index(int row, int column, const QModelIndex & = QModelIndex()) const override { return createIndex(row, column);}
    QModelIndex parent(const QModelIndex &) const override { return QModelIndex(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override {return static_cast<int>(N3NamesModel_defs::NumColumns); }

    int filledCount() const;
    bool used(uint index) const;
    bool hidden(uint index) const;
    bool active(uint index) const;
    void add();

};


#endif // N3NAMESMODEL_H
