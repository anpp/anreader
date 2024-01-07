/********************************************************************************
** Form generated from reading UI file 'n3jumpeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_N3JUMPEDITOR_H
#define UI_N3JUMPEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_N3JumpEditor
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QFrame *editFrame;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *lbAircraft;
    QCheckBox *cbxDeleted;
    QLabel *label;
    QComboBox *cbDZ;
    QComboBox *cbAC;
    QLabel *label_3;
    QLabel *label_6;
    QComboBox *cbCanopy;
    QLabel *label_2;
    QLabel *label_5;
    QSpinBox *spnNumber;
    QDateTimeEdit *deDate;
    QLabel *label_7;
    QPlainTextEdit *teNote;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *N3JumpEditor)
    {
        if (N3JumpEditor->objectName().isEmpty())
            N3JumpEditor->setObjectName(QString::fromUtf8("N3JumpEditor"));
        N3JumpEditor->setWindowModality(Qt::WindowModal);
        N3JumpEditor->resize(482, 462);
        N3JumpEditor->setModal(false);
        verticalLayout_2 = new QVBoxLayout(N3JumpEditor);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(4, 4, 4, 4);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editFrame = new QFrame(N3JumpEditor);
        editFrame->setObjectName(QString::fromUtf8("editFrame"));
        editFrame->setFrameShape(QFrame::Panel);
        editFrame->setFrameShadow(QFrame::Sunken);
        editFrame->setLineWidth(0);
        gridLayout_2 = new QGridLayout(editFrame);
        gridLayout_2->setSpacing(8);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lbAircraft = new QLabel(editFrame);
        lbAircraft->setObjectName(QString::fromUtf8("lbAircraft"));

        gridLayout->addWidget(lbAircraft, 3, 0, 1, 1);

        cbxDeleted = new QCheckBox(editFrame);
        cbxDeleted->setObjectName(QString::fromUtf8("cbxDeleted"));
        cbxDeleted->setEnabled(true);
        cbxDeleted->setCheckable(true);
        cbxDeleted->setChecked(false);

        gridLayout->addWidget(cbxDeleted, 5, 1, 1, 1);

        label = new QLabel(editFrame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cbDZ = new QComboBox(editFrame);
        cbDZ->setObjectName(QString::fromUtf8("cbDZ"));
        cbDZ->setStyleSheet(QString::fromUtf8(""));
        cbDZ->setFrame(true);

        gridLayout->addWidget(cbDZ, 2, 1, 1, 1);

        cbAC = new QComboBox(editFrame);
        cbAC->setObjectName(QString::fromUtf8("cbAC"));

        gridLayout->addWidget(cbAC, 3, 1, 1, 1);

        label_3 = new QLabel(editFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_6 = new QLabel(editFrame);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        cbCanopy = new QComboBox(editFrame);
        cbCanopy->setObjectName(QString::fromUtf8("cbCanopy"));

        gridLayout->addWidget(cbCanopy, 4, 1, 1, 1);

        label_2 = new QLabel(editFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_5 = new QLabel(editFrame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        spnNumber = new QSpinBox(editFrame);
        spnNumber->setObjectName(QString::fromUtf8("spnNumber"));
        spnNumber->setEnabled(true);
        spnNumber->setReadOnly(false);
        spnNumber->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        spnNumber->setMaximum(999999999);

        gridLayout->addWidget(spnNumber, 0, 1, 1, 1);

        deDate = new QDateTimeEdit(editFrame);
        deDate->setObjectName(QString::fromUtf8("deDate"));
        deDate->setEnabled(true);
        deDate->setReadOnly(false);
        deDate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        deDate->setCalendarPopup(true);

        gridLayout->addWidget(deDate, 1, 1, 1, 1);

        label_7 = new QLabel(editFrame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        teNote = new QPlainTextEdit(editFrame);
        teNote->setObjectName(QString::fromUtf8("teNote"));

        gridLayout->addWidget(teNote, 6, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(editFrame);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(N3JumpEditor);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(N3JumpEditor);
        QObject::connect(buttonBox, SIGNAL(rejected()), N3JumpEditor, SLOT(reject()));

        QMetaObject::connectSlotsByName(N3JumpEditor);
    } // setupUi

    void retranslateUi(QDialog *N3JumpEditor)
    {
        N3JumpEditor->setWindowTitle(QCoreApplication::translate("N3JumpEditor", "Jump edit", nullptr));
        lbAircraft->setText(QCoreApplication::translate("N3JumpEditor", "Aircraft", nullptr));
        cbxDeleted->setText(QString());
        label->setText(QCoreApplication::translate("N3JumpEditor", "Number", nullptr));
        label_3->setText(QCoreApplication::translate("N3JumpEditor", "DZ", nullptr));
        label_6->setText(QCoreApplication::translate("N3JumpEditor", "Deleted", nullptr));
        label_2->setText(QCoreApplication::translate("N3JumpEditor", "Date", nullptr));
        label_5->setText(QCoreApplication::translate("N3JumpEditor", "Canopy", nullptr));
        label_7->setText(QCoreApplication::translate("N3JumpEditor", "Note", nullptr));
    } // retranslateUi

};

namespace Ui {
    class N3JumpEditor: public Ui_N3JumpEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_N3JUMPEDITOR_H
