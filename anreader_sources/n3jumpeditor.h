#ifndef N3JUMPEDITOR_H
#define N3JUMPEDITOR_H

#include <QDialog>
#include <memory>
#include <QComboBox>

#include "common.h"
#include "device/jumps/n3jump.h"

namespace Ui {
class N3JumpEditor;
}

class N3JumpEditor : public QDialog
{
    Q_OBJECT

public:
    explicit N3JumpEditor(QWidget *parent, N3Jump& jump, const map_DataList& aircrafts, const map_DataList& dropzonesconst, const map_DataList& canopies);
    ~N3JumpEditor();

    bool isModified() const {return m_modified; }

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::N3JumpEditor *ui;
    N3Jump *ptrJump = nullptr;
    bool m_modified = false;

    void setComboValues(QComboBox& combo, const map_DataList& data_list) const;
};

#endif // N3JUMPEDITOR_H
