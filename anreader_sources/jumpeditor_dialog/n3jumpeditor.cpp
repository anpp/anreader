#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

//-------------------------------------------------------------------------------------------------------------------------------------
N3JumpEditor::N3JumpEditor(QWidget *parent, N3Jump& jump, const DataLists& ref_dl) :
    QDialog(parent),
    ui(new Ui::N3JumpEditor),
    m_aircrafts_model(datakind::aircrafts, ref_dl),
    m_dropzones_model(datakind::dropzones, ref_dl),
    m_canopies_model(datakind::canopies, ref_dl)
{
    ui->setupUi(this);

    ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());

    setup_table_view(m_tv_aircrafts, *ui->cbAirplane);
    setup_table_view(m_tv_dropzones, *ui->cbDZ);
    setup_table_view(m_tv_canopies, *ui->cbCanopy);

    ui->cbAirplane->setModel(&m_aircrafts_model);
    ui->cbDZ->setModel(&m_dropzones_model);
    ui->cbCanopy->setModel(&m_canopies_model);

    ui->cbDZ->setCurrentIndex(m_dropzones_model.indexByKey(jump.getDZ()));
    ui->cbAirplane->setCurrentIndex(m_aircrafts_model.indexByKey(jump.getAP()));
    ui->cbCanopy->setCurrentIndex(m_canopies_model.indexByKey(jump.getCanopy()));

    ui->teNote->setPlainText(jump.getNote());

    ptrJump = &jump;
}


//-------------------------------------------------------------------------------------------------------------------------------------
N3JumpEditor::~N3JumpEditor()
{
    delete ui;    
}



//-------------------------------------------------------------------------------------------------------------------------------------
void N3JumpEditor::setup_table_view(QTableView &view, QComboBox& cb)
{
    view.horizontalHeader()->hide();
    view.verticalHeader()->hide();
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setSelectionBehavior(QAbstractItemView::SelectRows);
    view.setCornerWidget(new QSizeGrip(nullptr));
    //view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    view.horizontalHeader()->setStretchLastSection(true);

    cb.setModelColumn(1);
    cb.setView(&view);
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
