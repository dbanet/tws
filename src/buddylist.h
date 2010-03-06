#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QtGui/QWidget>
#include "ui_buddylist.h"

class buddylist : public QWidget
{
    Q_OBJECT

public:
    buddylist(QWidget *parent = 0);
    ~buddylist();

private:
    Ui::buddylistClass ui;
private slots:
	void add();
	void remove();
};

#endif // BUDDYLIST_H
