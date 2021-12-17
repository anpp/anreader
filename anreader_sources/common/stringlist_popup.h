#ifndef STRINGLISTPOPUP_H
#define STRINGLISTPOPUP_H

#include <QDialog>
#include <QStringList>
#include "common.h"
#include "ui_string_list.h"


class StringListPopup : public QDialog,
                        protected Ui::StringList
{
    Q_OBJECT

    QStringList m_strings;
public:
    explicit StringListPopup(const QString& strings, QWidget *parent = nullptr);
    ~StringListPopup() {}

    const QString value() const { return m_strings.join(";"); }
    void setValue(const QString& avalue);

signals:
    void finish(bool);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
};

#endif // STRINGLISTPOPUP_H
