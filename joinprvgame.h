#ifndef JOINPRVGAME_H
#define JOINPRVGAME_H

#include <QtGui/QWidget>
#include "ui_joinprvgame.h"

class joinprvgame : public QWidget
{
    Q_OBJECT

public:
    joinprvgame(const QString&,const QString&);
    ~joinprvgame();
    const QString chan;
private slots:
	void addclicked();
	void okclicked();
	void cancelclicked();
private:
	QString anchor;
	Ui::joinprvgameClass ui;
signals:
	void sigjoingamelink(const QString &);

};

#endif // JOINPRVGAME_H
