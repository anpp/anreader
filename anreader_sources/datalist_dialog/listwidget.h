#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);

private:
    QPointer<QTableView> m_listTable = new QTableView(this);
signals:

};

#endif // LISTWIDGET_H
