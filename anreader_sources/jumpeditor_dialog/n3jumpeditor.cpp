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

    //ui->cbxDeleted->setAttribute(Qt::WA_TransparentForMouseEvents);
    //ui->cbxDeleted->setFocusPolicy(Qt::NoFocus);

    ui->spnNumber->setValue(jump.getJumpNumber());
    ui->deDate->setDateTime(jump.getJumpDate());
    ui->cbxDeleted->setChecked(jump.isDeleted());

    setup_combobox(datakind::aircrafts, m_aircrafts_model, m_tv_aircrafts);
    setup_combobox(datakind::dropzones, m_dropzones_model, m_tv_dropzones);
    setup_combobox(datakind::canopies, m_canopies_model, m_tv_canopies);

    ui->cbDZ->setCurrentIndex(m_dropzones_model.indexByKey(jump.getDZ()));
    ui->cbAC->setCurrentIndex(m_aircrafts_model.indexByKey(jump.getAC()));
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
void N3JumpEditor::setup_combobox(const datakind dk, QAbstractItemModel &model, QTableView &view)
{    
    QComboBox& cb = (datakind::aircrafts == dk ? *ui->cbAC : (datakind::dropzones == dk ? *ui->cbDZ : *ui->cbCanopy));

    cb.setModel(&model);
    cb.setProperty("datakind", QVariant(static_cast<int>(dk)));
    connect(&cb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &N3JumpEditor::combo_changeindex);
    cb.setModelColumn(1);
    cb.setView(&view);

    view.horizontalHeader()->hide();
    view.verticalHeader()->hide();
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setSelectionBehavior(QAbstractItemView::SelectRows);
    view.setCornerWidget(new QSizeGrip(nullptr));
    view.resizeColumnsToContents();
    view.resizeRowsToContents();
    view.horizontalHeader()->setStretchLastSection(true);
}


//-------------------------------------------------------------------------------------------------------------------------------------
void N3JumpEditor::combo_changeindex(int index)
{
    datakind dk = static_cast<datakind>((QObject::sender()->property("datakind").toInt()));
    const Combolist_model *model = dynamic_cast<Combolist_model*>((dynamic_cast<QComboBox*>(QObject::sender()))->model());

    if(datakind::aircrafts == dk) m_aircraft_key = model->keyByIndex(index);
    if(datakind::dropzones == dk) m_dropzone_key = model->keyByIndex(index);
    if(datakind::canopies == dk) m_canopy_key = model->keyByIndex(index);
}



//-------------------------------------------------------------------------------------------------------------------------------------
void N3JumpEditor::on_buttonBox_accepted()
{
    if(nullptr != ptrJump)
    {

        m_modified = (
                       (ptrJump->getJumpNumber() != static_cast<uint>(ui->spnNumber->value()))
                    || (ptrJump->isDeleted() != (ui->cbxDeleted->checkState() == Qt::CheckState::Checked))
                    || (ptrJump->getJumpDate() != ui->deDate->dateTime())
                    || (ptrJump->getAC() != m_aircraft_key)
                    || (ptrJump->getDZ() != m_dropzone_key)
                    || (ptrJump->getCanopy() != m_canopy_key)
                    || (ptrJump->getNote() != ui->teNote->toPlainText())
                );

        if(isModified())
        {
            ptrJump->setJumpNumber(ui->spnNumber->value());
            ptrJump->setDeleted((ui->cbxDeleted->checkState() == Qt::CheckState::Checked));
            ptrJump->setJumpDate(ui->deDate->dateTime());
            ptrJump->setAC(m_aircraft_key);
            ptrJump->setDZ(m_dropzone_key);
            ptrJump->setCanopy(m_canopy_key);
            ptrJump->setNote(ui->teNote->toPlainText());
        }

    }
    accept();
}
