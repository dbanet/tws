#ifndef SNOPPANET_H
#define SNOPPANET_H

#include<QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QStringList>
#include<QTimer>
class QSignalMapper;
#include"hoststruct.h"
class snoppanet : public QObject
{
    Q_OBJECT

public:
    snoppanet(QObject *parent = 0);
    ~snoppanet();

    void start();
    void setchannellist(const QStringList&);
    void getscheme(QString);
    void sendhost(const QString &gamename,const QString &ip,const QString &nick,const QString &pwd,const QString &chan,const QString &flag);
    void closehost(hoststruct h);
    void closelasthost();

public slots:
    void refreshhostlist();

signals: //public signals:
    void sighostlist(QList<hoststruct>,QString);
    void sigircip(QString);
    void sigchannelscheme(QString,QString);
    void sighostwontstart();    
    void sighoststarts(hoststruct);

private:
    void inithosting(QString url);
    hoststruct findlasthost(QList<hoststruct> list);
    QString temp;
    QStringList templist;
    QStringList currentchannellist;
    QString htmladdress;
    QString ircip;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer hosttimer;
    QMap<int,QString> channelmap;

    QNetworkReply *schemereply;

    QNetworkReply *hostreply;

    QNetworkReply *hostlistforhostingreply;


    QString schemechannel;
    QString scheme;

    QSignalMapper *signalmapper;
    QNetworkRequest request;
    QList<QNetworkReply*> replylist;
    QList<QNetworkRequest> requestlist;
    QList<hoststruct> hostlist;
    bool gameliststarts;
    hoststruct lasthost;
private slots:
    void readircip();
    void httpError(QNetworkReply::NetworkError);
    void hosttimeout();
    void readgamelist(int);
    void getscheme();
    void readhostreply();

    void hostlistforhostingreplyfinished();
    void repeathostlistforhostingreplyrequest();
};

#endif // SNOPPANET_H
