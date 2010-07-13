#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui/QFrame>
#include "ui_about.h"

class about : public QFrame
{
    Q_OBJECT

public:
    about(QFrame *parent = 0);
    ~about();

    static QString version;
private:
    Ui::aboutClass ui;
};

#endif // ABOUT_H
