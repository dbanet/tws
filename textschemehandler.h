#ifndef TEXTSCHEMEHANDLER_H
#define TEXTSCHEMEHANDLER_H

#include <QtGui/QWidget>
#include "ui_textschemehandler.h"
#include<QTextCharFormat>

class textschemehandler: public QWidget {
Q_OBJECT

public:
	textschemehandler(QWidget *parent = 0);
	~textschemehandler();

private:
	Ui::textschemehandlerClass ui;
	QMap<QString,QTextCharFormat*> map;
private slots:
	void closeclicked();
	void fontclicked();
	void colorclicked();
	void showclicked();

};

#endif // TEXTSCHEMEHANDLER_H
