#include "stringlist_popup.h"


//---------------------------------------------------------------------------------------------------------------------------
StringListPopup::StringListPopup(const QString& strings, QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setValue(strings);    
}


//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::setValue(const QString &avalue)
{
    btnDelete->setEnabled(false);
    m_strings = avalue.split(";");
    listWidget->clear();
    listWidget->addItems(m_strings);

    for(auto i = 0; i < listWidget->count(); ++ i)
        listWidget->item(i)->setFlags(listWidget->item(i)->flags() | Qt::ItemIsEditable);
}


//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::on_buttonBox_accepted()
{
    m_strings.clear();
    for(auto i = 0; i < listWidget->count(); ++ i)
        if(!listWidget->item(i)->text().trimmed().isEmpty())
            m_strings << listWidget->item(i)->text();

    emit finish(true);
}


//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::on_buttonBox_rejected()
{
    emit finish(false);
}


//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::on_btnAdd_clicked()
{
    listWidget->addItem("");
    QListWidgetItem *item = listWidget->item(listWidget->count() - 1);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    listWidget->setCurrentRow(listWidget->count() - 1);
}

//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::on_btnDelete_clicked()
{
    if(listWidget->currentRow() >= 0 && listWidget->currentRow() < listWidget->count())
    {
        QListWidgetItem *item = listWidget->takeItem(listWidget->currentRow());
        delete item;
    }
}

//---------------------------------------------------------------------------------------------------------------------------
void StringListPopup::on_listWidget_currentRowChanged(int currentRow)
{
    btnDelete->setEnabled(currentRow >= 0 && currentRow < listWidget->count());
}
