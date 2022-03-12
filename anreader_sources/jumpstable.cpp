#include "jumpstable.h"
#include <QHeaderView>

//----------------------------------------------------------------------------------------------------------------------
void JumpsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    //resizeColumnsToContents();


}

//----------------------------------------------------------------------------------------------------------------------
JumpsTable::JumpsTable()
{
    //verticalHeader()->hide();
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->hide();
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
