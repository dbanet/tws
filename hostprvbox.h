#ifndef HOSTPRVBOX_H
#define HOSTPRVBOX_H

#include <QtGui/QWidget>
#include<QLabel>
#include "ui_hostprvbox.h"

class hostprvbox : public QWidget
{
    Q_OBJECT

public:
    hostprvbox(QWidget *parent = 0);
    ~hostprvbox();
    QString gamename;
       QString pwd;
       QString icon;

   private:
       Ui::hostprvboxClass ui;
       QLabel schemelabel;
private slots:
   	void addclicked();
   	void okclicked();
   	void okjustclicked();
   	void cancelclicked();
	void showschemeclicked();
	void copyslot();
   protected:
   	void showEvent ( QShowEvent * event );
signals:
   	void sigok(const QString&);
 };

#endif // HOSTPRVBOX_H
