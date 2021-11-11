#include "listwidget.h"

ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(m_listTable);
    this->setLayout(layout);

    layout->setMargin(0);

    m_listTable->resizeColumnsToContents();
    m_listTable->horizontalHeader()->setStretchLastSection(true);
    m_listTable->setModel(&m_model);
}
