#ifndef SETTINGSEDITOR_H
#define SETTINGSEDITOR_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QTabWidget>

#include "common/common.h"
#include "settings_com_port.h"
#include "../settings.h"

class SettingsEditor : public QDialog
{
    Q_OBJECT   
    enum SettingsEditor_defs: int {default_width = 500, default_height = 400, margin = 4};

    void setup();
    void applySettings();

    QDialogButtonBox buttonBox;
    QTabWidget tw;
    SettingsCOMPort com_port_widget;

    Settings &settings;
public:    
    explicit SettingsEditor(Settings &s, QWidget *parent = nullptr);

private slots:
    void accept_slot();
    void clicked_slot(QAbstractButton*);
};

#endif // SETTINGSEDITOR_H
