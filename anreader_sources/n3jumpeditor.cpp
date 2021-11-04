#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

N3JumpEditor::N3JumpEditor(QWidget *parent, N3Jump& jump) :
    QDialog(parent),
    ui(new Ui::N3JumpEditor)
{
    ui->setupUi(this);

    ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());
}

N3JumpEditor::~N3JumpEditor()
{
    delete ui;
}
