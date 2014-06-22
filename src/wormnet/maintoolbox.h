#ifndef MAINTOOLBOX_H
#define MAINTOOLBOX_H

#include <QtGui/QWidget>
class mytabwidget;
class QVBoxLayout;
class maintoolbox : public QWidget
{
    Q_OBJECT

public:
    maintoolbox(QString,QWidget *parent = 0);
    ~maintoolbox();
    void addremovetab();
    void alert(QWidget*);
private:
	mytabwidget *tab;
	QVBoxLayout *layout;
	QList<QWidget*> widgetlist;

public slots:
	void addwidget(QWidget*);
	void closewindow(int);
	void removeallwidgets();
	void activateone(QWidget*);
signals:
	void sigclosewindow(QWidget *);
};

#endif // MAINTOOLBOX_H
