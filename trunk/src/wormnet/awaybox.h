#ifndef AWAYBOX_H
#define AWAYBOX_H

#include <QtGui/QWidget>
#include "ui_awaybox.h"

class awaybox : public QWidget
{
    Q_OBJECT

public:
    awaybox(QWidget *parent = 0);
    ~awaybox();
    static bool ison;

private slots:
	void okclicked();
	void cancelclicked();
	void backclicked();
	void forwardclicked();
private:
	QStringList sl;
	int counter;
	int i;
    Ui::awayboxClass ui;
protected:
	void closeEvent ( QCloseEvent *);
        bool eventFilter(QObject * o, QEvent *e);
signals:
	void sigok();
};

#endif // AWAYBOX_H
