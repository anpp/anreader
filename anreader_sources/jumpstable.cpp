#include "jumpstable.h"

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
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}
