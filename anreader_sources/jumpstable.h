#ifndef JUMPSTABLE_H
#define JUMPSTABLE_H

#include <QTableView>


class JumpsTable : public QTableView
{
protected:
    void resizeEvent(QResizeEvent *event);
public:
    JumpsTable();
};

#endif // JUMPSTABLE_H
