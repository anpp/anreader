#include "listdevicetypeswidget.h"

//-----------------------------------------------------------------------------------------------------------------------------
ListDeviceTypesWidget::ListDeviceTypesWidget(t_devicetypelist &datalist, QWidget *parent): QWidget(parent), m_model(datalist)
{
    auto layout = new QVBoxLayout;
    auto cedType = new ComboEditDelegate(this);
    auto cedDescription = new ComboPopupWidgetDelegate(this);

    layout->addWidget(m_listTable.get());
    this->setLayout(layout);

    layout->setMargin(0);

    m_listTable->setModel(&m_model);
    m_listTable->setItemDelegateForColumn(static_cast<int>(DeviceDescriptionListModel_defs::DeviceType), cedType);
    m_listTable->setItemDelegateForColumn(static_cast<int>(DeviceDescriptionListModel_defs::DeviceDescription), cedDescription);
    m_listTable->resizeColumnsToContents();
    m_listTable->resizeRowsToContents();
    m_listTable->verticalHeader()->hide();
    m_listTable->horizontalHeader()->setStretchLastSection(true);
    m_listTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listTable->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(m_listTable->selectionModel(), &QItemSelectionModel::currentRowChanged,  this, &ListDeviceTypesWidget::rowChanged);
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListDeviceTypesWidget::setupCurrentRow()
{
    emit m_listTable->selectionModel()->currentRowChanged(QModelIndex(), QModelIndex());
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListDeviceTypesWidget::addItem()
{
   m_model.addItem(std::make_tuple(dtype::unk, ""));
}


//-----------------------------------------------------------------------------------------------------------------------------
void ListDeviceTypesWidget::removeFocusedItem()
{
    if(!m_listTable->currentIndex().isValid())
        return;

    auto currentRow  = m_listTable->currentIndex().row();
    m_model.removeItem(currentRow);
}

//-----------------------------------------------------------------------------------------------------------------------------
void ListDeviceTypesWidget::rowChanged()
{
    const int row_index = m_listTable->currentIndex().row();
    emit rowIsEnabled(row_index >= 0 && row_index < m_model.rowCount());
}
