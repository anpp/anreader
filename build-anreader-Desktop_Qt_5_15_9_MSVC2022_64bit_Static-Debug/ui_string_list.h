/********************************************************************************
** Form generated from reading UI file 'string_list.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRING_LIST_H
#define UI_STRING_LIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_StringList
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *mainLayout;
    QFrame *centralFrame;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QFrame *buttonFrame;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StringList)
    {
        if (StringList->objectName().isEmpty())
            StringList->setObjectName(QString::fromUtf8("StringList"));
        StringList->resize(341, 275);
        verticalLayout_2 = new QVBoxLayout(StringList);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(4, 6, 4, 6);
        mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(4);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        centralFrame = new QFrame(StringList);
        centralFrame->setObjectName(QString::fromUtf8("centralFrame"));
        centralFrame->setFrameShape(QFrame::NoFrame);
        centralFrame->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(centralFrame);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(centralFrame);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);

        horizontalLayout->addWidget(listWidget);

        buttonFrame = new QFrame(centralFrame);
        buttonFrame->setObjectName(QString::fromUtf8("buttonFrame"));
        buttonFrame->setMinimumSize(QSize(80, 0));
        buttonFrame->setFrameShape(QFrame::StyledPanel);
        buttonFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(buttonFrame);
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btnAdd = new QPushButton(buttonFrame);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

        verticalLayout->addWidget(btnAdd);

        btnDelete = new QPushButton(buttonFrame);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));

        verticalLayout->addWidget(btnDelete);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout);


        horizontalLayout->addWidget(buttonFrame);


        horizontalLayout_2->addLayout(horizontalLayout);


        mainLayout->addWidget(centralFrame);

        buttonBox = new QDialogButtonBox(StringList);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        mainLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(mainLayout);


        retranslateUi(StringList);

        QMetaObject::connectSlotsByName(StringList);
    } // setupUi

    void retranslateUi(QDialog *StringList)
    {
        StringList->setWindowTitle(QCoreApplication::translate("StringList", "Dialog", nullptr));
        btnAdd->setText(QCoreApplication::translate("StringList", "Add", nullptr));
        btnDelete->setText(QCoreApplication::translate("StringList", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StringList: public Ui_StringList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRING_LIST_H
