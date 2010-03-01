#ifndef HOSTBOX_H
#define HOSTBOX_H

#include <QtGui/QWidget>
#include "ui_hostbox.h"

class hostbox : public QWidget
{
    Q_OBJECT

public:
    hostbox(QString,QWidget *parent = 0);
    ~hostbox();
    QString gamename;
    QString pwd;
    QString icon;
    const QString channel;

private:
    Ui::hostboxClass ui;
    QRegExpValidator *validator;
private slots:
        void on_pbshowdetails_clicked();
 void addclicked();
	void okclicked();
	void cancelclicked();
protected:
        bool eventFilter(QObject *obj, QEvent *event);
	void showEvent ( QShowEvent * event );
signals:
	void sigok();
};

#endif // HOSTBOX_H
