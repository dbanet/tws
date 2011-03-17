#ifndef NETCOUPLER_H
#define NETCOUPLER_H

#include<QObject>
#include<QTimer>
#include<QProcess>
#include<QHostInfo>
#include<QPointer>

#include"hoststruct.h"
#include"userstruct.h"
#include"usermodel.h"
#include"hostmodel.h"
#include"global_macros.h"
#include"usermessage.h"

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
    void sendusermessage(const usermessage u);
    void joinchannel(const QString&);
    void partchannel(const QString&);       
    void sendhostinfotoserverandhost(const QString &name,const QString &pwd, const QString &chan,int flag);
    void refreshlist();
    void refreshwho();
    bool buddylistcontains(QString user);
    bool ignorelistcontains(QString user);
    QString getmyhostip();
    QString nick;
    usermodel users;
    hostmodel hosts;
    QMap<QString,QString> schememap;
    QStringList channellist;

    void settingswindowemitfunktion();

    void sendquit(QString s);
    int ircstate();

    state connectstate;

    DECLARE_SINGLETON(netcoupler);
signals:
    void signewwholist(QList<userstruct>);
    void siggotusermessage(const usermessage u);
    void siggotchanellist(QStringList);

    void sigsettingswindowchanged();
    void siggotidletime(const QString&, int);
    void signosuchnick(const QString&);
    void sigconnected();
    void sigdisconnected();  

    void sighoststarts(hoststruct);   

private:
    QPointer<ircnet> irc;
    QPointer<snoppanet> http;
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
    void sendinfotochan(const QString &,const QString &);
    void refreshhostlist();
private slots:    
    void initSoundAndStartWho();
    void getchannellist(QStringList);
    void getircip(QString);
    void gethostlist(QList<hoststruct>,QString);  
    void getusermessage(const usermessage u);
    void getwholist();    
    void getscheme(QString,QString);    

    void readprocess();    
    void ircconnected();
    void ircdisconnected();

    void getmywormnethost(hoststruct);

    friend class userstruct;

    void loopTimerTimeout();
};
#endif // NETCOUPLER_H
