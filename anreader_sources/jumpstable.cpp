#include "jumpstable.h"
#include <QHeaderView>
#include <QPainter>


//----------------------------------------------------------------------------------------------------------------------
void JumpsTable::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    //resizeColumnsToContents();


}

//----------------------------------------------------------------------------------------------------------------------
void JumpsTable::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);

    if(model() == nullptr || model()->rowCount(QModelIndex()) == 0)
    {
        QPainter painter(viewport());
        QFont font;
        font.setPointSize(20);
        painter.setFont(font);
        painter.setPen(Qt::gray);

        QFontMetrics fm = painter.fontMetrics();
        int text_width = fm.horizontalAdvance(m_NoDataToDisplayInfoText);

        QPoint point = viewport()->geometry().center();
        point.setX(point.x() - text_width / 2);

        painter.drawText(point, tr(m_NoDataToDisplayInfoText.toStdString().c_str()));
        painter.end();
    }
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
