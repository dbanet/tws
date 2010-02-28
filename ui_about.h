/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_aboutClass
{
public:
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *aboutClass)
    {
        if (aboutClass->objectName().isEmpty())
            aboutClass->setObjectName(QString::fromUtf8("aboutClass"));
        aboutClass->resize(400, 300);
        horizontalLayout = new QHBoxLayout(aboutClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textBrowser = new QTextBrowser(aboutClass);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        horizontalLayout->addWidget(textBrowser);


        retranslateUi(aboutClass);

        QMetaObject::connectSlotsByName(aboutClass);
    } // setupUi

    void retranslateUi(QWidget *aboutClass)
    {
        aboutClass->setWindowTitle(QApplication::translate("aboutClass", "about", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class aboutClass: public Ui_aboutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
