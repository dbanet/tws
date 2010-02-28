/********************************************************************************
** Form generated from reading UI file 'maintoolbox.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINTOOLBOX_H
#define UI_MAINTOOLBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_maintoolboxClass
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;

    void setupUi(QWidget *maintoolboxClass)
    {
        if (maintoolboxClass->objectName().isEmpty())
            maintoolboxClass->setObjectName(QString::fromUtf8("maintoolboxClass"));
        maintoolboxClass->resize(661, 393);
        horizontalLayout = new QHBoxLayout(maintoolboxClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(maintoolboxClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(maintoolboxClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(maintoolboxClass);
    } // setupUi

    void retranslateUi(QWidget *maintoolboxClass)
    {
        maintoolboxClass->setWindowTitle(QApplication::translate("maintoolboxClass", "maintoolbox", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("maintoolboxClass", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("maintoolboxClass", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class maintoolboxClass: public Ui_maintoolboxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINTOOLBOX_H
