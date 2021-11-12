#include "datalist_dialog.h"
#include "ui_datalist_dialog.h"

DataList_Dialog::DataList_Dialog(const QString& Title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataList_Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(Title);
    ui->centralLayout->addWidget(&m_listWidget);
}

DataList_Dialog::~DataList_Dialog()
{
    delete ui;
}

void DataList_Dialog::on_buttonBox_accepted()
{
    accept();
}
