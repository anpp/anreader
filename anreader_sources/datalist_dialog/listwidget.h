#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

#include "datalistmodel.h"

class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);

private:
    QPointer<QTableView> m_listTable = new QTableView(this);
    DataListModel m_model;
signals:

};

#endif // LISTWIDGET_H
