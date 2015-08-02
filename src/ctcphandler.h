/*
 * ctcphandler.h
 *
 *  Created on: 13.01.2009
 *      Author: looki
 */

#ifndef CTCPHANDLER_H_
#define CTCPHANDLER_H_
#include <QObject>
#include <QStringList>
#include <QMap>
#include "global_macros.h"

class chatwindow;
class usermessage;
class netcoupler;
class ctcphandler:public QObject {
    Q_OBJECT
public:
    ctcphandler(netcoupler *netc);
    ~ctcphandler();
    bool getctcp(const usermessage u);
    static QStringList awayusers;
    static QMap<QString,QString>  ctcpcontainer;
    QStringList bookedcommands;
    QMap<QString,bool> typingmap;
private:
    netcoupler *netc;
public slots:
signals:
    void sigctcpcommand(const QString&,const QString&);
};

#endif /* CTCPHANDLER_H_ */
