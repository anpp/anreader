#ifndef CHOICE_DATETIME_DIALOG_H
#define CHOICE_DATETIME_DIALOG_H

#include <QDialog>
#include <QDateTime>


namespace Ui {
class ChoiceDateTimeDialog;
}

class ChoiceDateTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceDateTimeDialog(const QDateTime& dt, QWidget *parent = nullptr);
    ~ChoiceDateTimeDialog();

    QDateTime datetime() const;

private slots:
    void on_tbCurrentTime_clicked();

private:
    Ui::ChoiceDateTimeDialog *ui;
};

#endif // CHOICE_DATETIME_DIALOG_H
