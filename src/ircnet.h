#ifndef IRCNET_H
#define IRCNET_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include "userstruct.h"
#include "usermessage.h"
class netcoupler;
class ircnet : public QObject
{
    Q_OBJECT

public:
    ircnet(netcoupler *netc,QString s, QObject *parent = 0);
//    enum {
//        QUIT=1,PRIVMSG,PART,JOIN,NOTICE
//    };

    void joinchannel(const QString&);    

    void sendusermessage(const usermessage u);

    void setip(const QString&);
    void refreshlist();
    void who();
    void partchannel(const QString&);
    void quit(QString s);
    QHash<QString/* channel name */,int/* amount of  users */> channellist;
    int state() const;
    QList<userstruct> userList;
    QHash<QString,QStringList> joinlist;
    QString nick;
    QString canonizeChannelName(QString);
public slots:
    void start();
    void reconnect();
    void connected();
    void disconnected();
    void tcpread();
signals: //public signals:
    void sigmsg(const QString &user,const QString &receiver,const QString &msg);
    void siggotusermessage(const usermessage u);
    void sigIRCReceivedChanList(QStringList);
    void sigIRCJoinedChannel(QString,int);
    void sigIRCUpdatedAmountOfUsers(QString,int);
    void sigIRCUpdatedUserList(QList<userstruct>*);
    //private signals:
    void sigconnected();
    void sigdisconnected();   
    void siggotidletime(const QString&,int,int);
    void signosuchnick(const QString&);
    void sigconnectingfailed();

private:
    netcoupler *netc;
    bool justgetlist;

    QList<userstruct> templist;

    QStringList joinedchannellist;
    QStringList tempchannellist;
    QString ircreadstring;
    QString wnip;
    QString servermessageindicator;
    QTcpSocket *tcp;
    QTimer *disconnectionTimeout;
    void tcp_write(const QString &msg);

private slots:
    void tcpError(QAbstractSocket::SocketError);
    void disconnectionTimedOut();

};

#endif // IRCNET_H
