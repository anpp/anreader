#include "datalist_dialog.h"
#include "ui_datalist_dialog.h"


//---------------------------------------------------------------------------------------------------------------
DataList_Dialog::DataList_Dialog(const QString& Title, t_datalist& datalist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataList_Dialog),
    m_listWidget(datalist)
{
    ui->setupUi(this);
    this->setWindowTitle(Title);
    ui->centralLayout->addWidget(&m_listWidget);
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
    m_listWidget.addItem(std::make_tuple(false, "", ""));
}

//---------------------------------------------------------------------------------------------------------------
void DataList_Dialog::on_btnRemove_clicked()
{
    QModelIndex currentIndex  = m_listWidget.listTable().currentIndex();
    m_listWidget.removeItem(currentIndex.row());
}
