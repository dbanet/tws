/*
 * uihelper.cpp
 *
 *  Created on: 31.10.2008
 *      Author: looki
 */

#include "uihelper.h"
#include<QtGui>
#include <QtGlobal>
uihelper::uihelper() {
        sl << "hosts" << "chat" << "msg" << "users"<<"send"<<"splitter1"<<"splitter2"<<"buttonlayout"<<"pbsmiley";
	wl.push_back((QObject**)&hosts);
	wl.push_back((QObject**)&chat);
	wl.push_back((QObject**)&msg);
	wl.push_back((QObject**)&users);
	wl.push_back((QObject**)&send);
	wl.push_back((QObject**)&splitter1);
	wl.push_back((QObject**)&splitter2);
	wl.push_back((QObject**)&buttonlayout);
        wl.push_back((QObject**)&pbsmiley);
	Q_ASSERT_X(wl.size()==sl.size(),"uihelper","size of the stringlist is unequal to the size of the vector");
	// TODO Auto-generated constructor stub
}
void uihelper::getchilds(QObject *w) {
	int i = 0;
	foreach(QString s,sl) {
			*wl[i] = w->findChild<QObject*> (s);                       
			Q_CHECK_PTR(*wl[i]);
			i++;
		}
}

uihelper::~uihelper() {
	// TODO Auto-generated destructor stub
}
