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

    m_delegate = std::make_unique<N3NamesDelegate>();

    initNames();
    initAlarms();

    ui->tabWidget->setTabText(0, tr("List"));
    ui->tabWidget->setTabText(1, tr("Alarms settings"));

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
bool N3NamesDialog::isChangedAlarmsSettings() const
{
    if(m_n3names.type() == N3NamesType::Alarms)
        return !(static_cast<N3AlarmsNames&>(*m_new_n3names).settings() == static_cast<const N3AlarmsNames&>(m_n3names).settings());
    return false;
}

//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::initNames()
{
    m_model = std::make_unique<N3NamesModel>(*m_new_n3names);
    m_model->setProperty("model_type", "N3Names");

    ui->tvNames->setModel(m_model.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Active), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Used), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Hidden), m_delegate.get());
    ui->tvNames->setItemDelegateForColumn(static_cast<int>(N3NamesModel_defs::Name), m_delegate.get());

    ui->tvNames->resizeColumnsToContents();
    ui->tvNames->resizeRowsToContents();

    connect(ui->tvNames->model(), &QAbstractItemModel::dataChanged, [this] () { this->dataChanged();});
}

//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::initAlarms()
{
    if(m_n3names.type() == N3NamesType::Alarms)
    {
        N3AlarmsNames& alarms_names = static_cast<N3AlarmsNames&>(*m_new_n3names);

        ui->gbxFreeFall->setChecked(alarms_names.settings().enabledFreeFallAlarms());
        ui->gbxCanopy->setChecked(alarms_names.settings().enabledCanopyAlarms());

        m_alarms_settings_model = std::make_unique<N3AlarmsSettingsModel>(alarms_names);
        m_alarms_settings_freefall_model = std::make_unique<N3FilterAlarmsSettingsModel>(m_alarms_settings_model.get(), static_cast<uint8_t>(N3AlarmsSettings::alarm_type::FreeFall));
        m_alarms_settings_canopy_model = std::make_unique<N3FilterAlarmsSettingsModel>(m_alarms_settings_model.get(), static_cast<uint8_t>(N3AlarmsSettings::alarm_type::Canopy));

        m_alarms_settings_freefall_model->setProperty("model_type", "N3AlarmsSettings");
        m_alarms_settings_canopy_model->setProperty("model_type", "N3AlarmsSettings");

        ui->tvFreefall->setModel(m_alarms_settings_freefall_model.get());
        ui->tvCanopy->setModel(m_alarms_settings_canopy_model.get());

        ui->tvFreefall->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::Active), m_delegate.get());
        ui->tvFreefall->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::NameIndex), m_delegate.get());
        ui->tvFreefall->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude1), m_delegate.get());
        ui->tvFreefall->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude2), m_delegate.get());
        ui->tvFreefall->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude3), m_delegate.get());

        ui->tvCanopy->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::Active), m_delegate.get());
        ui->tvCanopy->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::NameIndex), m_delegate.get());
        ui->tvCanopy->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude1), m_delegate.get());
        ui->tvCanopy->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude2), m_delegate.get());
        ui->tvCanopy->setItemDelegateForColumn(static_cast<int>(N3AlarmsSettings_defs::AlarmAltitude3), m_delegate.get());

        ui->tvFreefall->resizeColumnsToContents();
        ui->tvFreefall->setColumnWidth(static_cast<int>(N3AlarmsSettings_defs::NameIndex), ui->tvFreefall->columnWidth(static_cast<int>(N3AlarmsSettings_defs::NameIndex))  + 50);
        ui->tvFreefall->resizeRowsToContents();
        ui->tvCanopy->resizeColumnsToContents();
        ui->tvCanopy->setColumnWidth(static_cast<int>(N3AlarmsSettings_defs::NameIndex), ui->tvCanopy->columnWidth(static_cast<int>(N3AlarmsSettings_defs::NameIndex))  + 50);
        ui->tvCanopy->resizeRowsToContents();

        ui->gbxFreeFall->setTitle(ui->gbxFreeFall->title() + " (" + tr(alarms_names.settings().alitudePostfix().toStdString().c_str()) + ")");
        ui->gbxCanopy->setTitle(ui->gbxCanopy->title() + " (" + tr(alarms_names.settings().alitudePostfix().toStdString().c_str()) + ")");

        connect(m_alarms_settings_model.get(), &QAbstractItemModel::dataChanged, m_alarms_settings_freefall_model.get(), &QAbstractItemModel::dataChanged);
        connect(m_alarms_settings_model.get(), &QAbstractItemModel::dataChanged, m_alarms_settings_canopy_model.get(), &QAbstractItemModel::dataChanged);
        connect(m_alarms_settings_model.get(), &QAbstractItemModel::dataChanged, [this] () { this->dataChanged();});

        ui->gbxFreeFall->setProperty("alarms_type", "freefall");
        ui->gbxCanopy->setProperty("alarms_type", "canopy");
        connect(ui->gbxFreeFall, &QGroupBox::toggled, this, &N3NamesDialog::enableAlarms);
        connect(ui->gbxCanopy, &QGroupBox::toggled, this, &N3NamesDialog::enableAlarms);
    }
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    if(m_n3names.type() != N3NamesType::Alarms)
        ui->tabWidget->removeTab(1);
#else
    ui->tabWidget->setTabVisible(1, m_n3names.type() == N3NamesType::Alarms);
#endif
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

//--------------------------------------------------------------------------------------------------------------
void N3NamesDialog::enableAlarms(bool enable)
{
    QGroupBox *gbx = static_cast<QGroupBox*>(sender());
    if(nullptr != gbx)
    {
        QVariant alarms_type = gbx->property("alarms_type");
        if(m_n3names.type() == N3NamesType::Alarms && alarms_type.isValid() && !alarms_type.isNull())
        {
            N3AlarmsNames& alarms_names = static_cast<N3AlarmsNames&>(*m_new_n3names);
            if(alarms_type.toString() == "freefall")
            {
                alarms_names.settings().enableFreeFallAlarms(enable);
                dataChanged();
            }
            if(alarms_type.toString() == "canopy")
            {
                alarms_names.settings().enableCanopyAlarms(enable);
                dataChanged();
            }
        }
    }
}
