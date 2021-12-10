#include "stringlist_popup.h"

StringListPopup::StringListPopup(const QString& strings, QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    m_strings = strings.split(";");
    listWidget->addItems(m_strings);
}
