/********************************************************************************
** Form generated from reading UI file 'n3_names_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_N3_NAMES_DIALOG_H
#define UI_N3_NAMES_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_N3NamesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabNames;
    QVBoxLayout *verticalLayout_2;
    QTableView *tvNames;
    QWidget *tabAlarms;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *N3NamesDialog)
    {
        if (N3NamesDialog->objectName().isEmpty())
            N3NamesDialog->setObjectName(QString::fromUtf8("N3NamesDialog"));
        N3NamesDialog->setWindowModality(Qt::WindowModal);
        N3NamesDialog->resize(482, 436);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/N3/n3.png"), QSize(), QIcon::Normal, QIcon::Off);
        N3NamesDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(N3NamesDialog);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        tabWidget = new QTabWidget(N3NamesDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabNames = new QWidget();
        tabNames->setObjectName(QString::fromUtf8("tabNames"));
        verticalLayout_2 = new QVBoxLayout(tabNames);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tvNames = new QTableView(tabNames);
        tvNames->setObjectName(QString::fromUtf8("tvNames"));
        tvNames->setAlternatingRowColors(true);
        tvNames->setSelectionMode(QAbstractItemView::SingleSelection);
        tvNames->setSelectionBehavior(QAbstractItemView::SelectItems);
        tvNames->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tvNames->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tvNames->horizontalHeader()->setStretchLastSection(true);
        tvNames->verticalHeader()->setStretchLastSection(false);

        verticalLayout_2->addWidget(tvNames);

        tabWidget->addTab(tabNames, QString());
        tabAlarms = new QWidget();
        tabAlarms->setObjectName(QString::fromUtf8("tabAlarms"));
        tabWidget->addTab(tabAlarms, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(N3NamesDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(N3NamesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), N3NamesDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), N3NamesDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(N3NamesDialog);
    } // setupUi

    void retranslateUi(QDialog *N3NamesDialog)
    {
        N3NamesDialog->setWindowTitle(QCoreApplication::translate("N3NamesDialog", "Names", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabNames), QCoreApplication::translate("N3NamesDialog", "Names list", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAlarms), QCoreApplication::translate("N3NamesDialog", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class N3NamesDialog: public Ui_N3NamesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_N3_NAMES_DIALOG_H
