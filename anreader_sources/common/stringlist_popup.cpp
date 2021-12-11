#include "stringlist_popup.h"

StringListPopup::StringListPopup(const QString& strings, QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setValue(strings);
}

void StringListPopup::setValue(const QString &avalue)
{
    m_strings = avalue.split(";");
    listWidget->clear();
    listWidget->addItems(m_strings);
}

void StringListPopup::on_buttonBox_accepted()
{
    emit accepted(true);
}

void StringListPopup::on_buttonBox_rejected()
{
    emit accepted(false);
}
