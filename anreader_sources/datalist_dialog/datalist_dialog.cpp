#include "datalist_dialog.h"
#include "ui_datalist_dialog.h"


//---------------------------------------------------------------------------------------------------------------
DataList_Dialog::DataList_Dialog(const QString& Title, t_registry& datalist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataList_Dialog)
{
    ListWidget *listwidget = new ListWidget(datalist);
    ui->setupUi(this);
    this->setWindowTitle(Title);    
    ui->centralLayout->addWidget(listwidget);

    connect(listwidget, &ListWidget::selectedItemUsed,  this, &DataList_Dialog::selectedItemUsed);
    connect(this, &DataList_Dialog::removeFocusedItem, listwidget, &ListWidget::removeFocusedItem);
    connect(this, &DataList_Dialog::addItem, listwidget, &ListWidget::addItem);
}


//---------------------------------------------------------------------------------------------------------------
DataList_Dialog::DataList_Dialog(const QString &Title, t_devicetypelist &datalist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataList_Dialog)
{
    ListDeviceTypesWidget *listwidget = new ListDeviceTypesWidget(datalist);
    ui->setupUi(this);
    this->setWindowTitle(Title);
    ui->centralLayout->addWidget(listwidget);

    connect(this, &DataList_Dialog::removeFocusedItem, listwidget, &ListDeviceTypesWidget::removeFocusedItem);
    connect(this, &DataList_Dialog::addItem, listwidget, &ListDeviceTypesWidget::addItem);

}


//---------------------------------------------------------------------------------------------------------------
DataList_Dialog::~DataList_Dialog()
{
    delete ui;
}


//---------------------------------------------------------------------------------------------------------------
void DataList_Dialog::on_buttonBox_accepted()
{
    accept();
}


//---------------------------------------------------------------------------------------------------------------
void DataList_Dialog::on_btnAdd_clicked()
{
    emit addItem();
}

//---------------------------------------------------------------------------------------------------------------
void DataList_Dialog::on_btnRemove_clicked()
{
    emit removeFocusedItem();
}


//---------------------------------------------------------------------------------------------------------------
void DataList_Dialog::selectedItemUsed(bool value)
{
    ui->btnRemove->setEnabled(!value);
}
