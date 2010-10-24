#ifndef LEAGUESERVERHANDLER_H
#define LEAGUESERVERHANDLER_H

#include <QObject>
#include<QStringList>
#include<QMap>
#include<QTimer>
#include<QNetworkAccessManager>
#include"global_macros.h"
#include"mynetworkreply.h"
class QNetworkReply;
class QTimer;
class leagueserverhandler : public QObject
{
    Q_OBJECT
public:
    enum {
        e_truenick=0, e_rank, e_flag, e_webpage, e_clan
    };    
    void login(const QString n,const QString p);
    void logout();
    void startrefresh();
    void stoprefresh();
    void profile(QString n);
    void setleague(const QString league, const QString server);
    int map_at_toInt(const QString key,const int i);
    QString map_at_toString(const QString key,const int i);
    bool contains_key(QString key);
    QString nick;
    QString leagueloginnick;
    void reset();
    QString service_name();
    bool islogged();

    DECLARE_SINGLETON(leagueserverhandler);
signals:
    void sigloginsuccess();
    void sigloginfailed();
    void siglogout();
    void sigprofile(QString);
private slots:
    void loginFinished();
    void loginReadyRead();
    void logintTimeOut();    

    void refreshFinished();
    void refreshReadyRead();
    void refresh();    

    void logoutFinished();
    void logoutReadyRead();    

    void profileFinished();
    void profileReadyRead();    
private:
    void myconnect(QString p,QString n);
    QNetworkAccessManager qnam;
    QNetworkReply *loginreply;
    QNetworkReply *refreshreply;
    QNetworkReply *logoutreply;
    QNetworkReply *profilereply;
    QString connectresponse;
    QString refreshresponse;
    QString profileresponse;

    QString servicename;

    QTimer connecttimer;
    QTimer informationrefreshtimer;

    QMap<QString,QStringList> map;

    QString serveraddress;
    bool loggingstate;
};

#endif // LEAGUESERVERHANDLER_H
