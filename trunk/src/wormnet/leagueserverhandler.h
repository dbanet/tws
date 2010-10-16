#ifndef LEAGUESERVERHANDLER_H
#define LEAGUESERVERHANDLER_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QStringList>
#include<QMap>
#include<QNetworkReply>
class QTimer;
class leagueserverhandler : public QObject
{
    Q_OBJECT
public:
    explicit leagueserverhandler(QObject *parent);
    void login(const QString n,const QString p);

    void setleague(const QString league, const QString server);
    static int map_at_toInt(const QString key,const int i);
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
private:
    void myconnect(QString p,QString n);
    QNetworkAccessManager qnam;
    QNetworkReply *connectreply;
    QNetworkReply *refreshreply;
    QString connectresponse;
    QString refreshresponse;

    QString nick;
    QString password;
    QString servicename;

    QTimer *connecttimer;
    QTimer *informationrefreshtimer;

    static QMap<QString,QStringList> map;

    QString serveraddress;
};

#endif // LEAGUESERVERHANDLER_H
