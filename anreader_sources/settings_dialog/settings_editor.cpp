#include "settings_editor.h"
#include <QPushButton>

//-------------------------------------------------------------------------------------------------------------------------------------
SettingsEditor::SettingsEditor(Settings &s, QWidget *parent) : QDialog(parent), settings(s)
{
    this->setWindowTitle(tr("Settings"));
    this->setMinimumSize(default_width, default_height);
    setup();

    connect(&buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(&buttonBox, &QDialogButtonBox::accepted, this, &SettingsEditor::accept_slot);
    connect(&buttonBox, &QDialogButtonBox::clicked, this, &SettingsEditor::clicked_slot);
}


//-------------------------------------------------------------------------------------------------------------------------------------
void SettingsEditor::accept_slot()
{
    applySettings();
    accept();
}

//-------------------------------------------------------------------------------------------------------------------------------------
void SettingsEditor::clicked_slot(QAbstractButton* button)
{
    if(buttonBox.standardButton(button) == QDialogButtonBox::Apply)
        applySettings();
}



//-------------------------------------------------------------------------------------------------------------------------------------
void SettingsEditor::setup()
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(margin);

    buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok | QDialogButtonBox::Apply);

    com_port_widget.setCurrentSettings(settings.COMSettings());

    tw.addTab(&com_port_widget, "COM port");
    tw.setTabsClosable(false);
    tw.setDocumentMode(false);
    tw.setMovable(false);

    layout->addWidget(&tw);
    layout->addWidget(&buttonBox);
    this->setLayout(layout);
}

//-------------------------------------------------------------------------------------------------------------------------------------
void SettingsEditor::applySettings()
{
    settings.setCOMSettings(com_port_widget.actualSettings());
    settings.saveSettingsByKind(kindset::com_port);
}
