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

signals:

};

#endif // STRINGLISTPOPUP_H
