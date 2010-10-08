/*
 * mytabwidget.cpp
 *
 *  Created on: 11.12.2008
 *      Author: looki
 */

#include "mytabwidget.h"
#include<QEvent>
#include<QMouseEvent>
#include<QTabBar>
#include<QCursor>
#include<QMenu>
#include<QApplication>
#include<QDialogButtonBox>
mytabwidget::mytabwidget(QWidget *w) :
	QTabWidget(w) {
	menu=new QMenu;
        menu->addAction(QDialogButtonBox::tr("&Close"));
	tabBar()->installEventFilter(this);
}
bool mytabwidget::eventFilter(QObject *obj, QEvent *e) {
        if (e->type() == QEvent::MouseButtonPress) {
                QMouseEvent *event = dynamic_cast<QMouseEvent *> (e);
		if(event){
			if(QApplication::mouseButtons().testFlag(Qt::RightButton)){
				QPoint p=QCursor::pos();
				QAction *a=menu->exec(QCursor::pos());
				if(a){
					int i=tabBar()->tabAt(this->mapFromGlobal(p));
					emit sigclosetab(i);
					return true;
				}
			}
		}
	}
        return QObject::eventFilter(obj, e);
}
mytabwidget::~mytabwidget() {
	// TODO Auto-generated destructor stub
}
