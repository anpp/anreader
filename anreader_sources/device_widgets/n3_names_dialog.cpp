#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

#include "device/n3names.h"
#include "n3names_model.h"
#include "n3names_delegate.h"

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

    m_delegate = std::make_unique<N3NamesDelegate>();

    m_model = std::make_unique<N3NamesModel>(*m_new_n3names);
    ui->tvNames->setModel(m_model.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Active), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Used), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Hidden), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Name), m_delegate.get());
    ui->tvNames->setEditTriggers(QAbstractItemView::AllEditTriggers);

    ui->tvNames->resizeColumnsToContents();
    ui->tvNames->resizeRowsToContents();
}

//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::~N3NamesDialog()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChanged() const
{
    return !(*m_new_n3names == m_n3names);
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChangedCurrentName() const
{
    return (m_new_n3names->active_index_one() != m_n3names.active_index_one());
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChangedData() const
{
    return m_new_n3names->data() != m_n3names.data();
}


//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::on_buttonBox_accepted()
{
    m_new_n3names->calculateCheckSum();
    accept();
}
