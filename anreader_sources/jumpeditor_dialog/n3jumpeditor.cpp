#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

//-------------------------------------------------------------------------------------------------------------------------------------
N3JumpEditor::N3JumpEditor(QWidget *parent, N3Jump& jump, const DataLists& ref_dl) :
    QDialog(parent),
    ui(new Ui::N3JumpEditor),
    m_aircrafts_model(ref_dl.const_aircrafts()),
    m_dropzones_model(ref_dl.const_dropzones()),
    m_canopies_model(ref_dl.const_canopies())
{
    ui->setupUi(this);

    ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());

    setup_table_view(m_tv_aircrafts);
    setup_table_view(m_tv_dropzones);
    setup_table_view(m_tv_canopies);

    ui->cbAirplane->setModel(&m_aircrafts_model);
    ui->cbDZ->setModel(&m_dropzones_model);
    ui->cbCanopy->setModel(&m_canopies_model);


    ui->cbAirplane->setView(&m_tv_aircrafts);
    ui->cbDZ->setView(&m_tv_dropzones);
    ui->cbCanopy->setView(&m_tv_canopies);


    //setComboValues(*ui->cbAirplane, ref_dl.const_aircrafts());
    //setComboValues(*ui->cbDZ, ref_dl.const_dropzones());
    //setComboValues(*ui->cbCanopy, ref_dl.const_canopies());

    ui->cbDZ->setCurrentText(jump.getDZ() + " " +ref_dl.mappedValue(datakind::dropzones, jump.getDZ()));
    ui->cbAirplane->setCurrentText(jump.getAP() + " " +ref_dl.mappedValue(datakind::aircrafts, jump.getAP()));
    ui->cbCanopy->setCurrentText(jump.getCanopy() + " " +ref_dl.mappedValue(datakind::canopies, jump.getCanopy()));

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
void N3JumpEditor::setup_table_view(QTableView &view)
{
    view.horizontalHeader()->hide();
    view.verticalHeader()->hide();
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setSelectionBehavior(QAbstractItemView::SelectRows);
    view.setCornerWidget(new QSizeGrip(nullptr));
    //view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    view.horizontalHeader()->setStretchLastSection(true);
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
