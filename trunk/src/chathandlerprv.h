/*
 * chathandler.h
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#ifndef CHATHANDLERPRV_H_
#define CHATHANDLERPRV_H_
#include<QObject>
#include<QPoint>
#include<QUrl>
#include<QTextCharFormat>
#include<QTextCursor>
#include<QMenu>
#include<QPointer>
#include "chathandler.h"
#include "emoticonhandler.h"
class QTextBrowser;
class QTextDocument;
class emoticonhandler;
class chathandlerprv: public chathandler {
	Q_OBJECT
public:
	chathandlerprv(QObject*,QTextBrowser*,QString);
	virtual ~chathandlerprv();
	void append(const QString&,const QString&,const QString&);
        void appendgarbage(const QString&);
        void appendhistory(const QString&);
	static QPointer<emoticonhandler> emot;
private:
private slots:
};

#endif /* chathandlerprv_H_ */
