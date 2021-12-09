#include "listwidget.h"


//-----------------------------------------------------------------------------------------------------------------------------
ListWidget::ListWidget(t_registry& datalist, QWidget *parent) : QWidget(parent), m_model(datalist)
{
    auto layout = new QVBoxLayout;
    auto ledKey = new LineEditDelegate(this, chars_type::lat_only);
    auto ledValue = new LineEditDelegate(this);


    layout->addWidget(m_listTable.get());
    this->setLayout(layout);

    layout->setMargin(0);

    m_listTable->setModel(&m_model);
    m_listTable->setItemDelegateForColumn(static_cast<int>(DataListModel_defs::Key), ledKey);
    m_listTable->setItemDelegateForColumn(static_cast<int>(DataListModel_defs::Value), ledValue);
    m_listTable->resizeColumnsToContents();
    m_listTable->resizeRowsToContents();
    m_listTable->verticalHeader()->hide();
    m_listTable->horizontalHeader()->setStretchLastSection(true);
    m_listTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listTable->setAlternatingRowColors(true);
    connect(m_listTable->selectionModel(), &QItemSelectionModel::currentRowChanged,  this, &ListWidget::rowChanged);
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListWidget::addItem()
{
    m_model.addItem(std::make_tuple(false, "", ""));
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListWidget::removeFocusedItem()
{
    if(!m_listTable->currentIndex().isValid())
        return;

    auto currentRow  = m_listTable->currentIndex().row();
    m_model.removeItem(currentRow);
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListWidget::rowChanged()
{
    emit selectedItemUsed(m_model.isUsed(m_listTable->currentIndex().row()));
}
