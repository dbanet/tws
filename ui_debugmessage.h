/********************************************************************************
** Form generated from reading ui file 'debugmessage.ui'
**
** Created: Sun Dec 14 15:46:04 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DEBUGMESSAGE_H
#define UI_DEBUGMESSAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_debugmessageClass
{
public:
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *debugmessageClass)
    {
    if (debugmessageClass->objectName().isEmpty())
        debugmessageClass->setObjectName(QString::fromUtf8("debugmessageClass"));
    debugmessageClass->resize(564, 650);
    debugmessageClass->setMinimumSize(QSize(0, 0));
    debugmessageClass->setMaximumSize(QSize(2000, 2000));
    horizontalLayout = new QHBoxLayout(debugmessageClass);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    textEdit = new QTextEdit(debugmessageClass);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setMinimumSize(QSize(0, 0));
    textEdit->setMaximumSize(QSize(2000, 2000));
    textEdit->setReadOnly(true);

    horizontalLayout->addWidget(textEdit);


    retranslateUi(debugmessageClass);

    QMetaObject::connectSlotsByName(debugmessageClass);
    } // setupUi

    void retranslateUi(QWidget *debugmessageClass)
    {
    debugmessageClass->setWindowTitle(QApplication::translate("debugmessageClass", "debugmessage", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(debugmessageClass);
    } // retranslateUi

};

namespace Ui {
    class debugmessageClass: public Ui_debugmessageClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGMESSAGE_H
