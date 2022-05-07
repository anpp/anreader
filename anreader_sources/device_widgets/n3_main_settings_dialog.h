#ifndef N3_MAIN_SETTINGS_DIALOG_H
#define N3_MAIN_SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class N3MainSettingsDialog;
}

class N3DeviceSettings;

class N3MainSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit N3MainSettingsDialog(const N3DeviceSettings& n3settings, QWidget *parent = nullptr);
    ~N3MainSettingsDialog();

private:    
    Ui::N3MainSettingsDialog *ui;
    const N3DeviceSettings& m_n3settings;

    void fillComboBoxes();
    void initComboBoxes();
};

#endif // N3_MAIN_SETTINGS_DIALOG_H
