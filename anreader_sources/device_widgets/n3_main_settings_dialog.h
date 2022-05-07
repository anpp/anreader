#ifndef N3_MAIN_SETTINGS_DIALOG_H
#define N3_MAIN_SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class N3MainSettingsDialog;
}

class N3MainSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit N3MainSettingsDialog(QWidget *parent = nullptr);
    ~N3MainSettingsDialog();

private:
    Ui::N3MainSettingsDialog *ui;
};

#endif // N3_MAIN_SETTINGS_DIALOG_H
