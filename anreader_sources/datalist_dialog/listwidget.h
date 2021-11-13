#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <memory>

#include "datalistmodel.h"

class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(t_datalist& datalist, QWidget *parent = nullptr);

    void addItem(const t_datalist_item& item);
    void removeFocusedItem();
    const QTableView& listTable() const { return *m_listTable; }

private:
    std::unique_ptr<QTableView> m_listTable = std::make_unique<QTableView>(this);
    DataListModel m_model;
signals:

};

#endif // LISTWIDGET_H
