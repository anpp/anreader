#include "n3jumpeditor.h"
#include "ui_n3jumpeditor.h"

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
}

N3JumpEditor::~N3JumpEditor()
{
    delete ui;
}

void N3JumpEditor::setComboValues(QComboBox &combo, const map_DataList &data_list) const
{
    std::map<QString, QString> tmp_map; //for sorting

    for(auto it = data_list.begin(); it != data_list.end(); ++it)
        tmp_map.emplace(*it);

    for(auto it = tmp_map.begin(); it != tmp_map.end(); ++it)
        combo.addItem((*it).first);
}
