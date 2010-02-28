/********************************************************************************
** Form generated from reading UI file 'playername.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYERNAME_H
#define UI_PLAYERNAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_playernameClass
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *ok;

    void setupUi(QWidget *playernameClass)
    {
        if (playernameClass->objectName().isEmpty())
            playernameClass->setObjectName(QString::fromUtf8("playernameClass"));
        playernameClass->resize(400, 300);
        horizontalLayout = new QHBoxLayout(playernameClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(playernameClass);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lineEdit = new QLineEdit(playernameClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        ok = new QPushButton(playernameClass);
        ok->setObjectName(QString::fromUtf8("ok"));

        verticalLayout->addWidget(ok);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(playernameClass);

        QMetaObject::connectSlotsByName(playernameClass);
    } // setupUi

    void retranslateUi(QWidget *playernameClass)
    {
        playernameClass->setWindowTitle(QApplication::translate("playernameClass", "playername", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        ok->setText(QApplication::translate("playernameClass", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class playernameClass: public Ui_playernameClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYERNAME_H
