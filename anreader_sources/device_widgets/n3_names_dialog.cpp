#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

#include "device/n3names.h"
#include "n3names_model.h"

//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::N3NamesDialog(const QString& title, const N3Names& names, QWidget *parent) :
    QDialog(parent),
    m_n3names(names),
    ui(new Ui::N3NamesDialog)
{
    ui->setupUi(this);
    m_title = title;
    this->setWindowTitle(m_title);

    raw_names = m_n3names.data();
    m_new_n3names = std::make_unique<N3Names>(raw_names);
    *m_new_n3names = m_n3names; //оператор = перегружен, в m_new_n3names полная копия m_n3names (raw_names отдельно копируется, на него ссылка)

    m_model = std::make_unique<N3NamesModel>(*m_new_n3names);
    ui->tvNames->setModel(m_model.get());
    ui->tvNames->resizeColumnsToContents();
    ui->tvNames->resizeRowsToContents();
}

//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::~N3NamesDialog()
{
    delete ui;
}
