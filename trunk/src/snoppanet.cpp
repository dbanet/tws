#include "snoppanet.h"
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QTimer>
#include<QDebug>
#include<QFile>
#include<QTextStream>
#include<QApplication>
#include<QDir>
#include<QSignalMapper>
#include<QProcess>
#include"snpsettings.h"
#include"settingswindow.h"
#include"hoststruct.h"
#include"inihandlerclass.h"
#include"global_functions.h"
extern inihandlerclass inihandler;
snoppanet::snoppanet(QObject *parent) :
	QObject(parent) {
	manager = new QNetworkAccessManager(this);
	schememanager = new QNetworkAccessManager(this);
	hostmanager = new QNetworkAccessManager(this);
	hosttimer = new QTimer(this);
	signalmapper = new QSignalMapper(this);
	url = inihandler.stringlistfromini("[http wormnet server]").first();
	gameliststarts = 0;
        int delay = singleton<settingswindow>().from_map("sbhostrepead").value<int> ();
	hosttimer->start(delay);
}

snoppanet::~snoppanet() {
	manager->deleteLater();
	hosttimer->deleteLater();
	signalmapper->deleteLater();
}
void snoppanet::start() {
	request = inihandler.requestfromini("[http login header]");
	request.setUrl(url + "/wormageddonweb/Login.asp?UserName="
                        + singleton<snpsettings>().map["nickname"].value<QString> () + "&Password="
                        + singleton<snpsettings>().map["password"].value<QString> () + "&IPAddress=");
	reply = manager->get(request);
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
	this, SLOT(httpError(QNetworkReply::NetworkError)));
	connect(reply, SIGNAL(readyRead()),this, SLOT(readircip()));
}
void snoppanet::readircip() {
	temp.append(reply->readAll());
	qDebug() << temp;
	if (temp.contains(">") && temp.contains("<CONNECT ")) {
		temp = temp.remove("<CONNECT ").remove(">\n");
		ircip = temp;emit
		sigircip(ircip);
		temp.clear();
		reply->disconnect();
		reply->deleteLater();
	}
}
void snoppanet::httpError(QNetworkReply::NetworkError error) {
	qDebug()
			<< "an error occured, error code (value in http://doc.trolltech.com/4.4/qnetworkreply.html#NetworkError-enum) is: "
			<< error;
	qDebug() << "please make sure your have a working internet connection.";
}
void snoppanet::setchannellist(const QStringList &sl) {
	currentchannellist = sl;
	requestlist.clear();
	foreach(QNetworkReply *n,replylist) {
			Q_CHECK_PTR(n);
			n->deleteLater();
		}
	replylist.clear();
	foreach(QString s,currentchannellist) {
			requestlist.push_back(request);
			requestlist.last() = inihandler.requestfromini(
					"[http get host header]");
			channelmap[requestlist.size() - 1] = s;
			requestlist.last().setUrl(url
					+ "/wormageddonweb/GameList.asp?Channel=" + s.remove("#"));
			replylist.push_back(manager->get(requestlist.last()));
			connect(replylist.last(), SIGNAL(readyRead()),signalmapper, SLOT(map()));
			signalmapper->setMapping(replylist.last(), replylist.size() - 1);
			connect(replylist.last(), SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(httpError(QNetworkReply::NetworkError)));
		}
	connect(signalmapper, SIGNAL(mapped(int)),this, SLOT(readgamelist(int)));
	templist.clear();
	for (int i = 0; i < currentchannellist.size(); i++) {
		templist << "";
	}
	hosttimer->disconnect();
	connect(hosttimer, SIGNAL(timeout()),this, SLOT(hosttimeout()));
}
void snoppanet::hosttimeout() {
	foreach(QNetworkReply *n,replylist) {
			Q_CHECK_PTR(n);
			n->deleteLater();
		}
	replylist.clear();
	int i = 0;
	foreach(QString s,currentchannellist) {
			replylist.push_back(manager->get(requestlist[i]));
			i++;
			connect(replylist.last(), SIGNAL(readyRead()),signalmapper, SLOT(map()));
			signalmapper->setMapping(replylist.last(), replylist.size() - 1);
			connect(replylist.last(), SIGNAL(error(QNetworkReply::NetworkError)),
			this, SLOT(httpError(QNetworkReply::NetworkError)));
		}
	connect(signalmapper, SIGNAL(mapped(int)),this, SLOT(readgamelist(int)));
}
void snoppanet::readgamelist(int i) {
	templist[i].append(replylist[i]->readAll());
	if (templist[i].contains("<GAMELISTEND>") && templist[i].contains(
			"<GAMELISTSTART>")) {
		QStringList sl = templist[i].split("\n", QString::SkipEmptyParts);
		templist[i].clear();
		Q_ASSERT(sl.size()>=2);
		sl.takeFirst();
		sl.takeLast();
		sl.replaceInStrings("<GAME ", "");
		sl.replaceInStrings("><BR>", "");
		QList<hoststruct> list;
		QStringList sltemp;
		foreach(QString s,sl) {
				if (s[0].toAscii() == -33)
					s = s.remove(0, 1);
				list.push_back(hoststruct());
				sltemp = s.split(" ");
				list.last().sethost(sltemp);
			}
		emit sighostlist(list, channelmap[i]);
	} else if (templist[i].size() > 30000)
		templist[i].clear();
	else if (!templist[i].contains("<GAMELISTSTART>"))
		templist[i].clear();
}
void snoppanet::getscheme(QString s) {
	if (scheme.isEmpty()) {
		schemechannel = s;
		schemerequest = inihandler.requestfromini("[http get host header]");
		schemerequest.setUrl(url
				+ "/wormageddonweb/RequestChannelScheme.asp?Channel="
				+ s.remove("#"));
		schemereply = schememanager->get(schemerequest);
		connect(schemereply, SIGNAL(readyRead()),this, SLOT(getscheme()));
	}
}
void snoppanet::getscheme() {
	scheme.append(schemereply->readAll());
	if (scheme.startsWith("<") && scheme.endsWith("\n")) {
		scheme.remove("<SCHEME=");
		scheme.remove(">\n");emit
		sigchannelscheme(schemechannel, scheme);
		scheme.clear();
		schemereply->disconnect();
		schemereply = 0;
	}
}
void snoppanet::sendhost(const QString &gamename, const QString &ip,
		const QString &nick, const QString &pwd, const QString &chan,
		const QString &flag) {
	hostrequest = inihandler.requestfromini("[http get host header]");
	QString s = url + "/wormageddonweb/Game.asp?Cmd=Create&Name=" + gamename
			+ "&HostIP=" + ip + "&Nick=" + nick + "&Pwd=" + pwd + "&Chan="
			+ QString(chan).remove("#") + "&Loc=" + flag + "&Type=0&Pass=0";
	QString port=gethostport();
		if(port!="")
			s = url + "/wormageddonweb/Game.asp?Cmd=Create&Name=" + gamename
			+ "&HostIP=" + ip+":"+port + "&Nick=" + nick + "&Pwd=" + pwd + "&Chan="
			+ QString(chan).remove("#") + "&Loc=" + flag + "&Type=0&Pass=0";
	hostrequest.setUrl(s);
	hostreply = hostmanager->get(hostrequest);
	connect(hostreply, SIGNAL(readyRead()),this, SLOT(readhostreply()));
}
void snoppanet::closehost(QStringList sl) {
	if (!sl.isEmpty()) {
		hostrequest = inihandler.requestfromini("[http get host header]");
		QString s = url + "/wormageddonweb/Game.asp?Cmd=Close&GameID=" + sl[0]
				+ "&Name=" + sl[1] + "&HostID=&GuestID=&GameType=0";
		hostrequest.setUrl(s);
		hostreply = hostmanager->get(hostrequest);
	}
}
void snoppanet::refreshhostlist() {
	if (hostreply != 0) {
		this->hosttimeout();
	}
}
void snoppanet::readhostreply() {
	if (hostreply->readAll() == "<NOTHING>\n") {
		qDebug() << "wormnet wont open this game, try again later.";
		hostreply->disconnect();
		hostreply->deleteLater();
		emit sighostwontstart();
	}
}

