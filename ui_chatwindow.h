/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chatwindowClass
{
public:
    QHBoxLayout *horizontalLayout_4;
    QFrame *bgchatwindowframe;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QScrollArea *chatwindowbuttonscrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *buttonlayout;
    QPushButton *pbmute;
    QPushButton *pbbuddy;
    QPushButton *pblog;
    QPushButton *pbidle;
    QFrame *line_2;
    QPushButton *pbupload;
    QPushButton *pbfilehistory;
    QPushButton *pbsendfilehistorie;
    QFrame *line_3;
    QPushButton *pbctcp;
    QPushButton *pbstatus;
    QPushButton *pbfilter;
    QPushButton *pbsmileys;
    QTextBrowser *chat;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *send;

    void setupUi(QWidget *chatwindowClass)
    {
        if (chatwindowClass->objectName().isEmpty())
            chatwindowClass->setObjectName(QString::fromUtf8("chatwindowClass"));
        chatwindowClass->resize(517, 409);
        horizontalLayout_4 = new QHBoxLayout(chatwindowClass);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        bgchatwindowframe = new QFrame(chatwindowClass);
        bgchatwindowframe->setObjectName(QString::fromUtf8("bgchatwindowframe"));
        bgchatwindowframe->setFrameShape(QFrame::StyledPanel);
        bgchatwindowframe->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(bgchatwindowframe);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        chatwindowbuttonscrollArea = new QScrollArea(bgchatwindowframe);
        chatwindowbuttonscrollArea->setObjectName(QString::fromUtf8("chatwindowbuttonscrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chatwindowbuttonscrollArea->sizePolicy().hasHeightForWidth());
        chatwindowbuttonscrollArea->setSizePolicy(sizePolicy);
        chatwindowbuttonscrollArea->setMinimumSize(QSize(0, 43));
        chatwindowbuttonscrollArea->setMaximumSize(QSize(16777215, 43));
        chatwindowbuttonscrollArea->setSizeIncrement(QSize(0, 0));
        chatwindowbuttonscrollArea->setBaseSize(QSize(0, 0));
        chatwindowbuttonscrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chatwindowbuttonscrollArea->setWidgetResizable(true);
        chatwindowbuttonscrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 513, 41));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy1);
        scrollAreaWidgetContents->setMinimumSize(QSize(0, 41));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonlayout = new QHBoxLayout();
        buttonlayout->setSpacing(6);
        buttonlayout->setObjectName(QString::fromUtf8("buttonlayout"));
        buttonlayout->setSizeConstraint(QLayout::SetMinimumSize);
        pbmute = new QPushButton(scrollAreaWidgetContents);
        pbmute->setObjectName(QString::fromUtf8("pbmute"));
        pbmute->setMinimumSize(QSize(35, 35));
        pbmute->setMaximumSize(QSize(35, 35));
        pbmute->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbmute);

        pbbuddy = new QPushButton(scrollAreaWidgetContents);
        pbbuddy->setObjectName(QString::fromUtf8("pbbuddy"));
        pbbuddy->setMinimumSize(QSize(35, 35));
        pbbuddy->setMaximumSize(QSize(35, 35));
        pbbuddy->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbbuddy);

        pblog = new QPushButton(scrollAreaWidgetContents);
        pblog->setObjectName(QString::fromUtf8("pblog"));
        pblog->setMinimumSize(QSize(35, 35));
        pblog->setMaximumSize(QSize(35, 35));
        pblog->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pblog);

        pbidle = new QPushButton(scrollAreaWidgetContents);
        pbidle->setObjectName(QString::fromUtf8("pbidle"));
        pbidle->setMinimumSize(QSize(35, 35));
        pbidle->setMaximumSize(QSize(35, 35));
        pbidle->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbidle);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy2);
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        buttonlayout->addWidget(line_2);

        pbupload = new QPushButton(scrollAreaWidgetContents);
        pbupload->setObjectName(QString::fromUtf8("pbupload"));
        pbupload->setMinimumSize(QSize(35, 35));
        pbupload->setMaximumSize(QSize(35, 35));
        pbupload->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbupload);

        pbfilehistory = new QPushButton(scrollAreaWidgetContents);
        pbfilehistory->setObjectName(QString::fromUtf8("pbfilehistory"));
        pbfilehistory->setMinimumSize(QSize(35, 35));
        pbfilehistory->setMaximumSize(QSize(35, 35));
        pbfilehistory->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbfilehistory);

        pbsendfilehistorie = new QPushButton(scrollAreaWidgetContents);
        pbsendfilehistorie->setObjectName(QString::fromUtf8("pbsendfilehistorie"));
        pbsendfilehistorie->setMinimumSize(QSize(35, 35));
        pbsendfilehistorie->setMaximumSize(QSize(35, 35));
        pbsendfilehistorie->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbsendfilehistorie);

        line_3 = new QFrame(scrollAreaWidgetContents);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setLineWidth(2);
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        buttonlayout->addWidget(line_3);

        pbctcp = new QPushButton(scrollAreaWidgetContents);
        pbctcp->setObjectName(QString::fromUtf8("pbctcp"));
        pbctcp->setMinimumSize(QSize(35, 35));
        pbctcp->setMaximumSize(QSize(35, 35));
        pbctcp->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbctcp);

        pbstatus = new QPushButton(scrollAreaWidgetContents);
        pbstatus->setObjectName(QString::fromUtf8("pbstatus"));
        pbstatus->setMinimumSize(QSize(35, 35));
        pbstatus->setMaximumSize(QSize(35, 35));
        pbstatus->setIconSize(QSize(30, 30));

        buttonlayout->addWidget(pbstatus);

        pbfilter = new QPushButton(scrollAreaWidgetContents);
        pbfilter->setObjectName(QString::fromUtf8("pbfilter"));
        pbfilter->setMinimumSize(QSize(35, 35));
        pbfilter->setMaximumSize(QSize(35, 35));

        buttonlayout->addWidget(pbfilter);

        pbsmileys = new QPushButton(scrollAreaWidgetContents);
        pbsmileys->setObjectName(QString::fromUtf8("pbsmileys"));
        pbsmileys->setMinimumSize(QSize(35, 35));
        pbsmileys->setMaximumSize(QSize(35, 35));

        buttonlayout->addWidget(pbsmileys);


        horizontalLayout_2->addLayout(buttonlayout);

        chatwindowbuttonscrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(chatwindowbuttonscrollArea);

        chat = new QTextBrowser(bgchatwindowframe);
        chat->setObjectName(QString::fromUtf8("chat"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(chat->sizePolicy().hasHeightForWidth());
        chat->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(chat);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(bgchatwindowframe);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        send = new QPushButton(bgchatwindowframe);
        send->setObjectName(QString::fromUtf8("send"));

        horizontalLayout->addWidget(send);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_4->addWidget(bgchatwindowframe);


        retranslateUi(chatwindowClass);

        QMetaObject::connectSlotsByName(chatwindowClass);
    } // setupUi

    void retranslateUi(QWidget *chatwindowClass)
    {
        chatwindowClass->setWindowTitle(QApplication::translate("chatwindowClass", "chatwindow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbmute->setToolTip(QApplication::translate("chatwindowClass", "Use this button to mute or unmute this user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbmute->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbbuddy->setToolTip(QApplication::translate("chatwindowClass", "Use this button to add this user to buddies or normals.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbbuddy->setText(QString());
#ifndef QT_NO_TOOLTIP
        pblog->setToolTip(QApplication::translate("chatwindowClass", "Use this button to extract a logfile of the chat with htis user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pblog->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbidle->setToolTip(QApplication::translate("chatwindowClass", "Use this button to see the idle time of this user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbidle->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbupload->setToolTip(QApplication::translate("chatwindowClass", "Use this button to open QFileUpload.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbupload->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbfilehistory->setToolTip(QApplication::translate("chatwindowClass", "Use this button to handle the qfileupload files.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbfilehistory->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbsendfilehistorie->setToolTip(QApplication::translate("chatwindowClass", "Use this button to send this user your qfileupload history.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbsendfilehistorie->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbctcp->setToolTip(QApplication::translate("chatwindowClass", "Use this button to handle your ctcp commands.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbctcp->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbstatus->setToolTip(QApplication::translate("chatwindowClass", "Use this button to see the status of this user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbstatus->setText(QString());
#ifndef QT_NO_TOOLTIP
        pbfilter->setToolTip(QApplication::translate("chatwindowClass", "Use this button to filter join,part,quit information.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbfilter->setText(QString());
        pbsmileys->setText(QString());
        send->setText(QApplication::translate("chatwindowClass", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class chatwindowClass: public Ui_chatwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
