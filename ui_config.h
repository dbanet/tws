/********************************************************************************
** Form generated from reading UI file 'config.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_config
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *logButton;
    QPushButton *errButton;
    QPushButton *verlauf2Button;
    QPushButton *verlauf1Button;
    QPushButton *linkButton;

    void setupUi(QWidget *config)
    {
        if (config->objectName().isEmpty())
            config->setObjectName(QString::fromUtf8("config"));
        config->resize(426, 359);
        gridLayout_2 = new QGridLayout(config);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(config);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        logButton = new QPushButton(groupBox);
        logButton->setObjectName(QString::fromUtf8("logButton"));

        gridLayout->addWidget(logButton, 0, 0, 1, 1);

        errButton = new QPushButton(groupBox);
        errButton->setObjectName(QString::fromUtf8("errButton"));

        gridLayout->addWidget(errButton, 1, 0, 1, 1);

        verlauf2Button = new QPushButton(groupBox);
        verlauf2Button->setObjectName(QString::fromUtf8("verlauf2Button"));

        gridLayout->addWidget(verlauf2Button, 4, 0, 1, 1);

        verlauf1Button = new QPushButton(groupBox);
        verlauf1Button->setObjectName(QString::fromUtf8("verlauf1Button"));

        gridLayout->addWidget(verlauf1Button, 3, 0, 1, 1);

        linkButton = new QPushButton(groupBox);
        linkButton->setObjectName(QString::fromUtf8("linkButton"));

        gridLayout->addWidget(linkButton, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(config);

        QMetaObject::connectSlotsByName(config);
    } // setupUi

    void retranslateUi(QWidget *config)
    {
        config->setWindowTitle(QApplication::translate("config", "Einstellungen", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("config", "Farben", 0, QApplication::UnicodeUTF8));
        logButton->setText(QApplication::translate("config", "Log-Text", 0, QApplication::UnicodeUTF8));
        errButton->setText(QApplication::translate("config", "Error", 0, QApplication::UnicodeUTF8));
        verlauf2Button->setText(QApplication::translate("config", "Hystorie 2", 0, QApplication::UnicodeUTF8));
        verlauf1Button->setText(QApplication::translate("config", "Hystorie 1", 0, QApplication::UnicodeUTF8));
        linkButton->setText(QApplication::translate("config", "Link", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class config: public Ui_config {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
