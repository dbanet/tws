#ifndef LEAGUESERVERHANDLER_H
#define LEAGUESERVERHANDLER_H

#include <QObject>
#include<QStringList>
#include<QHash>
#include<QTimer>
#include<QNetworkAccessManager>
#include"global_macros.h"

class QNetworkReply;
class QTimer;
class leagueserverhandler : public QObject
{
    Q_OBJECT
public:
    enum {
        e_truenick=0, e_rank, e_flag, e_webpage, e_clan, e_ranktooltip
    };    
    void login(const QString n,const QString p);
    void logout();
    void startrefresh();
    void stoprefresh();
    void profile(QString n);
    void setleague(const QString league, QString server);
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
    void logintTimeOut();    

    void refreshFinished();    
    void refresh();    
    void logoutFinished();      
    void profileFinished();

private:
    void myconnect(QString p,QString n);

    QNetworkAccessManager qnam;
    QNetworkReply *loginreply;
    QNetworkReply *refreshreply;
    QNetworkReply *logoutreply;
    QNetworkReply *profilereply;    

    QString servicename;

    QTimer connecttimer;
    QTimer informationrefreshtimer;

    QHash<QString,QStringList> map;

    QString serveraddress;
    bool loggingstate;
};

#endif // LEAGUESERVERHANDLER_H
