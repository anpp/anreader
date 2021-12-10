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
public:
    explicit ListWidget(t_registry& datalist, QWidget *parent = nullptr);

    const QTableView& listTable() const { return *m_listTable; }

private:
    std::unique_ptr<QTableView> m_listTable = std::make_unique<QTableView>(this);
    DataListModel m_model;
signals:
    void selectedItemUsed(bool value);

public slots:
    void rowChanged();
    void addItem();
    void removeFocusedItem();

};

#endif // LISTWIDGET_H
