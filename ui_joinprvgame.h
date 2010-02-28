/********************************************************************************
** Form generated from reading UI file 'joinprvgame.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOINPRVGAME_H
#define UI_JOINPRVGAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_joinprvgameClass
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QComboBox *icons;
    QPushButton *add;
    QPushButton *ok;
    QPushButton *cancel;

    void setupUi(QWidget *joinprvgameClass)
    {
        if (joinprvgameClass->objectName().isEmpty())
            joinprvgameClass->setObjectName(QString::fromUtf8("joinprvgameClass"));
        joinprvgameClass->resize(382, 139);
        horizontalLayout = new QHBoxLayout(joinprvgameClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        icons = new QComboBox(joinprvgameClass);
        icons->setObjectName(QString::fromUtf8("icons"));

        verticalLayout->addWidget(icons);

        add = new QPushButton(joinprvgameClass);
        add->setObjectName(QString::fromUtf8("add"));

        verticalLayout->addWidget(add);

        ok = new QPushButton(joinprvgameClass);
        ok->setObjectName(QString::fromUtf8("ok"));

        verticalLayout->addWidget(ok);

        cancel = new QPushButton(joinprvgameClass);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        verticalLayout->addWidget(cancel);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(joinprvgameClass);

        QMetaObject::connectSlotsByName(joinprvgameClass);
    } // setupUi

    void retranslateUi(QWidget *joinprvgameClass)
    {
        joinprvgameClass->setWindowTitle(QApplication::translate("joinprvgameClass", "joinprvgame", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("joinprvgameClass", "Add icon", 0, QApplication::UnicodeUTF8));
        ok->setText(QApplication::translate("joinprvgameClass", "ok", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("joinprvgameClass", "cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class joinprvgameClass: public Ui_joinprvgameClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOINPRVGAME_H
