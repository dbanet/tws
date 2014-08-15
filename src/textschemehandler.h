#ifndef TEXTSCHEMEHANDLER_H
#define TEXTSCHEMEHANDLER_H

#include <QWidget>
#include "ui_textschemehandler.h"
#include <QTextCharFormat>

class TextSchemeHandler: public QWidget {
Q_OBJECT

public:
	TextSchemeHandler(QWidget *parent = 0);
	~TextSchemeHandler();

private:
	Ui::textschemehandlerClass ui;
        QHash<QString,QTextCharFormat*> map;
private slots:
	void closeclicked();
	void fontclicked();
	void colorclicked();
	void showclicked();
};

#endif // TEXTSCHEMEHANDLER_H
