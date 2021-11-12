#ifndef DATALIST_DIALOG_H
#define DATALIST_DIALOG_H

#include <QDialog>
#include "listwidget.h"

namespace Ui {
class DataList_Dialog;
}

class DataList_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataList_Dialog(const QString& Title, QWidget *parent = nullptr);
    ~DataList_Dialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DataList_Dialog *ui;
    ListWidget m_listWidget;
};

#endif // DATALIST_DIALOG_H
