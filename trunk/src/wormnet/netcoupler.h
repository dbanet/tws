#ifndef NETCOUPLER_H
#define NETCOUPLER_H

#include <QObject>
#include<QTimer>
#include<QProcess>
#include<QHostInfo>
#include"hoststruct.h"
#include"userstruct.h"
#include"usermodel.h"
#include"hostmodel.h"
class ircnet;
class snoppanet;
class QProcess;
class netcoupler : public QObject
{
    Q_OBJECT

public:
    netcoupler(QString,QObject *parent = 0);
    ~netcoupler();
    void sendmessage(const QString&,const QString&);
    void senduncheckedmessage(const QString&,const QString&);
    void sendnotice(const QString&,const QString&);
    void sendrawcommand(const QString&);
    void joinchannel(const QString&);
    void partchannel(const QString&);
    void createhost(const QString &name, const QString &chan);
    void createprvhost(const QString &chan,const QString &scheme);
    void sendHostInfoToChan(const QString &name,const QString &pwd, const QString &chan,const QString &flag);
    void refreshlist();
    void copyprvhosttoclipboard(const QString &);
    void sendprvhosttobuddys(const QString &);
    void refreshwho();
    void setaway(const QString &);
    QString awaymessage;
    bool isaway;
    bool wasaway;
    const QString nick;
    usermodel users;
    hostmodel hosts;
    QMap<QString,QString> schememap;
    QStringList channellist;

    void settingswindowemitfunktion();

    QStringList mutedusers;
    QString myip;
signals:
    void signewwholist(QList<userstruct>);
    void siggotmsg(const QString&,const QString&,const QString&);
    void siggotnotice(const QString&,const QString&,const QString&);
    void siggotprivmsg(const QString&,const QString&,const QString&);
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
    ircnet *irc;
    snoppanet *http;
    QString ircip;
    QStringList listofjoinedchannels;

    QTimer hostlifetimer;
    QTimer getownhosttimer;
    int hosttimeoutdelay;

    static QProcess *p;

    QTimer timer;

    bool setawayingame;
    QString awaystringwhilehosting;
    QString oldawaystring;

    QString getprocessstring();
    void startprocess(const QString&);
public slots:
    void joingamelink(const QString &);
    void joingame(const QString &,const QString&,const QString&);
    void getnotice(const QString&,const QString&,const QString&);
    void sendinfotochan(const QString &,const QString &);
    void refreshhostlist();
private slots:
    void setaway();
    void usesettingswindow(const QString &s="");    
    void getchannellist(QStringList);
    void getircip(QString);
    void gethostlist(QList<hoststruct>,QString);
    void getmsg(const QString&,const QString&,const QString&);
    void getwholist();
    void sendquit();
    void getscheme(QString,QString);    

    void sendmessagetoallbuddys(const QString&);
    void processfinished(int , QProcess::ExitStatus);
    void readprocess();    
    void ircconnected();
    void ircdiconnected();
};

#endif // NETCOUPLER_H
