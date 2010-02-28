/********************************************************************************
** Form generated from reading UI file 'buttonlayout.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONLAYOUT_H
#define UI_BUTTONLAYOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_buttonlayoutClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QScrollArea *chatwindowbuttonscrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbrefresh;
    QPushButton *pbhost;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QPushButton *pbsort;
    QPushButton *pbcostumwords;
    QPushButton *pbsound;
    QPushButton *pbballoon;
    QFrame *line_4;
    QPushButton *pbminimize;
    QFrame *line_2;
    QSlider *slideralpha;
    QFrame *line_3;

    void setupUi(QWidget *buttonlayoutClass)
    {
        if (buttonlayoutClass->objectName().isEmpty())
            buttonlayoutClass->setObjectName(QString::fromUtf8("buttonlayoutClass"));
        buttonlayoutClass->resize(941, 23);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonlayoutClass->sizePolicy().hasHeightForWidth());
        buttonlayoutClass->setSizePolicy(sizePolicy);
        buttonlayoutClass->setMinimumSize(QSize(50, 0));
        buttonlayoutClass->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_2 = new QHBoxLayout(buttonlayoutClass);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        chatwindowbuttonscrollArea = new QScrollArea(buttonlayoutClass);
        chatwindowbuttonscrollArea->setObjectName(QString::fromUtf8("chatwindowbuttonscrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(chatwindowbuttonscrollArea->sizePolicy().hasHeightForWidth());
        chatwindowbuttonscrollArea->setSizePolicy(sizePolicy1);
        chatwindowbuttonscrollArea->setMinimumSize(QSize(0, 23));
        chatwindowbuttonscrollArea->setMaximumSize(QSize(16777215, 23));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(251, 248, 241, 0));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        chatwindowbuttonscrollArea->setPalette(palette);
        chatwindowbuttonscrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        chatwindowbuttonscrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 989, 23));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbrefresh = new QPushButton(scrollAreaWidgetContents);
        pbrefresh->setObjectName(QString::fromUtf8("pbrefresh"));
        pbrefresh->setMinimumSize(QSize(170, 20));
        pbrefresh->setMaximumSize(QSize(200, 20));

        horizontalLayout->addWidget(pbrefresh);

        pbhost = new QPushButton(scrollAreaWidgetContents);
        pbhost->setObjectName(QString::fromUtf8("pbhost"));
        pbhost->setMinimumSize(QSize(120, 20));
        pbhost->setMaximumSize(QSize(100, 20));

        horizontalLayout->addWidget(pbhost);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        line = new QFrame(scrollAreaWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        pbsort = new QPushButton(scrollAreaWidgetContents);
        pbsort->setObjectName(QString::fromUtf8("pbsort"));

        horizontalLayout->addWidget(pbsort);

        pbcostumwords = new QPushButton(scrollAreaWidgetContents);
        pbcostumwords->setObjectName(QString::fromUtf8("pbcostumwords"));

        horizontalLayout->addWidget(pbcostumwords);

        pbsound = new QPushButton(scrollAreaWidgetContents);
        pbsound->setObjectName(QString::fromUtf8("pbsound"));

        horizontalLayout->addWidget(pbsound);

        pbballoon = new QPushButton(scrollAreaWidgetContents);
        pbballoon->setObjectName(QString::fromUtf8("pbballoon"));

        horizontalLayout->addWidget(pbballoon);

        line_4 = new QFrame(scrollAreaWidgetContents);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_4);

        pbminimize = new QPushButton(scrollAreaWidgetContents);
        pbminimize->setObjectName(QString::fromUtf8("pbminimize"));
        pbminimize->setMinimumSize(QSize(170, 20));
        pbminimize->setMaximumSize(QSize(200, 20));

        horizontalLayout->addWidget(pbminimize);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        slideralpha = new QSlider(scrollAreaWidgetContents);
        slideralpha->setObjectName(QString::fromUtf8("slideralpha"));
        slideralpha->setMinimumSize(QSize(100, 0));
        slideralpha->setMaximumSize(QSize(150, 20));
        slideralpha->setMinimum(20);
        slideralpha->setMaximum(100);
        slideralpha->setValue(20);
        slideralpha->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(slideralpha);

        line_3 = new QFrame(scrollAreaWidgetContents);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_3);

        horizontalLayout->setStretch(12, 1);
        chatwindowbuttonscrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(chatwindowbuttonscrollArea);


        retranslateUi(buttonlayoutClass);

        QMetaObject::connectSlotsByName(buttonlayoutClass);
    } // setupUi

    void retranslateUi(QWidget *buttonlayoutClass)
    {
        buttonlayoutClass->setWindowTitle(QApplication::translate("buttonlayoutClass", "buttonlayout", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonlayoutClass->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        pbrefresh->setText(QApplication::translate("buttonlayoutClass", "Refresh the Hostlist", 0, QApplication::UnicodeUTF8));
        pbhost->setText(QApplication::translate("buttonlayoutClass", "Host", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbsort->setToolTip(QApplication::translate("buttonlayoutClass", "If Sorting is off, users will appear in the direction in that they came into wormnet.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbsort->setText(QApplication::translate("buttonlayoutClass", "Sort", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbcostumwords->setToolTip(QApplication::translate("buttonlayoutClass", "Go to Settings Alert and Away tab to set costum words.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbcostumwords->setText(QApplication::translate("buttonlayoutClass", "Costum words", 0, QApplication::UnicodeUTF8));
        pbsound->setText(QApplication::translate("buttonlayoutClass", "Sound", 0, QApplication::UnicodeUTF8));
        pbballoon->setText(QApplication::translate("buttonlayoutClass", "Balloons", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbminimize->setToolTip(QApplication::translate("buttonlayoutClass", "To raise the window again, leftclick the trayicon.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbminimize->setText(QApplication::translate("buttonlayoutClass", "Minimize to tray", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class buttonlayoutClass: public Ui_buttonlayoutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONLAYOUT_H
