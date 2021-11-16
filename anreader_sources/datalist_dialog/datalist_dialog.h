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
    explicit DataList_Dialog(const QString& Title, t_datalist& datalist, QWidget *parent = nullptr);
    ~DataList_Dialog();

    const t_datalist& datalist() const {return m_datalist; }

private slots:
    void on_buttonBox_accepted();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void selectedItemUsed(bool value);

private:
    Ui::DataList_Dialog *ui;
    ListWidget m_listWidget;
    const t_datalist& m_datalist;
};

#endif // DATALIST_DIALOG_H
