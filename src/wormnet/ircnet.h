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
class ircnet : public QObject
{
    Q_OBJECT

public:
    ircnet(QString s, QObject *parent = 0);
//    enum {
//        QUIT=1,PRIVMSG,PART,JOIN,NOTICE
//    };

    void joinChannel(const QString&);

    void sendUserMessage(const usermessage u);

    void setIP(const QString&);
    void refreshList();
    void who();
    void partChannel(const QString&);
    void quit(QString s);
    QHash<QString/* channel name */,int/* amount of  users */> channellist;
    int state() const;
    QList<userstruct> userList;
    QString nick;
    QString canonizeChannelName(QString);
public slots:
    void start();
    void reconnect();
    void connected();
    void disconnected();
    void tcpRead();
signals: //public signals:
    void sigMessage(const QString &user,const QString &receiver,const QString &msg);
    void sigGotUserMessage(const usermessage u);
    void sigIRCReceivedChanList(QStringList);
    void sigIRCJoinedChannel(QString,int);
    void sigIRCUpdatedAmountOfUsers(QString,int);
    //private signals:
    void sigConnected();
    void sigDisconnected();
    void sigGotIdleTime(const QString&,int,int);
    void sigNoSuchNick(const QString&);
    void sigConnectionFailed();

private:
    bool justGotList;
    QStringList joinedChannels;
    QString ircReadString;
    QString wnip;
    QTcpSocket *tcp;
    QTimer *disconnectionTimeout;
    void disconnectionTimedOut();
    void tcpWrite(const QString &msg);

private slots:
    void tcpError(QAbstractSocket::SocketError);
};

#endif // IRCNET_H
