#ifndef NETCOUPLER_H
#define NETCOUPLER_H

#include<QObject>
#include<QTimer>
#include<QProcess>
#include<QHostInfo>
#include"hoststruct.h"
#include"userstruct.h"
#include"usermodel.h"
#include"hostmodel.h"
#include"global_macros.h"
class ircnet;
class snoppanet;
class QProcess;
class netcoupler : public QObject
{
    Q_OBJECT

public:    
    enum state{
        e_started, e_stoped
    };

    void start(QString nick);
    void stop();
    void sendmessage(const QString&,const QString&);
    void senduncheckedmessage(const QString&,const QString&);
    void sendnotice(const QString&,const QString&);
    void sendrawcommand(const QString&);
    void joinchannel(const QString&);
    void partchannel(const QString&);       
    void sendhostinfotoserverandhost(const QString &name,const QString &pwd, const QString &chan,int flag);
    void refreshlist();
    void refreshwho();    
    QString getmyhostip();
    QString nick;
    usermodel users;
    hostmodel hosts;
    QMap<QString,QString> schememap;
    QStringList channellist;

    void settingswindowemitfunktion();

    QStringList mutedusers;    
    void sendquit();
    int ircstate();

    state connectstate;

    DECLARE_SINGLETON(netcoupler);
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

    void sigsettingswindowchanged();
    void siggotidletime(const QString&, int);
    void signosuchnick(const QString&);
    void sigconnected();
    void sigdisconnected();  

    void sighoststarts(hoststruct);

private:
    ircnet *irc;
    snoppanet *http;
    QString ircip;
    QStringList listofjoinedchannels;

    QTimer hostlifetimer;
    QTimer getownhosttimer;
    int hosttimeoutdelay;

    QProcess *p;

    QTimer timer;
    QTimer loopTimer;           //to safe usergarbage every 30sec    

    QString getprocessstring();
    void startprocess(const QString&);
    void createhost(hoststruct);
    QString myip;
public slots:
    void joingamelink(const QString &);
    void joingame(const QString &,const QString&,const QString&);
    void getnotice(const QString&,const QString&,const QString&);
    void sendinfotochan(const QString &,const QString &);
    void refreshhostlist();
private slots:    
    void usesettingswindow(const QString &s="");    
    void getchannellist(QStringList);
    void getircip(QString);
    void gethostlist(QList<hoststruct>,QString);
    void getmsg(const QString&,const QString&,const QString&);
    void getwholist();    
    void getscheme(QString,QString);    

    void sendmessagetoallbuddys(const QString&);    
    void readprocess();    
    void ircconnected();
    void ircdisconnected();

    void getmywormnethost(hoststruct);

    friend class userstruct;

    void loopTimerTimeout();
};

#endif // NETCOUPLER_H
