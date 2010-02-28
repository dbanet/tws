/********************************************************************************
** Form generated from reading UI file 'ctctphandlerwidget.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTCTPHANDLERWIDGET_H
#define UI_CTCTPHANDLERWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ctctphandlerwidgetClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pbhowto;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *atomiclayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *add;
    QPushButton *safeclose;
    QPushButton *close_2;

    void setupUi(QWidget *ctctphandlerwidgetClass)
    {
        if (ctctphandlerwidgetClass->objectName().isEmpty())
            ctctphandlerwidgetClass->setObjectName(QString::fromUtf8("ctctphandlerwidgetClass"));
        ctctphandlerwidgetClass->resize(511, 346);
        horizontalLayout_2 = new QHBoxLayout(ctctphandlerwidgetClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pbhowto = new QPushButton(ctctphandlerwidgetClass);
        pbhowto->setObjectName(QString::fromUtf8("pbhowto"));

        verticalLayout_2->addWidget(pbhowto);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(ctctphandlerwidgetClass);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 485, 249));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        atomiclayout = new QVBoxLayout();
        atomiclayout->setSpacing(6);
        atomiclayout->setObjectName(QString::fromUtf8("atomiclayout"));

        horizontalLayout_3->addLayout(atomiclayout);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        add = new QPushButton(ctctphandlerwidgetClass);
        add->setObjectName(QString::fromUtf8("add"));

        horizontalLayout->addWidget(add);

        safeclose = new QPushButton(ctctphandlerwidgetClass);
        safeclose->setObjectName(QString::fromUtf8("safeclose"));

        horizontalLayout->addWidget(safeclose);

        close_2 = new QPushButton(ctctphandlerwidgetClass);
        close_2->setObjectName(QString::fromUtf8("close_2"));

        horizontalLayout->addWidget(close_2);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        retranslateUi(ctctphandlerwidgetClass);

        QMetaObject::connectSlotsByName(ctctphandlerwidgetClass);
    } // setupUi

    void retranslateUi(QWidget *ctctphandlerwidgetClass)
    {
        ctctphandlerwidgetClass->setWindowTitle(QApplication::translate("ctctphandlerwidgetClass", "ctctphandlerwidget", 0, QApplication::UnicodeUTF8));
        pbhowto->setText(QApplication::translate("ctctphandlerwidgetClass", "Show a how to for CTCP", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("ctctphandlerwidgetClass", "Add a command", 0, QApplication::UnicodeUTF8));
        safeclose->setText(QApplication::translate("ctctphandlerwidgetClass", "Save and Close", 0, QApplication::UnicodeUTF8));
        close_2->setText(QApplication::translate("ctctphandlerwidgetClass", "Close without saving", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ctctphandlerwidgetClass: public Ui_ctctphandlerwidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTCTPHANDLERWIDGET_H
