/********************************************************************************
** Form generated from reading UI file 'initextbrowser.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INITEXTBROWSER_H
#define UI_INITEXTBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_initextbrowserClass
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QPushButton *save;
    QPushButton *quit;

    void setupUi(QWidget *initextbrowserClass)
    {
        if (initextbrowserClass->objectName().isEmpty())
            initextbrowserClass->setObjectName(QString::fromUtf8("initextbrowserClass"));
        initextbrowserClass->resize(401, 351);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(initextbrowserClass->sizePolicy().hasHeightForWidth());
        initextbrowserClass->setSizePolicy(sizePolicy);
        verticalLayoutWidget = new QWidget(initextbrowserClass);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 401, 351));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(verticalLayoutWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);
        textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextEditable|Qt::TextEditorInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textBrowser);

        save = new QPushButton(verticalLayoutWidget);
        save->setObjectName(QString::fromUtf8("save"));

        verticalLayout->addWidget(save);

        quit = new QPushButton(verticalLayoutWidget);
        quit->setObjectName(QString::fromUtf8("quit"));

        verticalLayout->addWidget(quit);


        retranslateUi(initextbrowserClass);
        QObject::connect(quit, SIGNAL(clicked()), initextbrowserClass, SLOT(close()));

        QMetaObject::connectSlotsByName(initextbrowserClass);
    } // setupUi

    void retranslateUi(QWidget *initextbrowserClass)
    {
        initextbrowserClass->setWindowTitle(QApplication::translate("initextbrowserClass", "initextbrowser", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("initextbrowserClass", "Save and quit", 0, QApplication::UnicodeUTF8));
        quit->setText(QApplication::translate("initextbrowserClass", "Quit without saving", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class initextbrowserClass: public Ui_initextbrowserClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITEXTBROWSER_H
