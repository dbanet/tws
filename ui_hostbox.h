/********************************************************************************
** Form generated from reading UI file 'hostbox.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOSTBOX_H
#define UI_HOSTBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hostboxClass
{
public:
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *gamename;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chbPassword;
    QPushButton *pbshowdetails;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QComboBox *icons;
    QPushButton *add;
    QSpacerItem *verticalSpacer;
    QFrame *detailsframe;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *leplayername;
    QGroupBox *gbip;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *cbip;
    QLineEdit *leip;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *lehostport;
    QCheckBox *chbwaitforhost;
    QCheckBox *chbsendhostinfotochan;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *ok;
    QPushButton *cancel;

    void setupUi(QWidget *hostboxClass)
    {
        if (hostboxClass->objectName().isEmpty())
            hostboxClass->setObjectName(QString::fromUtf8("hostboxClass"));
        hostboxClass->resize(609, 662);
        hostboxClass->setMaximumSize(QSize(5645456, 16777215));
        horizontalLayout_5 = new QHBoxLayout(hostboxClass);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(hostboxClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 61));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gamename = new QLineEdit(groupBox);
        gamename->setObjectName(QString::fromUtf8("gamename"));

        horizontalLayout->addWidget(gamename);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        chbPassword = new QCheckBox(hostboxClass);
        chbPassword->setObjectName(QString::fromUtf8("chbPassword"));

        horizontalLayout_2->addWidget(chbPassword);

        pbshowdetails = new QPushButton(hostboxClass);
        pbshowdetails->setObjectName(QString::fromUtf8("pbshowdetails"));
        pbshowdetails->setMinimumSize(QSize(25, 25));
        pbshowdetails->setMaximumSize(QSize(25, 25));

        horizontalLayout_2->addWidget(pbshowdetails);


        verticalLayout_2->addLayout(horizontalLayout_2);

        groupBox_3 = new QGroupBox(hostboxClass);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(96, 100));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        icons = new QComboBox(groupBox_3);
        icons->setObjectName(QString::fromUtf8("icons"));

        verticalLayout->addWidget(icons);

        add = new QPushButton(groupBox_3);
        add->setObjectName(QString::fromUtf8("add"));

        verticalLayout->addWidget(add);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        detailsframe = new QFrame(hostboxClass);
        detailsframe->setObjectName(QString::fromUtf8("detailsframe"));
        detailsframe->setMinimumSize(QSize(0, 250));
        detailsframe->setMaximumSize(QSize(9999, 16777215));
        detailsframe->setFrameShape(QFrame::StyledPanel);
        detailsframe->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(detailsframe);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_4 = new QGroupBox(detailsframe);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(0, 61));
        horizontalLayout_6 = new QHBoxLayout(groupBox_4);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        leplayername = new QLineEdit(groupBox_4);
        leplayername->setObjectName(QString::fromUtf8("leplayername"));

        horizontalLayout_6->addWidget(leplayername);


        verticalLayout_3->addWidget(groupBox_4);

        gbip = new QGroupBox(detailsframe);
        gbip->setObjectName(QString::fromUtf8("gbip"));
        horizontalLayout_9 = new QHBoxLayout(gbip);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(9, 9, -1, -1);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        cbip = new QCheckBox(gbip);
        cbip->setObjectName(QString::fromUtf8("cbip"));

        horizontalLayout_8->addWidget(cbip);

        leip = new QLineEdit(gbip);
        leip->setObjectName(QString::fromUtf8("leip"));

        horizontalLayout_8->addWidget(leip);


        horizontalLayout_9->addLayout(horizontalLayout_8);


        verticalLayout_3->addWidget(gbip);

        groupBox_5 = new QGroupBox(detailsframe);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(0, 61));
        horizontalLayout_7 = new QHBoxLayout(groupBox_5);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        lehostport = new QLineEdit(groupBox_5);
        lehostport->setObjectName(QString::fromUtf8("lehostport"));

        horizontalLayout_7->addWidget(lehostport);


        verticalLayout_3->addWidget(groupBox_5);

        chbwaitforhost = new QCheckBox(detailsframe);
        chbwaitforhost->setObjectName(QString::fromUtf8("chbwaitforhost"));

        verticalLayout_3->addWidget(chbwaitforhost);


        verticalLayout_2->addWidget(detailsframe);

        chbsendhostinfotochan = new QCheckBox(hostboxClass);
        chbsendhostinfotochan->setObjectName(QString::fromUtf8("chbsendhostinfotochan"));

        verticalLayout_2->addWidget(chbsendhostinfotochan);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        ok = new QPushButton(hostboxClass);
        ok->setObjectName(QString::fromUtf8("ok"));

        horizontalLayout_4->addWidget(ok);

        cancel = new QPushButton(hostboxClass);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        horizontalLayout_4->addWidget(cancel);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout_2);


        retranslateUi(hostboxClass);

        QMetaObject::connectSlotsByName(hostboxClass);
    } // setupUi

    void retranslateUi(QWidget *hostboxClass)
    {
        hostboxClass->setWindowTitle(QApplication::translate("hostboxClass", "Set up your host and click ok.", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("hostboxClass", "Gamename", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chbPassword->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        chbPassword->setText(QApplication::translate("hostboxClass", "Show this game as a private game.", 0, QApplication::UnicodeUTF8));
        pbshowdetails->setText(QApplication::translate("hostboxClass", "O", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("hostboxClass", "Chose a program.", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("hostboxClass", "Add a Program.", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("hostboxClass", "Playername for this Host (leave this empty to use your wormnet nick.)", 0, QApplication::UnicodeUTF8));
        gbip->setTitle(QApplication::translate("hostboxClass", "Use this ip for hosting. (This is normally not necessary.)", 0, QApplication::UnicodeUTF8));
        cbip->setText(QApplication::translate("hostboxClass", "Enable", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("hostboxClass", "Host port.", 0, QApplication::UnicodeUTF8));
        chbwaitforhost->setText(QApplication::translate("hostboxClass", "The Snooper should wait starting worms, until the host appears.", 0, QApplication::UnicodeUTF8));
        chbsendhostinfotochan->setText(QApplication::translate("hostboxClass", "Send a hostinfo to the channel.", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("hostboxClass", "Ok", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("hostboxClass", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class hostboxClass: public Ui_hostboxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSTBOX_H
