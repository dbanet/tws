/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_windowClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *bgframe;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *buttonlayout;
    QSplitter *splitter2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter1;
    QTreeView *hosts;
    QTextBrowser *chat;
    QHBoxLayout *horizontalLayout;
    QLineEdit *msg;
    QPushButton *send;
    QTreeView *users;

    void setupUi(QWidget *windowClass)
    {
        if (windowClass->objectName().isEmpty())
            windowClass->setObjectName(QString::fromUtf8("windowClass"));
        windowClass->resize(886, 613);
        horizontalLayout_2 = new QHBoxLayout(windowClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        bgframe = new QFrame(windowClass);
        bgframe->setObjectName(QString::fromUtf8("bgframe"));
        bgframe->setFrameShape(QFrame::StyledPanel);
        bgframe->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(bgframe);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonlayout = new QHBoxLayout();
        buttonlayout->setSpacing(6);
        buttonlayout->setObjectName(QString::fromUtf8("buttonlayout"));

        verticalLayout_2->addLayout(buttonlayout);

        splitter2 = new QSplitter(bgframe);
        splitter2->setObjectName(QString::fromUtf8("splitter2"));
        splitter2->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter1 = new QSplitter(layoutWidget);
        splitter1->setObjectName(QString::fromUtf8("splitter1"));
        splitter1->setOrientation(Qt::Vertical);
        hosts = new QTreeView(splitter1);
        hosts->setObjectName(QString::fromUtf8("hosts"));
        hosts->setExpandsOnDoubleClick(false);
        splitter1->addWidget(hosts);
        chat = new QTextBrowser(splitter1);
        chat->setObjectName(QString::fromUtf8("chat"));
        splitter1->addWidget(chat);

        verticalLayout->addWidget(splitter1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        msg = new QLineEdit(layoutWidget);
        msg->setObjectName(QString::fromUtf8("msg"));

        horizontalLayout->addWidget(msg);

        send = new QPushButton(layoutWidget);
        send->setObjectName(QString::fromUtf8("send"));

        horizontalLayout->addWidget(send);


        verticalLayout->addLayout(horizontalLayout);

        splitter2->addWidget(layoutWidget);
        users = new QTreeView(splitter2);
        users->setObjectName(QString::fromUtf8("users"));
        users->setSortingEnabled(true);
        users->setExpandsOnDoubleClick(false);
        splitter2->addWidget(users);

        verticalLayout_2->addWidget(splitter2);


        horizontalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_2->addWidget(bgframe);


        retranslateUi(windowClass);

        QMetaObject::connectSlotsByName(windowClass);
    } // setupUi

    void retranslateUi(QWidget *windowClass)
    {
        windowClass->setWindowTitle(QApplication::translate("windowClass", "window", 0, QApplication::UnicodeUTF8));
        send->setText(QApplication::translate("windowClass", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class windowClass: public Ui_windowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
