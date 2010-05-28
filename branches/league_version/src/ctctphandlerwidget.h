#ifndef CTCTPHANDLERWIDGET_H
#define CTCTPHANDLERWIDGET_H

#include "ui_ctctphandlerwidget.h"
#include"global_macros.h"
#include<QPointer>
#include <QtGui/QFrame>
class ctcphandleratomic;
class ctctphandlerwidget : public QFrame
{
    Q_OBJECT
    DECLARE_SINGLETON(ctctphandlerwidget);
public:    
    QList<QPointer<ctcphandleratomic> > atomiclist;
    QMap<QString,QPointer<ctcphandleratomic> > atomicmap;

private:
	void addcommand(QString);
	void addsalary(QString,QString);
    Ui::ctctphandlerwidgetClass ui;
private slots:
	void safeandclose();
	void addcommandbyhand();
	void pbhowtoclicked();
};

#endif // CTCTPHANDLERWIDGET_H
