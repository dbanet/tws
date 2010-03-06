/*
 * ctcphandler.h
 *
 *  Created on: 13.01.2009
 *      Author: looki
 */

#ifndef CTCPHANDLER_H_
#define CTCPHANDLER_H_
#include<QObject>
#include<QStringList>
#include<QMap>
#include"global_macros.h"
class ctcphandler:public QObject {
    Q_OBJECT
    DECLARE_SINGLETON(ctcphandler);
public:
    bool getctcp(const QString&,const QString&);
    void typing(const QString&);
    static QStringList awayusers;
    static QMap<QString,QString>  ctcpcontainer;
    QStringList bookedcommands;
    QMap<QString,bool> typingmap;
public slots:
signals:
    void sigctcpcommand(const QString&,const QString&);
};

#endif /* CTCPHANDLER_H_ */
