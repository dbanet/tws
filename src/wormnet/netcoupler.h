#ifndef NETCOUPLER_H
#define NETCOUPLER_H

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QHostInfo>
#include <QPointer>
#include <QProcess>

#include "hoststruct.h"
#include "userstruct.h"
#include "usermodel.h"
#include "hostmodel.h"
#include "global_macros.h"
#include "usermessage.h"
#include "ircnet.h"
#include "snoppanet.h"

class netcoupler : public QObject
{
    Q_OBJECT

public:    
    enum state{
        e_started, e_stoped
    };
    QPointer<ircnet> irc;
    void start(QString nick);
    void stop();                
    void sendUserMessage(const usermessage u);
    void joinChannel(const QString&);
    void partChannel(const QString&);
    void sendHostInfoToServerAndHost(const QString &name,const QString &pwd, const QString &chan,int flag);
    void refreshList();
    void refreshWho();
    bool buddyListContains(QString user);
    bool ignoreListContains(QString user);
    QString getMyHostIP();
    QString nick;
    usermodel users;
    hostmodel hosts;
    QMap<QString,QString> schemeMap;
    QStringList channelList;

    void settingsWindowEmitFunction();

    void sendQuit(QString s);
    int ircState();

    state connectState;

    DECLARE_SINGLETON(netcoupler);
signals:
    void sigGotUserMessage(const usermessage u);
    void sigGotChannelList(QStringList);
    void sigGotChanList(QStringList);
    void sigJoinedChannel(QString,int);
    void sigUpdatedAmountOfUsers(QString,int);
    void sigSettingsWindowChanged();
    void sigGotIdleTime(const QString&,int,int);
    void sigNoSuchNick(const QString&);
    void sigConnected();
    void sigDisconnected();

    void sigHostStarts(hoststruct);

private:
    QPointer<snoppanet> http;
    QString ircIP;
    QStringList listOfJoinedChannels;

    QTimer hostLifeTimer;
    QTimer getOwnHostTimer;
    int hostTimeOutDelay;

    QProcess *p;

    QTimer timer;
    QTimer loopTimer;           //to safe usergarbage every 30sec    

    QString getProcessString();
    void startProcess(const QString&, const QStringList&);
//  void createHost(hoststruct);
    void createHost(QString);
    QString myIP;
public slots:
    void joinGameLink(const QString &);
    void joinGame(const QString &,const QString&,const QString&);
    void sendInfoToChan(const QString &,const QString &);
    void refreshHostList();
private slots:    
    void initSoundAndStartWho();
    void emitSigGotChanList(QStringList);
    void emitSigJoinedChannel(QString,int);
    void emitSigUpdatedAmountOfUsers(QString,int);
    void getIRCIP(QString);
    void getHostList(QList<hoststruct>,QString);
    void getUserMessage(const usermessage u);
    void getScheme(QString,QString);
    void readProcess();
    void ircConnected();
    void ircDisconnected();

//    void getmywormnethost(hoststruct);
    void getMyWormnetHost(QString id);

    friend class userstruct;

    void loopTimerTimeout();
};
#endif // NETCOUPLER_H
