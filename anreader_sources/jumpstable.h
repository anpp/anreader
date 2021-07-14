#ifndef JUMPSTABLE_H
#define JUMPSTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>



class JumpsTable : public QTableView
{
protected:
    void resizeEvent(QResizeEvent *event);
public:
    JumpsTable();
};

#endif // JUMPSTABLE_H
