#ifndef N3JUMPEDITOR_H
#define N3JUMPEDITOR_H

#include <QDialog>
#include <memory>

#include "common.h"
#include "device/jumps/n3jump.h"

namespace Ui {
class N3JumpEditor;
}

class N3JumpEditor : public QDialog
{
    Q_OBJECT

public:
    explicit N3JumpEditor(QWidget *parent, N3Jump& jump, const map_APs& aircrafts, const map_DZs& dropzones);
    ~N3JumpEditor();

private:
    Ui::N3JumpEditor *ui;
};

#endif // N3JUMPEDITOR_H
