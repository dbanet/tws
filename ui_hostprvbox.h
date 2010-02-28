/********************************************************************************
** Form generated from reading UI file 'hostprvbox.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOSTPRVBOX_H
#define UI_HOSTPRVBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hostprvboxClass
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *lescheme;
    QPushButton *pbscheme;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lehostport;
    QGroupBox *gbip;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cbip;
    QLineEdit *leip;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QComboBox *icons;
    QPushButton *add;
    QPushButton *clip;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QPushButton *ok;
    QPushButton *okjust;
    QPushButton *cancel;

    void setupUi(QWidget *hostprvboxClass)
    {
        if (hostprvboxClass->objectName().isEmpty())
            hostprvboxClass->setObjectName(QString::fromUtf8("hostprvboxClass"));
        hostprvboxClass->resize(459, 519);
        horizontalLayout_4 = new QHBoxLayout(hostprvboxClass);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_4 = new QGroupBox(hostprvboxClass);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(0, 80));
        horizontalLayout_7 = new QHBoxLayout(groupBox_4);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        lescheme = new QLineEdit(groupBox_4);
        lescheme->setObjectName(QString::fromUtf8("lescheme"));

        horizontalLayout_6->addWidget(lescheme);

        pbscheme = new QPushButton(groupBox_4);
        pbscheme->setObjectName(QString::fromUtf8("pbscheme"));

        horizontalLayout_6->addWidget(pbscheme);


        horizontalLayout_7->addLayout(horizontalLayout_6);


        verticalLayout_3->addWidget(groupBox_4);

        groupBox = new QGroupBox(hostprvboxClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 61));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lehostport = new QLineEdit(groupBox);
        lehostport->setObjectName(QString::fromUtf8("lehostport"));

        horizontalLayout->addWidget(lehostport);


        verticalLayout_3->addWidget(groupBox);

        gbip = new QGroupBox(hostprvboxClass);
        gbip->setObjectName(QString::fromUtf8("gbip"));
        gbip->setMinimumSize(QSize(0, 75));
        horizontalLayout_2 = new QHBoxLayout(gbip);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cbip = new QCheckBox(gbip);
        cbip->setObjectName(QString::fromUtf8("cbip"));

        horizontalLayout_2->addWidget(cbip);

        leip = new QLineEdit(gbip);
        leip->setObjectName(QString::fromUtf8("leip"));

        horizontalLayout_2->addWidget(leip);


        verticalLayout_3->addWidget(gbip);

        groupBox_3 = new QGroupBox(hostprvboxClass);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 110));
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


        verticalLayout_3->addWidget(groupBox_3);

        clip = new QPushButton(hostprvboxClass);
        clip->setObjectName(QString::fromUtf8("clip"));

        verticalLayout_3->addWidget(clip);

        verticalSpacer = new QSpacerItem(20, 190, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ok = new QPushButton(hostprvboxClass);
        ok->setObjectName(QString::fromUtf8("ok"));

        verticalLayout_2->addWidget(ok);

        okjust = new QPushButton(hostprvboxClass);
        okjust->setObjectName(QString::fromUtf8("okjust"));

        verticalLayout_2->addWidget(okjust);

        cancel = new QPushButton(hostprvboxClass);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        verticalLayout_2->addWidget(cancel);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(hostprvboxClass);

        QMetaObject::connectSlotsByName(hostprvboxClass);
    } // setupUi

    void retranslateUi(QWidget *hostprvboxClass)
    {
        hostprvboxClass->setWindowTitle(QApplication::translate("hostprvboxClass", "hostprvbox", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("hostprvboxClass", "Set a scheme", 0, QApplication::UnicodeUTF8));
        pbscheme->setText(QApplication::translate("hostprvboxClass", "Schow how to", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("hostprvboxClass", "Host port.", 0, QApplication::UnicodeUTF8));
        gbip->setTitle(QApplication::translate("hostprvboxClass", "Use this ip for hosting. (This is normally not necessary.)", 0, QApplication::UnicodeUTF8));
        cbip->setText(QApplication::translate("hostprvboxClass", "Enable", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("hostprvboxClass", "Chose a program.", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("hostprvboxClass", "Add a Program.", 0, QApplication::UnicodeUTF8));
        clip->setText(QApplication::translate("hostprvboxClass", "Copy the gamelink to clipboard", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("hostprvboxClass", "Host and send a link to all my buddys", 0, QApplication::UnicodeUTF8));
        okjust->setText(QApplication::translate("hostprvboxClass", "Just host", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("hostprvboxClass", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class hostprvboxClass: public Ui_hostprvboxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSTPRVBOX_H
