/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindowClass
{
public:
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QLabel *connectlabel;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout;
    QComboBox *cbini;
    QPushButton *pbeditor;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lenick;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_14;
    QLineEdit *lepassword;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_7;
    QComboBox *flag;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_8;
    QComboBox *rank;
    QGroupBox *groupBox_6;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *clan;
    QGroupBox *groupBox_7;
    QHBoxLayout *horizontalLayout_10;
    QLineEdit *client;
    QCheckBox *cbremember;
    QPushButton *start;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *chbminimized;
    QCheckBox *chbautojoin;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_13;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label;
    QVBoxLayout *verticalLayout_4;
    QComboBox *cbchannels;
    QPushButton *pbrememberjoin;
    QPushButton *pbjoin;
    QPushButton *labelButton;
    QPlainTextEdit *labeltraydescription1;
    QPlainTextEdit *labeltraydescription;

    void setupUi(QWidget *mainwindowClass)
    {
        if (mainwindowClass->objectName().isEmpty())
            mainwindowClass->setObjectName(QString::fromUtf8("mainwindowClass"));
        mainwindowClass->resize(405, 704);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainwindowClass->sizePolicy().hasHeightForWidth());
        mainwindowClass->setSizePolicy(sizePolicy);
        mainwindowClass->setMinimumSize(QSize(405, 320));
        mainwindowClass->setMaximumSize(QSize(10000, 100000));
        horizontalLayout_5 = new QHBoxLayout(mainwindowClass);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_2 = new QLabel(mainwindowClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(390, 0));
        label_2->setPixmap(QPixmap(QString::fromUtf8("snppictures/snooperbanner.png")));

        verticalLayout_3->addWidget(label_2);

        connectlabel = new QLabel(mainwindowClass);
        connectlabel->setObjectName(QString::fromUtf8("connectlabel"));
        connectlabel->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(connectlabel);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(mainwindowClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_3 = new QHBoxLayout(tab);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_6 = new QHBoxLayout(groupBox);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cbini = new QComboBox(groupBox);
        cbini->setObjectName(QString::fromUtf8("cbini"));

        horizontalLayout->addWidget(cbini);

        pbeditor = new QPushButton(groupBox);
        pbeditor->setObjectName(QString::fromUtf8("pbeditor"));

        horizontalLayout->addWidget(pbeditor);


        horizontalLayout_6->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFlat(false);
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lenick = new QLineEdit(groupBox_3);
        lenick->setObjectName(QString::fromUtf8("lenick"));

        horizontalLayout_4->addWidget(lenick);


        horizontalLayout_2->addWidget(groupBox_3);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox_8 = new QGroupBox(tab);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        horizontalLayout_14 = new QHBoxLayout(groupBox_8);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        lepassword = new QLineEdit(groupBox_8);
        lepassword->setObjectName(QString::fromUtf8("lepassword"));

        horizontalLayout_14->addWidget(lepassword);


        verticalLayout->addWidget(groupBox_8);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_4);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        flag = new QComboBox(groupBox_4);
        flag->setObjectName(QString::fromUtf8("flag"));

        horizontalLayout_7->addWidget(flag);


        verticalLayout->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(tab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_5);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        rank = new QComboBox(groupBox_5);
        rank->setObjectName(QString::fromUtf8("rank"));
        rank->setIconSize(QSize(25, 16));

        horizontalLayout_8->addWidget(rank);


        verticalLayout->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(tab);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        horizontalLayout_9 = new QHBoxLayout(groupBox_6);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        clan = new QLineEdit(groupBox_6);
        clan->setObjectName(QString::fromUtf8("clan"));

        horizontalLayout_9->addWidget(clan);


        verticalLayout->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        horizontalLayout_10 = new QHBoxLayout(groupBox_7);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        client = new QLineEdit(groupBox_7);
        client->setObjectName(QString::fromUtf8("client"));

        horizontalLayout_10->addWidget(client);


        verticalLayout->addWidget(groupBox_7);

        cbremember = new QCheckBox(tab);
        cbremember->setObjectName(QString::fromUtf8("cbremember"));

        verticalLayout->addWidget(cbremember);

        start = new QPushButton(tab);
        start->setObjectName(QString::fromUtf8("start"));

        verticalLayout->addWidget(start);


        horizontalLayout_3->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_6 = new QVBoxLayout(tab_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        chbminimized = new QCheckBox(tab_2);
        chbminimized->setObjectName(QString::fromUtf8("chbminimized"));

        verticalLayout_5->addWidget(chbminimized);

        chbautojoin = new QCheckBox(tab_2);
        chbautojoin->setObjectName(QString::fromUtf8("chbautojoin"));

        verticalLayout_5->addWidget(chbautojoin);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_13 = new QHBoxLayout(groupBox_2);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(label);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        cbchannels = new QComboBox(groupBox_2);
        cbchannels->setObjectName(QString::fromUtf8("cbchannels"));

        verticalLayout_4->addWidget(cbchannels);

        pbrememberjoin = new QPushButton(groupBox_2);
        pbrememberjoin->setObjectName(QString::fromUtf8("pbrememberjoin"));

        verticalLayout_4->addWidget(pbrememberjoin);


        horizontalLayout_12->addLayout(verticalLayout_4);


        horizontalLayout_13->addLayout(horizontalLayout_12);


        verticalLayout_5->addWidget(groupBox_2);

        pbjoin = new QPushButton(tab_2);
        pbjoin->setObjectName(QString::fromUtf8("pbjoin"));
        pbjoin->setEnabled(false);

        verticalLayout_5->addWidget(pbjoin);


        verticalLayout_6->addLayout(verticalLayout_5);

        labelButton = new QPushButton(tab_2);
        labelButton->setObjectName(QString::fromUtf8("labelButton"));
        labelButton->setMinimumSize(QSize(0, 45));
        labelButton->setIconSize(QSize(191, 39));

        verticalLayout_6->addWidget(labelButton);

        labeltraydescription1 = new QPlainTextEdit(tab_2);
        labeltraydescription1->setObjectName(QString::fromUtf8("labeltraydescription1"));
        labeltraydescription1->setMinimumSize(QSize(0, 100));
        labeltraydescription1->setMaximumSize(QSize(16777215, 100));
        labeltraydescription1->setReadOnly(true);

        verticalLayout_6->addWidget(labeltraydescription1);

        labeltraydescription = new QPlainTextEdit(tab_2);
        labeltraydescription->setObjectName(QString::fromUtf8("labeltraydescription"));
        labeltraydescription->setReadOnly(true);

        verticalLayout_6->addWidget(labeltraydescription);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_5->addLayout(verticalLayout_3);


        retranslateUi(mainwindowClass);

        tabWidget->setCurrentIndex(0);
        flag->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(mainwindowClass);
    } // setupUi

    void retranslateUi(QWidget *mainwindowClass)
    {
        mainwindowClass->setWindowTitle(QApplication::translate("mainwindowClass", "mainwindow", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        connectlabel->setText(QApplication::translate("mainwindowClass", "Disconnected", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("mainwindowClass", "Choose an existing ini file. (better dont touch this.)", 0, QApplication::UnicodeUTF8));
        pbeditor->setText(QApplication::translate("mainwindowClass", "Open Editor", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("mainwindowClass", "Choose a nickname.", 0, QApplication::UnicodeUTF8));
        lenick->setText(QApplication::translate("mainwindowClass", "nick", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("mainwindowClass", "Choose your password.", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("mainwindowClass", "Choose a Flag.", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("mainwindowClass", "Choose a Rank.", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("mainwindowClass", "Choose a Clan.", 0, QApplication::UnicodeUTF8));
        clan->setInputMask(QString());
        clan->setText(QApplication::translate("mainwindowClass", "noclan", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("mainwindowClass", "Choose a Client.", 0, QApplication::UnicodeUTF8));
        client->setText(QApplication::translate("mainwindowClass", "The Wheat Snooper", 0, QApplication::UnicodeUTF8));
        cbremember->setText(QApplication::translate("mainwindowClass", "Remember these settings and dont show this field on next start.", 0, QApplication::UnicodeUTF8));
        start->setText(QApplication::translate("mainwindowClass", "choose", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainwindowClass", "Settings", 0, QApplication::UnicodeUTF8));
        chbminimized->setText(QApplication::translate("mainwindowClass", "Start minimized to tray.", 0, QApplication::UnicodeUTF8));
        chbautojoin->setText(QApplication::translate("mainwindowClass", "Join a channel on startup.", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        label->setText(QString());
        pbrememberjoin->setText(QApplication::translate("mainwindowClass", "Join this channel on every startup.", 0, QApplication::UnicodeUTF8));
        pbjoin->setText(QApplication::translate("mainwindowClass", "join", 0, QApplication::UnicodeUTF8));
        labelButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainwindowClass", "Join channels", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainwindowClass: public Ui_mainwindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
