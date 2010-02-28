/********************************************************************************
** Form generated from reading UI file 'combobox_wrapper.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMBOBOX_WRAPPER_H
#define UI_COMBOBOX_WRAPPER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_combobox_wrapper
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *down;
    QPushButton *up;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *plus_2;
    QPushButton *minus;

    void setupUi(QWidget *combobox_wrapper)
    {
        if (combobox_wrapper->objectName().isEmpty())
            combobox_wrapper->setObjectName(QString::fromUtf8("combobox_wrapper"));
        combobox_wrapper->resize(366, 67);
        verticalLayout = new QVBoxLayout(combobox_wrapper);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comboBox = new QComboBox(combobox_wrapper);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(true);
        comboBox->setInsertPolicy(QComboBox::InsertAtCurrent);

        verticalLayout->addWidget(comboBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        down = new QPushButton(combobox_wrapper);
        down->setObjectName(QString::fromUtf8("down"));
        down->setMinimumSize(QSize(20, 20));
        down->setMaximumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8("snppictures/buttons/down_arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        down->setIcon(icon);

        horizontalLayout->addWidget(down);

        up = new QPushButton(combobox_wrapper);
        up->setObjectName(QString::fromUtf8("up"));
        up->setMinimumSize(QSize(20, 20));
        up->setMaximumSize(QSize(20, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("snppictures/buttons/up_arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        up->setIcon(icon1);

        horizontalLayout->addWidget(up);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(combobox_wrapper);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(20, 20));
        label_2->setMaximumSize(QSize(20, 20));
        label_2->setPixmap(QPixmap(QString::fromUtf8("snppictures/Chat_Icon.png")));
        label_2->setScaledContents(true);

        horizontalLayout->addWidget(label_2);

        label = new QLabel(combobox_wrapper);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        plus_2 = new QPushButton(combobox_wrapper);
        plus_2->setObjectName(QString::fromUtf8("plus_2"));
        plus_2->setMinimumSize(QSize(20, 20));
        plus_2->setMaximumSize(QSize(20, 20));

        horizontalLayout->addWidget(plus_2);

        minus = new QPushButton(combobox_wrapper);
        minus->setObjectName(QString::fromUtf8("minus"));
        minus->setMinimumSize(QSize(20, 20));
        minus->setMaximumSize(QSize(20, 20));

        horizontalLayout->addWidget(minus);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(combobox_wrapper);

        QMetaObject::connectSlotsByName(combobox_wrapper);
    } // setupUi

    void retranslateUi(QWidget *combobox_wrapper)
    {
        combobox_wrapper->setWindowTitle(QApplication::translate("combobox_wrapper", "Form", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        down->setToolTip(QApplication::translate("combobox_wrapper", "Move down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        down->setText(QString());
#ifndef QT_NO_TOOLTIP
        up->setToolTip(QApplication::translate("combobox_wrapper", "Move up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        up->setText(QString());
        label_2->setText(QString());
        label->setText(QApplication::translate("combobox_wrapper", "To finish editing, you must press enter.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        plus_2->setToolTip(QApplication::translate("combobox_wrapper", "Insert a word", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        plus_2->setText(QApplication::translate("combobox_wrapper", "+", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        minus->setToolTip(QApplication::translate("combobox_wrapper", "remove this word", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        minus->setText(QApplication::translate("combobox_wrapper", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class combobox_wrapper: public Ui_combobox_wrapper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMBOBOX_WRAPPER_H
