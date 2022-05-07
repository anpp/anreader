#include "n3_main_settings_dialog.h"
#include "ui_n3_main_settings_dialog.h"

N3MainSettingsDialog::N3MainSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::N3MainSettingsDialog)
{
    ui->setupUi(this);
}

N3MainSettingsDialog::~N3MainSettingsDialog()
{
    delete ui;
}
