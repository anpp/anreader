#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

#include "device/n3names.h"

N3NamesDialog::N3NamesDialog(const QString& title, const N3Names& names, int aindex, int aindex2, QWidget *parent) :
    QDialog(parent),
    m_n3names(names),
    m_active_index(aindex),
    m_active_index2(aindex2),
    ui(new Ui::N3NamesDialog)
{
    ui->setupUi(this);
    m_title = title;
    this->setWindowTitle(m_title);

    raw_names = m_n3names.data();
    m_new_n3names = std::make_unique<N3Names>(raw_names);

    m_new_active_index = m_active_index;
    m_new_active_index2 = m_active_index2;
}

N3NamesDialog::~N3NamesDialog()
{
    delete ui;
}
