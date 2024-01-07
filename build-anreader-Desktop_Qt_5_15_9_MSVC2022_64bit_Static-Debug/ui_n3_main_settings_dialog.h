/********************************************************************************
** Form generated from reading UI file 'n3_main_settings_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_N3_MAIN_SETTINGS_DIALOG_H
#define UI_N3_MAIN_SETTINGS_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_N3MainSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *gbxUnits;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *cbSpeed;
    QComboBox *cbAlt;
    QComboBox *cbTemp;
    QComboBox *cbDateFormat;
    QComboBox *cbTimeFormat;
    QGroupBox *gbxDisplay;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_6;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *cbCanopyDisp;
    QComboBox *cbAirDisp;
    QComboBox *cbDisplay;
    QGroupBox *gbxOther;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_8;
    QFormLayout *formLayout_3;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *cbLogBook;
    QComboBox *cbCanopyAlarm;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *N3MainSettingsDialog)
    {
        if (N3MainSettingsDialog->objectName().isEmpty())
            N3MainSettingsDialog->setObjectName(QString::fromUtf8("N3MainSettingsDialog"));
        N3MainSettingsDialog->setWindowModality(Qt::WindowModal);
        N3MainSettingsDialog->resize(405, 507);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(N3MainSettingsDialog->sizePolicy().hasHeightForWidth());
        N3MainSettingsDialog->setSizePolicy(sizePolicy);
        N3MainSettingsDialog->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/N3/n3.png"), QSize(), QIcon::Normal, QIcon::Off);
        N3MainSettingsDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(N3MainSettingsDialog);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        widget = new QWidget(N3MainSettingsDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(4, 4, 4, 4);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gbxUnits = new QGroupBox(widget);
        gbxUnits->setObjectName(QString::fromUtf8("gbxUnits"));
        QFont font;
        font.setPointSize(9);
        font.setBold(false);
        font.setWeight(50);
        gbxUnits->setFont(font);
        gbxUnits->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gbxUnits->setFlat(false);
        verticalLayout_5 = new QVBoxLayout(gbxUnits);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(4, 4, 4, 4);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(10);
        formLayout->setContentsMargins(6, 6, 6, 6);
        label = new QLabel(gbxUnits);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 0));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(8);
        font1.setBold(false);
        font1.setWeight(50);
        label->setFont(font1);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(gbxUnits);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(gbxUnits);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(100, 0));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(gbxUnits);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(100, 0));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        label_5 = new QLabel(gbxUnits);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(100, 0));
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        cbSpeed = new QComboBox(gbxUnits);
        cbSpeed->setObjectName(QString::fromUtf8("cbSpeed"));
        cbSpeed->setMinimumSize(QSize(200, 0));
        cbSpeed->setFont(font1);

        formLayout->setWidget(0, QFormLayout::FieldRole, cbSpeed);

        cbAlt = new QComboBox(gbxUnits);
        cbAlt->setObjectName(QString::fromUtf8("cbAlt"));
        cbAlt->setMinimumSize(QSize(200, 0));
        cbAlt->setFont(font1);

        formLayout->setWidget(1, QFormLayout::FieldRole, cbAlt);

        cbTemp = new QComboBox(gbxUnits);
        cbTemp->setObjectName(QString::fromUtf8("cbTemp"));
        cbTemp->setMinimumSize(QSize(200, 0));
        cbTemp->setFont(font1);

        formLayout->setWidget(2, QFormLayout::FieldRole, cbTemp);

        cbDateFormat = new QComboBox(gbxUnits);
        cbDateFormat->setObjectName(QString::fromUtf8("cbDateFormat"));
        cbDateFormat->setMinimumSize(QSize(200, 0));
        cbDateFormat->setFont(font1);

        formLayout->setWidget(3, QFormLayout::FieldRole, cbDateFormat);

        cbTimeFormat = new QComboBox(gbxUnits);
        cbTimeFormat->setObjectName(QString::fromUtf8("cbTimeFormat"));
        cbTimeFormat->setMinimumSize(QSize(200, 0));
        cbTimeFormat->setFont(font1);

        formLayout->setWidget(4, QFormLayout::FieldRole, cbTimeFormat);


        verticalLayout_4->addLayout(formLayout);


        verticalLayout_5->addLayout(verticalLayout_4);


        verticalLayout_2->addWidget(gbxUnits);

        gbxDisplay = new QGroupBox(widget);
        gbxDisplay->setObjectName(QString::fromUtf8("gbxDisplay"));
        gbxDisplay->setFont(font);
        gbxDisplay->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gbxDisplay->setFlat(false);
        verticalLayout_7 = new QVBoxLayout(gbxDisplay);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(4, 4, 4, 4);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setHorizontalSpacing(10);
        formLayout_2->setContentsMargins(6, 6, 6, 6);
        label_6 = new QLabel(gbxDisplay);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(100, 0));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(gbxDisplay);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(100, 0));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(gbxDisplay);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(100, 0));
        label_8->setFont(font1);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_8);

        cbCanopyDisp = new QComboBox(gbxDisplay);
        cbCanopyDisp->setObjectName(QString::fromUtf8("cbCanopyDisp"));
        cbCanopyDisp->setMinimumSize(QSize(200, 0));
        cbCanopyDisp->setFont(font1);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, cbCanopyDisp);

        cbAirDisp = new QComboBox(gbxDisplay);
        cbAirDisp->setObjectName(QString::fromUtf8("cbAirDisp"));
        cbAirDisp->setMinimumSize(QSize(200, 0));
        cbAirDisp->setFont(font1);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, cbAirDisp);

        cbDisplay = new QComboBox(gbxDisplay);
        cbDisplay->setObjectName(QString::fromUtf8("cbDisplay"));
        cbDisplay->setMinimumSize(QSize(200, 0));
        cbDisplay->setFont(font1);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, cbDisplay);


        verticalLayout_6->addLayout(formLayout_2);


        verticalLayout_7->addLayout(verticalLayout_6);


        verticalLayout_2->addWidget(gbxDisplay);

        gbxOther = new QGroupBox(widget);
        gbxOther->setObjectName(QString::fromUtf8("gbxOther"));
        gbxOther->setFont(font);
        gbxOther->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gbxOther->setFlat(false);
        verticalLayout_9 = new QVBoxLayout(gbxOther);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(4, 4, 4, 4);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setHorizontalSpacing(10);
        formLayout_3->setContentsMargins(6, 6, 6, 6);
        label_9 = new QLabel(gbxOther);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(100, 0));
        label_9->setFont(font1);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_9);

        label_10 = new QLabel(gbxOther);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(100, 0));
        label_10->setFont(font1);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_10);

        cbLogBook = new QComboBox(gbxOther);
        cbLogBook->setObjectName(QString::fromUtf8("cbLogBook"));
        cbLogBook->setMinimumSize(QSize(200, 0));
        cbLogBook->setFont(font1);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, cbLogBook);

        cbCanopyAlarm = new QComboBox(gbxOther);
        cbCanopyAlarm->setObjectName(QString::fromUtf8("cbCanopyAlarm"));
        cbCanopyAlarm->setMinimumSize(QSize(200, 0));
        cbCanopyAlarm->setFont(font1);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, cbCanopyAlarm);


        verticalLayout_8->addLayout(formLayout_3);


        verticalLayout_9->addLayout(verticalLayout_8);


        verticalLayout_2->addWidget(gbxOther);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(N3MainSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(N3MainSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), N3MainSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(N3MainSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *N3MainSettingsDialog)
    {
        N3MainSettingsDialog->setWindowTitle(QCoreApplication::translate("N3MainSettingsDialog", "Settings", nullptr));
        gbxUnits->setTitle(QCoreApplication::translate("N3MainSettingsDialog", "Units", nullptr));
        label->setText(QCoreApplication::translate("N3MainSettingsDialog", "Speed", nullptr));
        label_2->setText(QCoreApplication::translate("N3MainSettingsDialog", "Altitude", nullptr));
        label_3->setText(QCoreApplication::translate("N3MainSettingsDialog", "Temperature", nullptr));
        label_4->setText(QCoreApplication::translate("N3MainSettingsDialog", "Date format", nullptr));
        label_5->setText(QCoreApplication::translate("N3MainSettingsDialog", "Time format", nullptr));
        gbxDisplay->setTitle(QCoreApplication::translate("N3MainSettingsDialog", "Display", nullptr));
        label_6->setText(QCoreApplication::translate("N3MainSettingsDialog", "Canopy display", nullptr));
        label_7->setText(QCoreApplication::translate("N3MainSettingsDialog", "Aircraft display", nullptr));
        label_8->setText(QCoreApplication::translate("N3MainSettingsDialog", "Display", nullptr));
        gbxOther->setTitle(QCoreApplication::translate("N3MainSettingsDialog", "Other", nullptr));
        label_9->setText(QCoreApplication::translate("N3MainSettingsDialog", "Logbook", nullptr));
        label_10->setText(QCoreApplication::translate("N3MainSettingsDialog", "Canopy alarm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class N3MainSettingsDialog: public Ui_N3MainSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_N3_MAIN_SETTINGS_DIALOG_H
