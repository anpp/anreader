#include "settings_editor.h"

//-------------------------------------------------------------------------------------------------------------------------------------
SettingsEditor::SettingsEditor(QWidget *parent) : QDialog(parent)
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

    tw.addTab(&com_port_widget, "COM port");
    tw.setTabsClosable(false);
    tw.setDocumentMode(false);
    tw.setMovable(false);

    layout->addWidget(&tw);
    layout->addWidget(&buttonBox);
    this->setLayout(layout);
}
