/********************************************************************************
** Form generated from reading UI file 'settings_com_port.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_COM_PORT_H
#define UI_SETTINGS_COM_PORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_COMPortWidget
{
public:
    QGroupBox *parametersBox;
    QGridLayout *gridLayout_2;
    QLabel *baudRateLabel;
    QComboBox *baudRateBox;
    QLabel *dataBitsLabel;
    QComboBox *dataBitsBox;
    QLabel *parityLabel;
    QComboBox *parityBox;
    QLabel *stopBitsLabel;
    QComboBox *stopBitsBox;
    QLabel *flowControlLabel;
    QComboBox *flowControlBox;

    void setupUi(QWidget *COMPortWidget)
    {
        if (COMPortWidget->objectName().isEmpty())
            COMPortWidget->setObjectName(QString::fromUtf8("COMPortWidget"));
        COMPortWidget->resize(390, 373);
        parametersBox = new QGroupBox(COMPortWidget);
        parametersBox->setObjectName(QString::fromUtf8("parametersBox"));
        parametersBox->setGeometry(QRect(10, 10, 271, 241));
        gridLayout_2 = new QGridLayout(parametersBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        baudRateLabel = new QLabel(parametersBox);
        baudRateLabel->setObjectName(QString::fromUtf8("baudRateLabel"));

        gridLayout_2->addWidget(baudRateLabel, 0, 0, 1, 1);

        baudRateBox = new QComboBox(parametersBox);
        baudRateBox->setObjectName(QString::fromUtf8("baudRateBox"));

        gridLayout_2->addWidget(baudRateBox, 0, 1, 1, 1);

        dataBitsLabel = new QLabel(parametersBox);
        dataBitsLabel->setObjectName(QString::fromUtf8("dataBitsLabel"));

        gridLayout_2->addWidget(dataBitsLabel, 1, 0, 1, 1);

        dataBitsBox = new QComboBox(parametersBox);
        dataBitsBox->setObjectName(QString::fromUtf8("dataBitsBox"));

        gridLayout_2->addWidget(dataBitsBox, 1, 1, 1, 1);

        parityLabel = new QLabel(parametersBox);
        parityLabel->setObjectName(QString::fromUtf8("parityLabel"));

        gridLayout_2->addWidget(parityLabel, 2, 0, 1, 1);

        parityBox = new QComboBox(parametersBox);
        parityBox->setObjectName(QString::fromUtf8("parityBox"));

        gridLayout_2->addWidget(parityBox, 2, 1, 1, 1);

        stopBitsLabel = new QLabel(parametersBox);
        stopBitsLabel->setObjectName(QString::fromUtf8("stopBitsLabel"));

        gridLayout_2->addWidget(stopBitsLabel, 3, 0, 1, 1);

        stopBitsBox = new QComboBox(parametersBox);
        stopBitsBox->setObjectName(QString::fromUtf8("stopBitsBox"));

        gridLayout_2->addWidget(stopBitsBox, 3, 1, 1, 1);

        flowControlLabel = new QLabel(parametersBox);
        flowControlLabel->setObjectName(QString::fromUtf8("flowControlLabel"));

        gridLayout_2->addWidget(flowControlLabel, 4, 0, 1, 1);

        flowControlBox = new QComboBox(parametersBox);
        flowControlBox->setObjectName(QString::fromUtf8("flowControlBox"));

        gridLayout_2->addWidget(flowControlBox, 4, 1, 1, 1);


        retranslateUi(COMPortWidget);

        QMetaObject::connectSlotsByName(COMPortWidget);
    } // setupUi

    void retranslateUi(QWidget *COMPortWidget)
    {
        COMPortWidget->setWindowTitle(QCoreApplication::translate("COMPortWidget", "Form", nullptr));
        parametersBox->setTitle(QCoreApplication::translate("COMPortWidget", "Select Parameters", nullptr));
        baudRateLabel->setText(QCoreApplication::translate("COMPortWidget", "BaudRate:", nullptr));
        dataBitsLabel->setText(QCoreApplication::translate("COMPortWidget", "Data bits:", nullptr));
        parityLabel->setText(QCoreApplication::translate("COMPortWidget", "Parity:", nullptr));
        stopBitsLabel->setText(QCoreApplication::translate("COMPortWidget", "Stop bits:", nullptr));
        flowControlLabel->setText(QCoreApplication::translate("COMPortWidget", "Flow control:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class COMPortWidget: public Ui_COMPortWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_COM_PORT_H
