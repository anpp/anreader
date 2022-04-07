#include "choice_datetime_dialog.h"
#include "ui_choice_datetime_dialog.h"

//------------------------------------------------------------------------------------------
ChoiceDateTimeDialog::ChoiceDateTimeDialog(const QDateTime& dt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceDateTimeDialog)
{    
    ui->setupUi(this);
    ui->dteTime->setDateTime(dt);
}

//------------------------------------------------------------------------------------------
ChoiceDateTimeDialog::~ChoiceDateTimeDialog()
{
    delete ui;
}

//------------------------------------------------------------------------------------------
QDateTime ChoiceDateTimeDialog::datetime() const
{
    return ui->dteTime->dateTime();
}

//------------------------------------------------------------------------------------------
void ChoiceDateTimeDialog::on_tbCurrentTime_clicked()
{
    ui->dteTime->setDateTime(QDateTime::currentDateTime());
}

