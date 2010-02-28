/********************************************************************************
** Form generated from reading UI file 'filehistorylister.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEHISTORYLISTER_H
#define UI_FILEHISTORYLISTER_H

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

class Ui_filehistorylisterClass
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QPushButton *close;

    void setupUi(QWidget *filehistorylisterClass)
    {
        if (filehistorylisterClass->objectName().isEmpty())
            filehistorylisterClass->setObjectName(QString::fromUtf8("filehistorylisterClass"));
        filehistorylisterClass->resize(533, 300);
        horizontalLayout = new QHBoxLayout(filehistorylisterClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(filehistorylisterClass);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 509, 240));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        horizontalLayout_2->addLayout(verticalLayout_2);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        close = new QPushButton(filehistorylisterClass);
        close->setObjectName(QString::fromUtf8("close"));

        verticalLayout->addWidget(close);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(filehistorylisterClass);

        QMetaObject::connectSlotsByName(filehistorylisterClass);
    } // setupUi

    void retranslateUi(QWidget *filehistorylisterClass)
    {
        filehistorylisterClass->setWindowTitle(QApplication::translate("filehistorylisterClass", "filehistorylister", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("filehistorylisterClass", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class filehistorylisterClass: public Ui_filehistorylisterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEHISTORYLISTER_H
