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

#include "chathandler.h"
#include "emoticonhandler.h"
class QTextBrowser;
class QTextDocument;

class usermessage;
class chathandlerprv: public chathandler {
    Q_OBJECT
public:
    chathandlerprv(QObject*,QTextBrowser*,QString);
    virtual ~chathandlerprv();    
private slots:
    void opencontextmenu(const QPoint &);
private:
    virtual QList<QPair<QVariant, QTextCharFormat> > getSegmentation(QString s, QTextCharFormat format);
    virtual QTextCharFormat getRightFormat(const usermessage u);   
};

#endif /* chathandlerprv_H_ */
