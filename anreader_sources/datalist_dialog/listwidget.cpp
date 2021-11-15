#include "listwidget.h"

ListWidget::ListWidget(t_datalist& datalist, QWidget *parent) : QWidget(parent), m_model(datalist)
{
    auto layout = new QVBoxLayout;
    auto delegate = new LineEditDelegate(this);

    layout->addWidget(m_listTable.get());
    this->setLayout(layout);

    layout->setMargin(0);

    m_listTable->setModel(&m_model);
    m_listTable->setItemDelegateForColumn(DataListModel_defs::Key, delegate);
    m_listTable->setItemDelegateForColumn(DataListModel_defs::Value, delegate);
    m_listTable->resizeColumnsToContents();
    m_listTable->verticalHeader()->hide();
    m_listTable->horizontalHeader()->setStretchLastSection(true);
    m_listTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ListWidget::addItem(const t_datalist_item &item)
{
    m_model.addItem(item);
}

void ListWidget::removeFocusedItem()
{
    auto currentRow  = m_listTable->currentIndex().row();
    m_model.removeItem(currentRow);
}
