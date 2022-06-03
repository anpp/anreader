#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

N3NamesDialog::N3NamesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::N3NamesDialog)
{
    ui->setupUi(this);
}

N3NamesDialog::~N3NamesDialog()
{
    delete ui;
}
