/********************************************************************************
** Form generated from reading UI file 'qmainwindow.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMAINWINDOW_H
#define UI_QMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *widgetList;
    QFrame *container;
    QVBoxLayout *verticalLayout;
    QWidget *uploadWidget;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QPushButton *buttonStart;
    QLabel *label_2;
    QProgressBar *pBar;
    QListWidget *fileWidget;
    QPushButton *addButton;
    QPushButton *delButton;
    QSpacerItem *verticalSpacer;
    QWidget *logWidget;
    QGridLayout *gridLayout;
    QTextBrowser *logNew;
    QWidget *verlaufWidget;
    QGridLayout *gridLayout_3;
    QTextBrowser *logOld;
    QPushButton *delHistoryButton;
    QWidget *aboutWidget;
    QGridLayout *gridLayout_4;
    QTextBrowser *aboutBrowser;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(538, 598);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
        horizontalLayout = new QHBoxLayout(MainWindow);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widgetList = new QListWidget(MainWindow);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(widgetList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/diag.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(widgetList);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/verlauf.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(widgetList);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(widgetList);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(widgetList);
        __qlistwidgetitem4->setIcon(icon4);
        widgetList->setObjectName(QString::fromUtf8("widgetList"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetList->sizePolicy().hasHeightForWidth());
        widgetList->setSizePolicy(sizePolicy1);
        widgetList->setMinimumSize(QSize(150, 0));
        widgetList->setMaximumSize(QSize(150, 16777215));
        widgetList->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(widgetList);

        container = new QFrame(MainWindow);
        container->setObjectName(QString::fromUtf8("container"));
        container->setFrameShape(QFrame::StyledPanel);
        container->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(container);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        uploadWidget = new QWidget(container);
        uploadWidget->setObjectName(QString::fromUtf8("uploadWidget"));
        gridLayout_2 = new QGridLayout(uploadWidget);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(uploadWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label, 0, 0, 1, 2);

        buttonStart = new QPushButton(uploadWidget);
        buttonStart->setObjectName(QString::fromUtf8("buttonStart"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonStart->setIcon(icon5);
        buttonStart->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(buttonStart, 5, 0, 1, 2);

        label_2 = new QLabel(uploadWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_2, 6, 0, 1, 2);

        pBar = new QProgressBar(uploadWidget);
        pBar->setObjectName(QString::fromUtf8("pBar"));
        pBar->setMaximum(1);
        pBar->setValue(0);
        pBar->setInvertedAppearance(false);

        gridLayout_2->addWidget(pBar, 7, 0, 1, 2);

        fileWidget = new QListWidget(uploadWidget);
        fileWidget->setObjectName(QString::fromUtf8("fileWidget"));

        gridLayout_2->addWidget(fileWidget, 2, 0, 3, 1);

        addButton = new QPushButton(uploadWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy3);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/file-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addButton->setIcon(icon6);
        addButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(addButton, 2, 1, 1, 1);

        delButton = new QPushButton(uploadWidget);
        delButton->setObjectName(QString::fromUtf8("delButton"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/file-remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        delButton->setIcon(icon7);
        delButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(delButton, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 1, 1, 1);


        verticalLayout->addWidget(uploadWidget);

        logWidget = new QWidget(container);
        logWidget->setObjectName(QString::fromUtf8("logWidget"));
        gridLayout = new QGridLayout(logWidget);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        logNew = new QTextBrowser(logWidget);
        logNew->setObjectName(QString::fromUtf8("logNew"));
        logNew->setOpenExternalLinks(true);

        gridLayout->addWidget(logNew, 0, 0, 1, 1);


        verticalLayout->addWidget(logWidget);

        verlaufWidget = new QWidget(container);
        verlaufWidget->setObjectName(QString::fromUtf8("verlaufWidget"));
        gridLayout_3 = new QGridLayout(verlaufWidget);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        logOld = new QTextBrowser(verlaufWidget);
        logOld->setObjectName(QString::fromUtf8("logOld"));
        logOld->setOpenExternalLinks(true);

        gridLayout_3->addWidget(logOld, 0, 0, 1, 1);

        delHistoryButton = new QPushButton(verlaufWidget);
        delHistoryButton->setObjectName(QString::fromUtf8("delHistoryButton"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        delHistoryButton->setIcon(icon8);
        delHistoryButton->setIconSize(QSize(24, 24));

        gridLayout_3->addWidget(delHistoryButton, 1, 0, 1, 1);


        verticalLayout->addWidget(verlaufWidget);

        aboutWidget = new QWidget(container);
        aboutWidget->setObjectName(QString::fromUtf8("aboutWidget"));
        gridLayout_4 = new QGridLayout(aboutWidget);
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        aboutBrowser = new QTextBrowser(aboutWidget);
        aboutBrowser->setObjectName(QString::fromUtf8("aboutBrowser"));
        aboutBrowser->setOpenExternalLinks(true);

        gridLayout_4->addWidget(aboutBrowser, 0, 0, 1, 1);


        verticalLayout->addWidget(aboutWidget);


        horizontalLayout->addWidget(container);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QFileUpload", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = widgetList->isSortingEnabled();
        widgetList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = widgetList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "Upload", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = widgetList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = widgetList->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "Hystorie", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = widgetList->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = widgetList->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
        widgetList->setSortingEnabled(__sortingEnabled);

        label->setText(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        buttonStart->setText(QApplication::translate("MainWindow", "Send", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Progress", 0, QApplication::UnicodeUTF8));
        pBar->setFormat(QApplication::translate("MainWindow", "%v Byte", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        delButton->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        delHistoryButton->setText(QApplication::translate("MainWindow", "Delete history", 0, QApplication::UnicodeUTF8));
        aboutBrowser->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt;\"><span style=\" font-family:'Sans Serif'; font-size:11pt; font-weight:600;\">QFileUploader</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:11pt; font-weight:600;\"><span style=\" font-size:9pt;\">Uploader for </span><a href=\"http://www.file-upload.net\"><span style=\" text-decoration: underline; color:#0000ff;\">file-upload.net</span></a></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margi"
                        "n-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"mailto:markus.kuenkler@gmx.de\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#0000ff;\">markus.kuenkler@gmx.de</span></a></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#0000ff;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#0000ff;\"><span style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:600; text-decoration:none; color:#000000;\">Credits</span></p>\n"
"<p style=\"-qt-paragraph-type:emp"
                        "ty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#0000ff;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#0000ff;\"><span style=\" text-decoration:none; color:#000000;\">Icons from Oxygen Project</span><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt;\"><span style=\" font-size:x-large;\">About Qt</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8pt;\"> Qt is a C++ toolkit for cross-platform application development.Qt provides single-source portability across MS"
                        " Windows, Mac OS X,Linux and all major commercial Unix variants. Qt is also available forembedded devices as Qtopia Core.The Qt Open Source Edition is available to the Open Source communityunder Trolltech's Dual Licensing Model. The Open Source Edition of Qtis freely available for the development of Open Source softwaregoverned by the GNU General Public License (GPL).See http://www.trolltech.com/qt/ for more information.</p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMAINWINDOW_H
