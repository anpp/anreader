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

    const N3DeviceSettings& new_settings() const { return *m_new_n3settings; }
    bool isChanged() const;

private:    
    Ui::N3MainSettingsDialog *ui;
    const N3DeviceSettings& m_n3settings;
    std::unique_ptr<N3DeviceSettings> m_new_n3settings;
    QByteArray raw_settings;
    QString m_title;

    void fillComboBoxes();
    void initComboBoxes();

private slots:
    void combo_changeindex(int index);

};

#endif // N3_MAIN_SETTINGS_DIALOG_H
