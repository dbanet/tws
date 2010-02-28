/********************************************************************************
** Form generated from reading UI file 'logbrowser.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGBROWSER_H
#define UI_LOGBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_logbrowserClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *chat;
    QPushButton *close;

    void setupUi(QWidget *logbrowserClass)
    {
        if (logbrowserClass->objectName().isEmpty())
            logbrowserClass->setObjectName(QString::fromUtf8("logbrowserClass"));
        logbrowserClass->resize(263, 300);
        horizontalLayout_2 = new QHBoxLayout(logbrowserClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listWidget = new QListWidget(logbrowserClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        chat = new QPushButton(logbrowserClass);
        chat->setObjectName(QString::fromUtf8("chat"));

        horizontalLayout->addWidget(chat);

        close = new QPushButton(logbrowserClass);
        close->setObjectName(QString::fromUtf8("close"));

        horizontalLayout->addWidget(close);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(logbrowserClass);

        QMetaObject::connectSlotsByName(logbrowserClass);
    } // setupUi

    void retranslateUi(QWidget *logbrowserClass)
    {
        logbrowserClass->setWindowTitle(QApplication::translate("logbrowserClass", "logbrowser", 0, QApplication::UnicodeUTF8));
        chat->setText(QApplication::translate("logbrowserClass", "Open a chatwindow.", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("logbrowserClass", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class logbrowserClass: public Ui_logbrowserClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGBROWSER_H
