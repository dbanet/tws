#ifndef irc_netcoupler_H
#define irc_netcoupler_H

#include <QObject>
#include<QTimer>
#include<QProcess>
#include<QHostInfo>
#include"src/wormnet/hoststruct.h"
#include"src/wormnet/userstruct.h"
#include"irc_usermodel.h"
#include"src/wormnet/hostmodel.h"
class irc_ircnet;
class snoppanet;
class QProcess;
class irc_netcoupler : public QObject
{
    Q_OBJECT

public:
    irc_netcoupler(QString,QObject *parent = 0,QString channel="#worms");
    ~irc_netcoupler();
    void sendmessage(const QString&,const QString&);
    void senduncheckedmessage(const QString&,const QString&);
    void sendnotice(const QString&,const QString&);
    void sendrawcommand(const QString&);
    void joinchannel(const QString&);
    void partchannel(const QString&);
    const QString nick;
    irc_usermodel users;
    QStringList channellist;

    void settingswindowemitfunktion();
    irc_ircnet *irc;

    userstruct getUser(QString user);
    QList<userstruct> &getUsersInChannel(QString channel);
    bool joinListContains(QString user);

    int countUsers();
    int countUsers(QString user);
    int countUsers(userstruct user);
    QStringList mutedusers;
signals:
    void siggotmsg(const QString&,const QString&,const QString&);
    void siggotnotice(const QString&,const QString&,const QString&);
    void siggotchanellist(QStringList);
    void sigusergarbage(const QString&,const QString&);
    void sigusergarbagejoin(const QString&,const QString&);
    void sigusergarbagepart(const QString&,const QString&);
    void sigusergarbagequit(const QString&,const QString&);

    void sigawaystringchanged();
    void sigsettingswindowchanged();
    void siggotidletime(const QString&, int);
    void signosuchnick(const QString&);
    void sigconnected();
    void sigdisconnected();
    void sigdisconnect();
    void sigreconnect();
    //void siggethostlist(QList<hoststruct>,QString);

private:
    QString ircip;
    QTimer timer;


    bool setawayingame;
    QString oldawaystring;

public slots:
    void getnotice(const QString&,const QString&,const QString&);
    void getFirstServerMessage();
private slots:
    void usesettingswindow(const QString &s="");
    void sendinfotochan(const QString &,const QString &);
    void getmsg(const QString&,const QString&,const QString&);
    void sendquit();

    void ircconnected();
    void ircdiconnected();

    void getjoinlist();
};

#endif // irc_netcoupler_H
