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
    void sendhost(hoststruct h);
    void closehostandstartlasthost(hoststruct h);
    void closelasthost();
    hoststruct lasthost;

public slots:
    void refreshhostlist();

signals: //public signals:
    void sighostlist(QList<hoststruct>,QString);
    void sigircip(QString);
    void sigchannelscheme(QString,QString);
    void sighostwontstart();    
    void sighoststarts(hoststruct);
    void sigloginfailed();
private:
    void inithosting(QString url);    
    hoststruct findduplicatedhosts(QList<hoststruct> list);
    QString temp;
    QStringList templist;
    QStringList currentchannellist;
    QString htmladdress;
    QString ircip;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer hosttimer;
    QTimer logintimer;
    QMap<int,QString> channelmap;

    QNetworkReply *schemereply;

    QNetworkReply *hostreply;
    QNetworkReply *closehostreply;

    QNetworkReply *hostlistforhostingreply;


    QString schemechannel;
    QString scheme;

    QSignalMapper *signalmapper;
    QNetworkRequest request;
    QList<QNetworkReply*> replylist;
    QList<QNetworkRequest> requestlist;
    QList<hoststruct> hostlist;
    bool gameliststarts;    
private slots:
    void sendhostrequest();
    void readircip();
    void httpError(QNetworkReply::NetworkError);
    void hosttimertimeout();
    void readgamelist(int);
    void getscheme();
    void readhostreply();
    void closehostreplyfinished();

    void logintimertimeout();

    void hostlistforhostingreplyfinished();
    void repeathostlistforhostingreplyrequest();
};

#endif // SNOPPANET_H
