#ifndef LEAGUESERVERHANDLER_H
#define LEAGUESERVERHANDLER_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QStringList>
#include<QMap>
#include<QNetworkReply>
#include"global_macros.h"
class QTimer;
class leagueserverhandler : public QObject
{
    Q_OBJECT
public:
    enum {
        e_rank=0,e_flag,e_webpage,e_clan
    };    
    void login(const QString n,const QString p);
    void logout();
    void setleague(const QString league, const QString server);
    int map_at_toInt(const QString key,const int i);
    QString map_at_toString(const QString key,const int i);
    bool contains_key(QString key);
    QString nick;
    void reset();
    QString service_name();

    DECLARE_SINGLETON(leagueserverhandler);
signals:
    void sigloginsuccess();
    void sigloginfailed();
private slots:
    void loginFinished();
    void loginReadyRead();
    void logintTimeOut();

    void refreshFinished();
    void refreshReadyRead();
    void refresh();
    void refresherror(QNetworkReply::NetworkError);
    void loginerror(QNetworkReply::NetworkError);

    void logoutFinished();
    void logoutReadyRead();
    void logouterror(QNetworkReply::NetworkError);
private:
    void myconnect(QString p,QString n);
    QNetworkAccessManager qnam;
    QNetworkReply *loginreply;
    QNetworkReply *refreshreply;
    QNetworkReply *logoutreply;
    QString connectresponse;
    QString refreshresponse;

    QString servicename;

    QTimer *connecttimer;
    QTimer *informationrefreshtimer;

    QMap<QString,QStringList> map;

    QString serveraddress;
};

#endif // LEAGUESERVERHANDLER_H
