#ifndef ABOUT_H
#define ABOUT_H

#include <QFrame>
#include "ui_about.h"

class About : public QFrame
{
    Q_OBJECT

public:
    About(QFrame *parent = 0);

    static QString version;
private:
    Ui::aboutClass ui;
};

#endif // ABOUT_H
