#ifndef N3JUMPEDITOR_H
#define N3JUMPEDITOR_H

#include <QDialog>
#include <memory>
#include <QComboBox>
#include <QTableView>
#include <QSizeGrip>

#include "common/common.h"
#include "device/jumps/n3jump.h"
#include "datalists.h"
#include "combolist_model.h"

namespace Ui {
class N3JumpEditor;
}

class N3JumpEditor : public QDialog
{
    Q_OBJECT

public:
    explicit N3JumpEditor(QWidget *parent, N3Jump& jump, const DataLists& ref_dl);
    ~N3JumpEditor();

    bool isModified() const {return m_modified; }

private slots:
    void on_buttonBox_accepted();
    void combo_changeindex(int index);

private:
    Ui::N3JumpEditor *ui;
    N3Jump *ptrJump = nullptr;
    bool m_modified = false;

    Combolist_model m_aircrafts_model;
    Combolist_model m_dropzones_model;
    Combolist_model m_canopies_model;

    QString m_aircraft_key;
    QString m_dropzone_key;
    QString m_canopy_key;


    QTableView m_tv_aircrafts;
    QTableView m_tv_dropzones;
    QTableView m_tv_canopies;

    void setup_combobox(const datakind dk, QAbstractItemModel &model, QTableView &view);

};

#endif // N3JUMPEDITOR_H
