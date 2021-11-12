#include "listwidget.h"

ListWidget::ListWidget(t_datalist& datalist, QWidget *parent) : QWidget(parent), m_model(datalist)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(m_listTable);
    this->setLayout(layout);

    layout->setMargin(0);

    m_listTable->setModel(&m_model);
    m_listTable->resizeColumnsToContents();
    m_listTable->horizontalHeader()->setStretchLastSection(true);
}
