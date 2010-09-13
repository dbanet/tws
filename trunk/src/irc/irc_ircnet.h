#ifndef irc_ircnet_H
#define irc_ircnet_H

#include <QObject>
#include<QTcpSocket>
#include<QStringList>
#include<QTimer>
#include"src/wormnet/userstruct.h"
class irc_ircnet : public QObject
{
    Q_OBJECT

public:
    irc_ircnet(QString s, QObject *parent = 0,QString channel="#worms");
    ~irc_ircnet();
    void joinchannel(const QString&);
    void sendmessage(const QString&, const QString&);
    void sendnotice(const QString&, const QString&);
    void sendrawcommand(const QString&);
    void setip(const QString&);
    void partchannel(const QString&);
    void quit();
    QList<userstruct> wholist;
    QMap<QString,QList<userstruct> > joinlist;
    QString nick;
    QString ircChannel;

    QStringList listofjoinedchannels;
public slots:
    void start();
    void connected();
    void disconnected();
    void tcpread();
signals: //public signals:
    void sigmsg(const QString&,const QString&,const QString&);
    void signotice(const QString&,const QString&,const QString&);
    void siggetlist(QStringList);
    //private signals:
    void sigconnected();
    void sigdisconnected();
    void sigusergarbage(const QString&,const QString&);
    void sigusergarbagejoin(const QString&,const QString&);
    void sigusergarbagepart(const QString&,const QString&);
    void sigusergarbagequit(const QString&,const QString&);
    void siggotidletime(const QString&, int);
    void signosuchnick(const QString&);
    void sigreconnect();

    void sigFirstServerMessage();

private:

    QStringList channellist;
    QStringList tempchannellist;
    QString ircreadstring;
    QString wnip;
    QString servermessageindicator;
    QTcpSocket *tcp;
    QTimer reconnecttimer;
    void readservermassege(QString);
    void readusermessage(QString &);
    void gotusergarbage(QString&,QString&);

    QMap<QString,int> mapusercommand;    

private slots:
    void tcpError(QAbstractSocket::SocketError);
};

#endif // irc_ircnet_H
