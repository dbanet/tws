#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QtGui/QFrame>
#include "ui_volumeslider.h"

class volumeslider : public QFrame
{
    Q_OBJECT

public:
    volumeslider(QFrame *parent = 0);
    ~volumeslider();
    double value();
    void setvalue(int);

private:
    Ui::volumesliderClass ui;
signals:
	void valueChanged(int);
};

#endif // VOLUMESLIDER_H
