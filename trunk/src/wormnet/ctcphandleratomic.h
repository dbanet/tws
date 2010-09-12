#ifndef CTCPHANDLERATOMIC_H
#define CTCPHANDLERATOMIC_H

#include <QtGui/QWidget>
#include "ui_ctcphandleratomic.h"

class ctcphandleratomic : public QWidget
{
    Q_OBJECT

public:
    ctcphandleratomic(QWidget *parent = 0);
    ~ctcphandleratomic();

    Ui::ctcphandleratomicClass ui;
private slots:
	void delclicked();
};

#endif // CTCPHANDLERATOMIC_H
