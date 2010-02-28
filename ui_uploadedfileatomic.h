/********************************************************************************
** Form generated from reading UI file 'uploadedfileatomic.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPLOADEDFILEATOMIC_H
#define UI_UPLOADEDFILEATOMIC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_uploadedfileatomicClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QPushButton *copy;
    QPushButton *del;

    void setupUi(QWidget *uploadedfileatomicClass)
    {
        if (uploadedfileatomicClass->objectName().isEmpty())
            uploadedfileatomicClass->setObjectName(QString::fromUtf8("uploadedfileatomicClass"));
        uploadedfileatomicClass->resize(400, 104);
        uploadedfileatomicClass->setMinimumSize(QSize(400, 104));
        uploadedfileatomicClass->setMaximumSize(QSize(9999, 104));
        horizontalLayout_2 = new QHBoxLayout(uploadedfileatomicClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(uploadedfileatomicClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(200, 25));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        copy = new QPushButton(groupBox);
        copy->setObjectName(QString::fromUtf8("copy"));
        copy->setMaximumSize(QSize(9999, 16777215));

        horizontalLayout->addWidget(copy);

        del = new QPushButton(groupBox);
        del->setObjectName(QString::fromUtf8("del"));
        del->setMaximumSize(QSize(2009999, 16777215));

        horizontalLayout->addWidget(del);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(uploadedfileatomicClass);

        QMetaObject::connectSlotsByName(uploadedfileatomicClass);
    } // setupUi

    void retranslateUi(QWidget *uploadedfileatomicClass)
    {
        uploadedfileatomicClass->setWindowTitle(QApplication::translate("uploadedfileatomicClass", "uploadedfileatomic", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label->setText(QString());
        copy->setText(QApplication::translate("uploadedfileatomicClass", "Copy this link to clipboard.", 0, QApplication::UnicodeUTF8));
        del->setText(QApplication::translate("uploadedfileatomicClass", "Delete this link.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class uploadedfileatomicClass: public Ui_uploadedfileatomicClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPLOADEDFILEATOMIC_H
