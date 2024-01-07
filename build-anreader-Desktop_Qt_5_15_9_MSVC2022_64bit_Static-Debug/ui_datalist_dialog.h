/********************************************************************************
** Form generated from reading UI file 'datalist_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATALIST_DIALOG_H
#define UI_DATALIST_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DataList_Dialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *mainLayout;
    QFrame *topFrame;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnAdd;
    QToolButton *btnRemove;
    QSpacerItem *horizontalSpacer;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *centralLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DataList_Dialog)
    {
        if (DataList_Dialog->objectName().isEmpty())
            DataList_Dialog->setObjectName(QString::fromUtf8("DataList_Dialog"));
        DataList_Dialog->resize(422, 388);
        verticalLayout_2 = new QVBoxLayout(DataList_Dialog);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(4, 4, 4, 4);
        mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(0);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        topFrame = new QFrame(DataList_Dialog);
        topFrame->setObjectName(QString::fromUtf8("topFrame"));
        topFrame->setMaximumSize(QSize(16777215, 40));
        topFrame->setFrameShape(QFrame::Panel);
        topFrame->setFrameShadow(QFrame::Sunken);
        topFrame->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(topFrame);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnAdd = new QToolButton(topFrame);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setMinimumSize(QSize(36, 36));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icons/buttons/Button_Add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon);
        btnAdd->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnAdd);

        btnRemove = new QToolButton(topFrame);
        btnRemove->setObjectName(QString::fromUtf8("btnRemove"));
        btnRemove->setMinimumSize(QSize(36, 36));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/icons/buttons/Button_Delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemove->setIcon(icon1);
        btnRemove->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnRemove);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        mainLayout->addWidget(topFrame);

        frame = new QFrame(DataList_Dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        centralLayout = new QVBoxLayout();
        centralLayout->setSpacing(4);
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        centralLayout->setContentsMargins(0, 0, 0, 4);

        verticalLayout_3->addLayout(centralLayout);


        mainLayout->addWidget(frame);

        buttonBox = new QDialogButtonBox(DataList_Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(mainLayout);


        retranslateUi(DataList_Dialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), DataList_Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DataList_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DataList_Dialog)
    {
        DataList_Dialog->setWindowTitle(QCoreApplication::translate("DataList_Dialog", "Dialog", nullptr));
        btnAdd->setText(QCoreApplication::translate("DataList_Dialog", "+", nullptr));
        btnRemove->setText(QCoreApplication::translate("DataList_Dialog", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DataList_Dialog: public Ui_DataList_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATALIST_DIALOG_H
