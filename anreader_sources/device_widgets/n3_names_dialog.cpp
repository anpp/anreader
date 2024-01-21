#include "n3_names_dialog.h"
#include "ui_n3_names_dialog.h"

#include "device/n3names.h"
#include "device/n3alarms_names.h"
#include "device/n3alarms_settings.h"
#include "n3names_model.h"
#include "n3names_delegate.h"
#include "n3alarms_settings_model.h"

#include <QPushButton>

//--------------------------------------------------------------------------------------------------------------
N3NamesDialog::N3NamesDialog(const QString& title, const N3Names& names, QWidget *parent) :
    QDialog(parent),
    m_n3names(names),
    ui(new Ui::N3NamesDialog)
{
    ui->setupUi(this);
    m_title = title;

    m_new_n3names = (m_n3names.type() == N3NamesType::Alarms? std::make_unique<N3AlarmsNames>(): std::make_unique<N3Names>());
    *m_new_n3names = m_n3names; //оператор = перегружен, в m_new_n3names полная копия m_n3names

    if(m_n3names.type() == N3NamesType::Alarms)
    {
        ui->gbxFreeFall->setChecked(static_cast<N3AlarmsNames&>(*m_new_n3names).settings().enabledFreeFallAlarms());
        ui->gbxCanopy->setChecked(static_cast<N3AlarmsNames&>(*m_new_n3names).settings().enabledCanopyAlarms());

        m_alarms_settings_model = std::make_unique<N3AlarmsSettingsModel>(static_cast<N3AlarmsNames&>(*m_new_n3names).settings());
        m_alarms_settings_freefall_model = std::make_unique<N3FilterAlarmsSettingsModel>(m_alarms_settings_model.get(), static_cast<uint8_t>(N3AlarmsSettings::alarm_type::FreeFall));
        m_alarms_settings_canopy_model = std::make_unique<N3FilterAlarmsSettingsModel>(m_alarms_settings_model.get(), static_cast<uint8_t>(N3AlarmsSettings::alarm_type::Canopy));

        ui->tvFreefall->setModel(m_alarms_settings_freefall_model.get());
        ui->tvCanopy->setModel(m_alarms_settings_canopy_model.get());

        ui->tvFreefall->resizeColumnsToContents();
        ui->tvFreefall->resizeRowsToContents();
        ui->tvCanopy->resizeColumnsToContents();
        ui->tvCanopy->resizeRowsToContents();
    }

    m_delegate = std::make_unique<N3NamesDelegate>();

    m_model = std::make_unique<N3NamesModel>(*m_new_n3names);
    ui->tvNames->setModel(m_model.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Active), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Used), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Hidden), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Name), m_delegate.get());

    ui->tvNames->resizeColumnsToContents();
    ui->tvNames->resizeRowsToContents();

    ui->tabWidget->setTabText(0, tr("List"));
    ui->tabWidget->setTabText(1, tr("Alarms settings"));

#if QT_VERSION <= QT_VERSION_CHECK(5, 6, 3)
    if(m_n3names.type() != N3NamesType::Alarms)
        ui->tabWidget->removeTab(1);
#else
    ui->tabWidget->setTabVisible(1, m_n3names.type() == N3NamesType::Alarms);
#endif
    connect(ui->tvNames->model(), &QAbstractItemModel::dataChanged, [&] () { dataChanged();});
    dataChanged();
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
    return (m_new_n3names->map_active() != m_n3names.map_active());
}

//--------------------------------------------------------------------------------------------------------------
bool N3NamesDialog::isChangedData() const
{
    return m_new_n3names->data() != m_n3names.data();
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
