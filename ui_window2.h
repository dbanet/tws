/********************************************************************************
** Form generated from reading UI file 'window2.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW2_H
#define UI_WINDOW2_H

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

class Ui_Form
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
    QTreeView *users;
    QTreeView *hosts;
    QHBoxLayout *horizontalLayout;
    QLineEdit *msg;
    QPushButton *send;
    QTextBrowser *chat;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(823, 635);
        horizontalLayout_2 = new QHBoxLayout(Form);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        bgframe = new QFrame(Form);
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

        splitter2 = new QSplitter(bgframe);
        splitter2->setObjectName(QString::fromUtf8("splitter2"));
        splitter2->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter1 = new QSplitter(layoutWidget);
        splitter1->setObjectName(QString::fromUtf8("splitter1"));
        splitter1->setOrientation(Qt::Horizontal);
        users = new QTreeView(splitter1);
        users->setObjectName(QString::fromUtf8("users"));
        users->setSortingEnabled(true);
        users->setExpandsOnDoubleClick(false);
        splitter1->addWidget(users);
        hosts = new QTreeView(splitter1);
        hosts->setObjectName(QString::fromUtf8("hosts"));
        hosts->setExpandsOnDoubleClick(false);
        splitter1->addWidget(hosts);

        verticalLayout->addWidget(splitter1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        msg = new QLineEdit(layoutWidget);
        msg->setObjectName(QString::fromUtf8("msg"));

        horizontalLayout->addWidget(msg);

        send = new QPushButton(layoutWidget);
        send->setObjectName(QString::fromUtf8("send"));

        horizontalLayout->addWidget(send);


        verticalLayout->addLayout(horizontalLayout);

        splitter2->addWidget(layoutWidget);
        chat = new QTextBrowser(splitter2);
        chat->setObjectName(QString::fromUtf8("chat"));
        splitter2->addWidget(chat);

        verticalLayout_2->addWidget(splitter2);


        horizontalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_2->addWidget(bgframe);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        send->setText(QApplication::translate("Form", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW2_H
