/********************************************************************************
** Form generated from reading ui file 'mmwindow.ui'
**
** Created: Wed Oct 22 17:47:44 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MMWINDOW_H
#define UI_MMWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mmwindowClass
{
public:
    QGroupBox *groupBox;
    QListWidget *userlist;
    QGroupBox *groupBox_2;
    QListWidget *chatlist;
    QLineEdit *lemsg;
    QComboBox *cbuser;
    QPushButton *pbinvent;

    void setupUi(QWidget *mmwindowClass)
    {
    if (mmwindowClass->objectName().isEmpty())
        mmwindowClass->setObjectName(QString::fromUtf8("mmwindowClass"));
    mmwindowClass->resize(430, 455);
    groupBox = new QGroupBox(mmwindowClass);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(0, 40, 431, 161));
    userlist = new QListWidget(groupBox);
    userlist->setObjectName(QString::fromUtf8("userlist"));
    userlist->setGeometry(QRect(10, 20, 411, 131));
    groupBox_2 = new QGroupBox(mmwindowClass);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(0, 210, 431, 141));
    chatlist = new QListWidget(groupBox_2);
    chatlist->setObjectName(QString::fromUtf8("chatlist"));
    chatlist->setGeometry(QRect(10, 20, 411, 101));
    lemsg = new QLineEdit(mmwindowClass);
    lemsg->setObjectName(QString::fromUtf8("lemsg"));
    lemsg->setGeometry(QRect(0, 360, 431, 22));
    cbuser = new QComboBox(mmwindowClass);
    cbuser->setObjectName(QString::fromUtf8("cbuser"));
    cbuser->setGeometry(QRect(0, 10, 331, 22));
    pbinvent = new QPushButton(mmwindowClass);
    pbinvent->setObjectName(QString::fromUtf8("pbinvent"));
    pbinvent->setGeometry(QRect(350, 10, 75, 26));

    retranslateUi(mmwindowClass);

    QMetaObject::connectSlotsByName(mmwindowClass);
    } // setupUi

    void retranslateUi(QWidget *mmwindowClass)
    {
    mmwindowClass->setWindowTitle(QApplication::translate("mmwindowClass", "mmwindow", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("mmwindowClass", "user", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("mmwindowClass", "chat", 0, QApplication::UnicodeUTF8));
    pbinvent->setText(QApplication::translate("mmwindowClass", "invent", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(mmwindowClass);
    } // retranslateUi

};

namespace Ui {
    class mmwindowClass: public Ui_mmwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MMWINDOW_H
