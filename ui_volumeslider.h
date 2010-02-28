/********************************************************************************
** Form generated from reading UI file 'volumeslider.ui'
**
** Created: Sun Feb 14 00:16:16 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUMESLIDER_H
#define UI_VOLUMESLIDER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_volumesliderClass
{
public:
    QHBoxLayout *horizontalLayout;
    QSlider *horizontalSlider;

    void setupUi(QWidget *volumesliderClass)
    {
        if (volumesliderClass->objectName().isEmpty())
            volumesliderClass->setObjectName(QString::fromUtf8("volumesliderClass"));
        volumesliderClass->resize(400, 76);
        horizontalLayout = new QHBoxLayout(volumesliderClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSlider = new QSlider(volumesliderClass);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMaximum(10);
        horizontalSlider->setSingleStep(0);
        horizontalSlider->setValue(5);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksBothSides);
        horizontalSlider->setTickInterval(0);

        horizontalLayout->addWidget(horizontalSlider);


        retranslateUi(volumesliderClass);

        QMetaObject::connectSlotsByName(volumesliderClass);
    } // setupUi

    void retranslateUi(QWidget *volumesliderClass)
    {
        volumesliderClass->setWindowTitle(QApplication::translate("volumesliderClass", "volumeslider", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class volumesliderClass: public Ui_volumesliderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMESLIDER_H
