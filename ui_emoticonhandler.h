/********************************************************************************
** Form generated from reading UI file 'emoticonhandler.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMOTICONHANDLER_H
#define UI_EMOTICONHANDLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_emoticonhandlerClass
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QComboBox *cbscheme;
    QCheckBox *chbsmileys;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QPushButton *close;
    QGridLayout *gridLayout;
    QPushButton *remove;
    QPushButton *add;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *lineEdit;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *emoticonhandlerClass)
    {
        if (emoticonhandlerClass->objectName().isEmpty())
            emoticonhandlerClass->setObjectName(QString::fromUtf8("emoticonhandlerClass"));
        emoticonhandlerClass->resize(437, 323);
        horizontalLayout_4 = new QHBoxLayout(emoticonhandlerClass);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_2 = new QGroupBox(emoticonhandlerClass);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        cbscheme = new QComboBox(groupBox_2);
        cbscheme->setObjectName(QString::fromUtf8("cbscheme"));
        cbscheme->setMinimumContentsLength(6);

        verticalLayout_3->addWidget(cbscheme);

        chbsmileys = new QCheckBox(groupBox_2);
        chbsmileys->setObjectName(QString::fromUtf8("chbsmileys"));

        verticalLayout_3->addWidget(chbsmileys);


        verticalLayout_2->addWidget(groupBox_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listWidget = new QListWidget(emoticonhandlerClass);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMaximumSize(QSize(300, 16777215));

        verticalLayout->addWidget(listWidget);

        close = new QPushButton(emoticonhandlerClass);
        close->setObjectName(QString::fromUtf8("close"));
        close->setMaximumSize(QSize(300, 16777215));

        verticalLayout->addWidget(close);


        horizontalLayout->addLayout(verticalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        remove = new QPushButton(emoticonhandlerClass);
        remove->setObjectName(QString::fromUtf8("remove"));

        gridLayout->addWidget(remove, 0, 0, 1, 1);

        add = new QPushButton(emoticonhandlerClass);
        add->setObjectName(QString::fromUtf8("add"));

        gridLayout->addWidget(add, 2, 0, 1, 1);

        groupBox = new QGroupBox(emoticonhandlerClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 50));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout_4->addWidget(lineEdit);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_4->addLayout(verticalLayout_2);


        retranslateUi(emoticonhandlerClass);

        QMetaObject::connectSlotsByName(emoticonhandlerClass);
    } // setupUi

    void retranslateUi(QWidget *emoticonhandlerClass)
    {
        emoticonhandlerClass->setWindowTitle(QApplication::translate("emoticonhandlerClass", "emoticonhandler", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("emoticonhandlerClass", "Select a Scheme", 0, QApplication::UnicodeUTF8));
        chbsmileys->setText(QApplication::translate("emoticonhandlerClass", "Dont show smileys.", 0, QApplication::UnicodeUTF8));
        close->setText(QApplication::translate("emoticonhandlerClass", "Close", 0, QApplication::UnicodeUTF8));
        remove->setText(QApplication::translate("emoticonhandlerClass", "Remove this item", 0, QApplication::UnicodeUTF8));
        add->setText(QApplication::translate("emoticonhandlerClass", "Add a smiley.", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("emoticonhandlerClass", "First set a Shortcut.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class emoticonhandlerClass: public Ui_emoticonhandlerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOTICONHANDLER_H
