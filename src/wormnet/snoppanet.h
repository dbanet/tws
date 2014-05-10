#ifndef SNOPPANET_H
#define SNOPPANET_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
class QSignalMapper;
#include "hoststruct.h"
class snoppanet : public QObject
{
    Q_OBJECT

public:
    snoppanet(QObject *parent = 0);
    ~snoppanet();

    void start();
    void setChannelList(const QStringList&);
    void getscheme(QString);
    void sendhost(hoststruct h);
    hoststruct lasthost;

public slots:
    void refreshHostList();

signals: //public signals:
    void sighostlist(QList<hoststruct>,QString);
    void sigircip(QString);
    void sigchannelscheme(QString,QString);
    void sighostwontstart();    
    void sighoststarts(hoststruct);
    void sigstarthost(QString);
    void sigloginfailed();
private:
//    void inithosting(QString url);
    void closehostandstartlasthost(hoststruct h);
    void closeLastHost();

    hoststruct findduplicatedhosts(QList<hoststruct> list);
    hoststruct findmyhost(QList<hoststruct> list);
    QString temp;
    QStringList templist;
    QStringList currentChannelList;
    QString htmlAddress;
    QString ircip;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QTimer hostTimer;
    QTimer loginTimer;
    QMap<int,QString> channelMap;

    QNetworkReply *schemereply;

    QNetworkReply *hostreply;
    QNetworkReply *closehostreply;

//    QNetworkReply *hostlistforhostingreply;


    QString schemechannel;
    QString scheme;

    QSignalMapper *signalMapper;
    QNetworkRequest request;
    QList<QNetworkReply*> replyList;
    QList<QNetworkRequest> requestList;
    QList<hoststruct> hostlist;
    bool gameListStarts;    

    QString lastgameid;
private slots:
    void sendHostRequest();
    void readircip();
    void httpError(QNetworkReply::NetworkError);
    void hosttimertimeout();
    void readgamelist(int);
    void getscheme();
    void hostreplyfinished();
    void closeHostReplyFinished();

    void logintimertimeout();

//    void hostlistforhostingreplyfinished();
//    void repeathostlistforhostingreplyrequest();
};

#endif // SNOPPANET_H
