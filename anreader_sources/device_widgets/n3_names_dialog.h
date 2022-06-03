#ifndef N3_NAMES_DIALOG_H
#define N3_NAMES_DIALOG_H

#include <QDialog>

namespace Ui {
class N3NamesDialog;
}

class N3NamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit N3NamesDialog(QWidget *parent = nullptr);
    ~N3NamesDialog();

private:
    Ui::N3NamesDialog *ui;
};

#endif // N3_NAMES_DIALOG_H
