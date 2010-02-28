/********************************************************************************
** Form generated from reading UI file 'buddylist.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUDDYLIST_H
#define UI_BUDDYLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_buddylistClass
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLineEdit *le;
    QPushButton *add;
    QListWidget *list;
    QPushButton *remove;
    QSpacerItem *verticalSpacer;
    QPushButton *close;

    void setupUi(QWidget *buddylistClass)
    {
        if (buddylistClass->objectName().isEmpty())
            buddylistClass->setObjectName(QString::fromUtf8("buddylistClass"));
        buddylistClass->resize(331, 383);
        horizontalLayout = new QHBoxLayout(buddylistClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        le = new QLineEdit(buddylistClass);
        le->setObjectName(QString::fromUtf8("le"));

        verticalLayout->addWidget(le);

        add = new QPushButton(buddylistClass);
        add->setObjectName(QString::fromUtf8("add"));

        verticalLayout->addWidget(add);

        list = new QListWidget(buddylistClass);
        list->setObjectName(QString::fromUtf8("list"));

        verticalLayout->addWidget(list);

        remove = new QPushButton(buddylistClass);
        remove->setObjectName(QString::fromUtf8("remove"));

        verticalLayout->addWidget(remove);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        close = new QPushButton(buddylistClass);
        close->setObjectName(QString::fromUtf8("close"));

        verticalLayout->addWidget(close);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(buddylistClass);

        QMetaObject::connectSlotsByName(buddylistClass);
    } // setupUi

    void retranslateUi(QWidget *buddylistClass)
    {
        buddylistClass->setWindowTitle(QApplication::translate("buddylistClass", "Buddylist handler.", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("buddylistClass", "Add this user.", 0, QApplication::UnicodeUTF8));
        remove->setText(QApplication::translate("buddylistClass", "Remove this user.", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("buddylistClass", "close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class buddylistClass: public Ui_buddylistClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUDDYLIST_H
