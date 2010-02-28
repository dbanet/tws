/********************************************************************************
** Form generated from reading UI file 'awaybox.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWAYBOX_H
#define UI_AWAYBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_awayboxClass
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *back;
    QPushButton *forward;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QTextEdit *textEdit;
    QPushButton *ok;

    void setupUi(QWidget *awayboxClass)
    {
        if (awayboxClass->objectName().isEmpty())
            awayboxClass->setObjectName(QString::fromUtf8("awayboxClass"));
        awayboxClass->resize(388, 252);
        horizontalLayout_3 = new QHBoxLayout(awayboxClass);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        back = new QPushButton(awayboxClass);
        back->setObjectName(QString::fromUtf8("back"));
        back->setMaximumSize(QSize(22, 16777215));

        horizontalLayout_2->addWidget(back);

        forward = new QPushButton(awayboxClass);
        forward->setObjectName(QString::fromUtf8("forward"));
        forward->setMaximumSize(QSize(22, 16777215));

        horizontalLayout_2->addWidget(forward);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox = new QGroupBox(awayboxClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(17, 96, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout_2->addWidget(textEdit);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addWidget(groupBox);

        ok = new QPushButton(awayboxClass);
        ok->setObjectName(QString::fromUtf8("ok"));

        verticalLayout->addWidget(ok);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(awayboxClass);

        QMetaObject::connectSlotsByName(awayboxClass);
    } // setupUi

    void retranslateUi(QWidget *awayboxClass)
    {
        awayboxClass->setWindowTitle(QApplication::translate("awayboxClass", "awaybox", 0, QApplication::UnicodeUTF8));
        back->setText(QApplication::translate("awayboxClass", "<", 0, QApplication::UnicodeUTF8));
        forward->setText(QApplication::translate("awayboxClass", ">", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("awayboxClass", "Set your away message", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("awayboxClass", "ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class awayboxClass: public Ui_awayboxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWAYBOX_H
