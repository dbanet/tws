/********************************************************************************
** Form generated from reading UI file 'textschemehandler.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTSCHEMEHANDLER_H
#define UI_TEXTSCHEMEHANDLER_H

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

class Ui_textschemehandlerClass
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidget;
    QPushButton *show;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *color;
    QPushButton *font;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *close;

    void setupUi(QWidget *textschemehandlerClass)
    {
        if (textschemehandlerClass->objectName().isEmpty())
            textschemehandlerClass->setObjectName(QString::fromUtf8("textschemehandlerClass"));
        textschemehandlerClass->resize(400, 300);
        horizontalLayout_3 = new QHBoxLayout(textschemehandlerClass);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidget = new QListWidget(textschemehandlerClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_2->addWidget(listWidget);

        show = new QPushButton(textschemehandlerClass);
        show->setObjectName(QString::fromUtf8("show"));

        verticalLayout_2->addWidget(show);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        color = new QPushButton(textschemehandlerClass);
        color->setObjectName(QString::fromUtf8("color"));

        horizontalLayout->addWidget(color);

        font = new QPushButton(textschemehandlerClass);
        font->setObjectName(QString::fromUtf8("font"));

        horizontalLayout->addWidget(font);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        close = new QPushButton(textschemehandlerClass);
        close->setObjectName(QString::fromUtf8("close"));

        horizontalLayout_2->addWidget(close);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(textschemehandlerClass);

        QMetaObject::connectSlotsByName(textschemehandlerClass);
    } // setupUi

    void retranslateUi(QWidget *textschemehandlerClass)
    {
        textschemehandlerClass->setWindowTitle(QApplication::translate("textschemehandlerClass", "textschemehandler", 0, QApplication::UnicodeUTF8));
        show->setText(QApplication::translate("textschemehandlerClass", "Show this texttype in channelwindows.", 0, QApplication::UnicodeUTF8));
        color->setText(QApplication::translate("textschemehandlerClass", "Set Color", 0, QApplication::UnicodeUTF8));
        font->setText(QApplication::translate("textschemehandlerClass", "Set font", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("textschemehandlerClass", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class textschemehandlerClass: public Ui_textschemehandlerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTSCHEMEHANDLER_H
