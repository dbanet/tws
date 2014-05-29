#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QWidget>
#include "ui_buddylist.h"

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    BuddyList(QWidget *parent = 0);

private:
    Ui::buddylistClass ui;
private slots:
	void add();
	void remove();
};

#endif // BUDDYLIST_H
