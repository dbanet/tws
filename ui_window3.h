/********************************************************************************
** Form generated from reading UI file 'window3.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW3_H
#define UI_WINDOW3_H

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

class Ui_Form3
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *bgframe;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *buttonlayout;
    QSplitter *splitter1;
    QTreeView *hosts;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter2;
    QTreeView *users;
    QTextBrowser *chat;
    QHBoxLayout *horizontalLayout;
    QLineEdit *msg;
    QPushButton *send;

    void setupUi(QWidget *Form3)
    {
        if (Form3->objectName().isEmpty())
            Form3->setObjectName(QString::fromUtf8("Form3"));
        Form3->resize(898, 742);
        horizontalLayout_2 = new QHBoxLayout(Form3);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        bgframe = new QFrame(Form3);
        bgframe->setObjectName(QString::fromUtf8("bgframe"));
        bgframe->setFrameShape(QFrame::StyledPanel);
        bgframe->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(bgframe);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonlayout = new QHBoxLayout();
        buttonlayout->setObjectName(QString::fromUtf8("buttonlayout"));

        verticalLayout_2->addLayout(buttonlayout);

        splitter1 = new QSplitter(bgframe);
        splitter1->setObjectName(QString::fromUtf8("splitter1"));
        splitter1->setOrientation(Qt::Vertical);
        hosts = new QTreeView(splitter1);
        hosts->setObjectName(QString::fromUtf8("hosts"));
        hosts->setExpandsOnDoubleClick(false);
        splitter1->addWidget(hosts);
        layoutWidget = new QWidget(splitter1);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter2 = new QSplitter(layoutWidget);
        splitter2->setObjectName(QString::fromUtf8("splitter2"));
        splitter2->setOrientation(Qt::Horizontal);
        users = new QTreeView(splitter2);
        users->setObjectName(QString::fromUtf8("users"));
        users->setSortingEnabled(true);
        users->setExpandsOnDoubleClick(false);
        splitter2->addWidget(users);
        chat = new QTextBrowser(splitter2);
        chat->setObjectName(QString::fromUtf8("chat"));
        splitter2->addWidget(chat);

        verticalLayout->addWidget(splitter2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        msg = new QLineEdit(layoutWidget);
        msg->setObjectName(QString::fromUtf8("msg"));

        horizontalLayout->addWidget(msg);

        send = new QPushButton(layoutWidget);
        send->setObjectName(QString::fromUtf8("send"));

        horizontalLayout->addWidget(send);


        verticalLayout->addLayout(horizontalLayout);

        splitter1->addWidget(layoutWidget);

        verticalLayout_2->addWidget(splitter1);


        horizontalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_2->addWidget(bgframe);


        retranslateUi(Form3);

        QMetaObject::connectSlotsByName(Form3);
    } // setupUi

    void retranslateUi(QWidget *Form3)
    {
        Form3->setWindowTitle(QApplication::translate("Form3", "Form", 0, QApplication::UnicodeUTF8));
        send->setText(QApplication::translate("Form3", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form3: public Ui_Form3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW3_H
