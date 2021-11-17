#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

//-------------------------------------------------------------------------------------------------------------------------------------
N3JumpEditor::N3JumpEditor(QWidget *parent, N3Jump& jump, const map_DataList& aircrafts, const map_DataList& dropzones, const map_DataList& canopies) :
    QDialog(parent),
    ui(new Ui::N3JumpEditor)
{
    ui->setupUi(this);

    ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());

    setComboValues(*ui->cbAirplane, aircrafts);
    setComboValues(*ui->cbDZ, dropzones);
    setComboValues(*ui->cbCanopy, canopies);

    ui->cbDZ->setCurrentText(jump.getDZ());
    ui->cbAirplane->setCurrentText(jump.getAP());
    ui->cbCanopy->setCurrentText(jump.getCanopy());
    ui->teNote->setPlainText(jump.getNote());

    ptrJump = &jump;
}


//-------------------------------------------------------------------------------------------------------------------------------------
N3JumpEditor::~N3JumpEditor()
{
    delete ui;    
}


//-------------------------------------------------------------------------------------------------------------------------------------
void N3JumpEditor::setComboValues(QComboBox &combo, const map_DataList &data_list) const
{
    for(auto it = data_list.begin(); it != data_list.end(); ++it)
        combo.addItem((*it).first);
}




//-------------------------------------------------------------------------------------------------------------------------------------
void N3JumpEditor::on_buttonBox_accepted()
{
    if(nullptr != ptrJump)
    {
        if(ptrJump->getAP() != ui->cbAirplane->currentText())
        {
            ptrJump->setAP(ui->cbAirplane->currentText());
            m_modified = true;
        }

        if(ptrJump->getDZ() != ui->cbDZ->currentText())
        {
            ptrJump->setDZ(ui->cbDZ->currentText());
            m_modified = true;
        }

        if(ptrJump->getCanopy() != ui->cbCanopy->currentText())
        {
            ptrJump->setCanopy(ui->cbCanopy->currentText());
            m_modified = true;
        }

        if(ptrJump->getNote() != ui->teNote->toPlainText())
        {
            ptrJump->setNote(ui->teNote->toPlainText());
            m_modified = true;
        }

    }
    accept();
}
