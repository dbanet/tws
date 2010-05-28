#ifndef SNOPPANET_H
#define SNOPPANET_H

#include <QObject>
#include<QNetworkReply>
#include<QStringList>
class QNetworkReply;
class QNetworkAccessManager;
class QTimer;
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
    void closehost(QStringList);

public slots:
    void refreshhostlist();

signals: //public signals:
	void sighostlist(QList<hoststruct>,QString);
    void sigircip(QString);
    void sigchannelscheme(QString,QString);
    void sighostwontstart();

private:
	QString temp;
	QStringList templist;
	QStringList currentchannellist;
	QString url;
	QString ircip;
	QNetworkAccessManager *manager;
	QNetworkReply *reply;
	QTimer *hosttimer;
	QMap<int,QString> channelmap;

	QNetworkAccessManager *schememanager;
	QNetworkRequest schemerequest;
	QNetworkReply *schemereply;

	QNetworkAccessManager *hostmanager;
	QNetworkRequest hostrequest;
	QNetworkReply *hostreply;

	QString schemechannel;
	QString scheme;

	QSignalMapper *signalmapper;
	QNetworkRequest request;
	QList<QNetworkReply*> replylist;
	QList<QNetworkRequest> requestlist;
	bool gameliststarts;
private slots:
	void readircip();
	void httpError(QNetworkReply::NetworkError);
	void hosttimeout();
	void readgamelist(int);
	void getscheme();
	void readhostreply();
};

#endif // SNOPPANET_H
