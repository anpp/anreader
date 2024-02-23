#ifndef N3_NAMES_DIALOG_H
#define N3_NAMES_DIALOG_H

#include <QDialog>
#include <memory>


namespace Ui {
class N3NamesDialog;
}

class N3Names;
class N3NamesModel;
class N3NamesDelegate;
class N3AlarmsSettingsModel;
class N3FilterAlarmsSettingsModel;

class N3NamesDialog : public QDialog
{
    Q_OBJECT

    const N3Names& m_n3names;
    std::unique_ptr<N3Names> m_new_n3names;
    QString m_title;
    std::unique_ptr<N3NamesModel> m_model;
    std::unique_ptr<N3NamesDelegate> m_delegate;

    std::unique_ptr<N3AlarmsSettingsModel> m_alarms_settings_model;
    std::unique_ptr<N3FilterAlarmsSettingsModel> m_alarms_settings_freefall_model;
    std::unique_ptr<N3FilterAlarmsSettingsModel> m_alarms_settings_canopy_model;

public:
    explicit N3NamesDialog(const QString& title, const N3Names& names,  QWidget *parent = nullptr);
    ~N3NamesDialog();

    bool isChanged() const;
    bool isChangedCurrentName() const;
    bool isChangedData() const;
    const N3Names& new_n3names() const { return *m_new_n3names; }

private:
    Ui::N3NamesDialog *ui;

    void initNames();
    void initAlarms();

private slots:
    void dataChanged();
    void on_buttonBox_accepted();
    void enableAlarms(bool enable);
};

#endif // N3_NAMES_DIALOG_H
