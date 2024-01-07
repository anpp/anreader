#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

#include "device/n3names.h"
#include "device/n3alarms_names.h"
#include "device/n3alarms_settings.h"
#include "n3names_model.h"
#include "n3names_delegate.h"

#include <QPushButton>
#include <QDebug>
//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::N3NamesDialog(const QString& title, const N3Names& names, QWidget *parent) :
    QDialog(parent),
    m_n3names(names),
    ui(new Ui::N3NamesDialog)
{
    ui->setupUi(this);
    m_title = title;

    if(m_n3names.type() == N3NamesType::Alarms)
    {
        m_new_n3names = std::make_unique<N3AlarmsNames>();
        static_cast<N3AlarmsNames&>(*m_new_n3names) = static_cast<const N3AlarmsNames&>(m_n3names);

        qDebug() << static_cast<N3AlarmsNames&>(*m_new_n3names).settings().altitude(0, 0);
        qDebug() << static_cast<N3AlarmsNames&>(*m_new_n3names).settings().altitude(0, 1);
        qDebug() << static_cast<N3AlarmsNames&>(*m_new_n3names).settings().altitude(0, 2);
    }
    else
    {
        m_new_n3names = std::make_unique<N3Names>();
        *m_new_n3names = m_n3names; //оператор = перегружен, в m_new_n3names полная копия m_n3names
    }

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

    ui->tabWidget->setTabText(0, tr("List"));
    ui->tabWidget->setTabText(1, tr("Alarms settings"));
    ui->tabWidget->setTabVisible(1, names.type() == N3NamesType::Alarms);

    connect(ui->tvNames->model(), &QAbstractItemModel::dataChanged, [&] () { dataChanged();});
    dataChanged();
}

//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::~N3NamesDialog()
{
    delete ui;
}
#include <QDebug>
//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChanged() const
{
    //qDebug() << m_new_n3names->data();
    //qDebug() << m_n3names.data_const();

    if(m_n3names.type() == N3NamesType::Alarms)
        return !(static_cast<const N3AlarmsNames&>(*m_new_n3names) == static_cast<const N3AlarmsNames&>(m_n3names));
    return !(*m_new_n3names == m_n3names);
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChangedCurrentName() const
{
    return (m_new_n3names->map_active() != m_n3names.map_active());
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChangedData() const
{
    return m_new_n3names->data() != m_n3names.data_const();
}

//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::dataChanged()
{

    bool is_changed = isChanged();
    ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(is_changed);
    setWindowTitle(tr(m_title.toStdString().c_str()) + (is_changed ? "*" : ""));
}


//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::on_buttonBox_accepted()
{
    m_new_n3names->calculateCheckSum();
    accept();
}
