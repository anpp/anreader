#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <memory>

#include "common/common.h"
#include "models/datalistmodel.h"
#include "delegates/lineeditdelegate.h"

class ListWidget : public QWidget
{
    Q_OBJECT

    std::unique_ptr<QTableView> m_listTable = std::make_unique<QTableView>(this);
    DataListModel m_model;

public:
    explicit ListWidget(t_registry& datalist, QWidget *parent = nullptr);
    const QTableView& listTable() const { return *m_listTable; }

signals:
    void selectedItemUsed(bool value);

public slots:
    void rowChanged();
    void addItem();
    void removeFocusedItem();
    void setupCurrentRow();

};

#endif // LISTWIDGET_H
