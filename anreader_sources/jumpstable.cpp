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
#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
        int text_width = fm.width(m_NoDataToDisplayInfoText);
#else
        int text_width = fm.horizontalAdvance(m_NoDataToDisplayInfoText);
#endif
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
