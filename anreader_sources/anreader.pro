QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport

CONFIG += c++14

#QMAKE_CXXFLAGS += -pie
#QMAKE_LFLAGS += -pie

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    common/stringlist_popup.cpp \
    csvparser.cpp \
    datalist_dialog/datalist_dialog.cpp \
    datalist_dialog/delegates/combo_popupwidget_delegate.cpp \
    datalist_dialog/delegates/comboeditdelegate.cpp \
    datalist_dialog/models/datalistmodel.cpp \
    datalist_dialog/models/devicedescriptionlistmodel.cpp \
    datalist_dialog/delegates/lineeditdelegate.cpp \
    datalist_dialog/listdevicetypeswidget.cpp \
    datalist_dialog/listwidget.cpp \
    datalists.cpp \
    device_widgets/n3_main_settings_dialog.cpp \
    device_widgets/n3_names_dialog.cpp \
    device_widgets/n3alarms_settings_model.cpp \
    device_widgets/n3names_delegate.cpp \
    device_widgets/n3names_model.cpp \
    devices_list_widget.cpp \
    jumpeditor_dialog/combolist_model.cpp \
    log_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    jumpstable.cpp \
    jumpstablemodel.cpp \
    device_widgets/dwidget.cpp \
    jumpeditor_dialog/n3jumpeditor.cpp \
    device_widgets/n3widget.cpp \
    settings.cpp \
    settings_dialog/settings_com_port.cpp \
    settings_dialog/settings_editor.cpp \
    device_widgets/choice_datetime_dialog.cpp

HEADERS += \
    aboutdialog.h \
    common/common.h \
    common/stringlist_popup.h \
    csvparser.h \
    datalist_dialog/datalist_dialog.h \
    datalist_dialog/delegates/combo_popupwidget_delegate.h \
    datalist_dialog/delegates/comboeditdelegate.h \
    datalist_dialog/models/datalistmodel.h \
    datalist_dialog/models/devicedescriptionlistmodel.h \
    datalist_dialog/delegates/lineeditdelegate.h \
    datalist_dialog/listdevicetypeswidget.h \
    datalist_dialog/listwidget.h \
    datalists.h \
    device_widgets/n3_main_settings_dialog.h \
    device_widgets/n3_names_dialog.h \
    device_widgets/n3alarms_settings_model.h \
    device_widgets/n3names_delegate.h \
    device_widgets/n3names_model.h \
    devices_list_widget.h \
    jumpeditor_dialog/combolist_model.h \
    log_widget.h \
    mainwindow.h \
    jumpstable.h \
    jumpstablemodel.h \
    device_widgets/dwidget.h \
    jumpeditor_dialog/n3jumpeditor.h \
    device_widgets/n3widget.h \
    settings.h \
    settings_dialog/settings_com_port.h \
    settings_dialog/settings_editor.h \
    device_widgets/choice_datetime_dialog.h

TRANSLATIONS += \
    anreader_en.ts \
    anreader_ru.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(device/device.pri)

RESOURCES += \
    images.qrc

win32: RC_ICONS = $$PWD/images/icons/main/anreader.ico

win32:{
    VERSION = 1.0.0
    QMAKE_TARGET_COMPANY =
    QMAKE_TARGET_PRODUCT = anreader
    QMAKE_TARGET_DESCRIPTION = Atlas and Neptune reader utility
    QMAKE_TARGET_COPYRIGHT = Andrey Ponomarenko
}
FORMS += \
    aboutdialog.ui \
    common/string_list.ui \
    datalist_dialog/datalist_dialog.ui \
    device_widgets/n3_main_settings_dialog.ui \
    device_widgets/n3_names_dialog.ui \
    jumpeditor_dialog/n3jumpeditor.ui \
    settings_dialog/settings_com_port.ui \
    device_widgets/choice_datetime_dialog.ui


