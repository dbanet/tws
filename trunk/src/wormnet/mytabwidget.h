/*
 * mytabwidget.h
 *
 *  Created on: 11.12.2008
 *      Author: looki
 */

#ifndef MYTABWIDGET_H_
#define MYTABWIDGET_H_
#include<QTabWidget>
class QMenu;
class QEvent;
class mytabwidget:public QTabWidget {
	Q_OBJECT
public:
	mytabwidget(QWidget *w=0);
	virtual ~mytabwidget();
private:
	QMenu *menu;
signals:
	void sigclosetab(int);
protected:
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif /* MYTABWIDGET_H_ */
