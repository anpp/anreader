#include "settings_editor.h"

//-------------------------------------------------------------------------------------------------------------------------------------
SettingsEditor::SettingsEditor(Settings &s, QWidget *parent) : QDialog(parent), settings(s)
{
    this->setWindowTitle(tr("Settings"));
    this->setMinimumSize(default_width, default_height);
    setup();
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
