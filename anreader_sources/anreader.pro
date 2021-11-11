QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport


CONFIG += c++14
QMAKE_CFLAGS += -std=c++14
QMAKE_CXXFLAGS += -std=c++14

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    csvparser.cpp \
    datalist_dialog/datalist_dialog.cpp \
    datalist_dialog/datalistmodel.cpp \
    datalist_dialog/listwidget.cpp \
    datalists.cpp \
    devices_list_widget.cpp \
    log_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    jumpstable.cpp \
    jumpstablemodel.cpp \
    dwidget.cpp \
    n3jumpeditor.cpp \
    n3widget.cpp \
    settings.cpp

HEADERS += \
    common.h \
    csvparser.h \
    datalist_dialog/datalist_dialog.h \
    datalist_dialog/datalistmodel.h \
    datalist_dialog/listwidget.h \
    datalists.h \
    devices_list_widget.h \
    log_widget.h \
    mainwindow.h \
    jumpstable.h \
    jumpstablemodel.h \
    dwidget.h \
    n3jumpeditor.h \
    n3widget.h \
    settings.h

TRANSLATIONS += \
    anreader_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(device/device.pro)

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
    datalist_dialog/datalist_dialog.ui \
    n3jumpeditor.ui

