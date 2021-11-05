#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

N3JumpEditor::N3JumpEditor(QWidget *parent, N3Jump& jump, const map_APs& aircrafts, const map_DZs& dropzones) :
    QDialog(parent),
    ui(new Ui::N3JumpEditor)
{
    ui->setupUi(this);

    ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());

    for(auto it = aircrafts.begin(); it != aircrafts.end(); ++it)
        ui->cbAirplane->addItem((*it).first);

    for(auto it = dropzones.begin(); it != dropzones.end(); ++it)
        ui->cbDZ->addItem((*it).first);


    ui->cbDZ->setCurrentText(jump.getDZ());
    ui->cbAirplane->setCurrentText(jump.getAP());
}

N3JumpEditor::~N3JumpEditor()
{
    delete ui;
}
