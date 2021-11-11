#include "listwidget.h"

ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;
    layout->addWidget(m_listTable);
    this->setLayout(layout);

    layout->setMargin(0);
}
