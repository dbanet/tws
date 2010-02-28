/*
 * uihelper.h
 *
 *  Created on: 31.10.2008
 *      Author: looki
 */

#ifndef UIHELPER_H_
#define UIHELPER_H_
#include<QStringList>
class QTextBrowser;
class QWidget;
class QTreeView;
class QLineEdit;
class QPushButton;
class QSplitter;
class QHBoxLayout;
class uihelper {
public:
	uihelper();
	void getchilds(QObject *w);
	virtual ~uihelper();
	QList<QObject**> wl;
	QTreeView *hosts;
	QTextBrowser *chat;
	QLineEdit *msg;
	QTreeView *users;
	QPushButton *send;
	QSplitter *splitter1;
	QSplitter *splitter2;
	QHBoxLayout *buttonlayout;

	QStringList sl;
};

#endif /* UIHELPER_H_ */
