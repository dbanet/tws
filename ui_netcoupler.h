/********************************************************************************
** Form generated from reading ui file 'netcoupler.ui'
**
** Created: Sat Oct 18 16:55:58 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NETCOUPLER_H
#define UI_NETCOUPLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_netcouplerClass
{
public:

    void setupUi(QWidget *netcouplerClass)
    {
    if (netcouplerClass->objectName().isEmpty())
        netcouplerClass->setObjectName(QString::fromUtf8("netcouplerClass"));
    netcouplerClass->resize(400, 300);

    retranslateUi(netcouplerClass);

    QMetaObject::connectSlotsByName(netcouplerClass);
    } // setupUi

    void retranslateUi(QWidget *netcouplerClass)
    {
    netcouplerClass->setWindowTitle(QApplication::translate("netcouplerClass", "netcoupler", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(netcouplerClass);
    } // retranslateUi

};

namespace Ui {
    class netcouplerClass: public Ui_netcouplerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETCOUPLER_H
