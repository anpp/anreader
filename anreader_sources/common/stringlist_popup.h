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
    void accepted(bool);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // STRINGLISTPOPUP_H
