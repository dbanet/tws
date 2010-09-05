#include "ircnet.h"
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QTime>
#include<QMessageBox>
#include<QDebug>
#include"inihandlerclass.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include "about.h"
extern inihandlerclass inihandler;
extern QMap<QString, QStringList> usergarbagemap;
int i1 = 0;
int i2 = 0;
ircnet::ircnet(QString s, QObject *parent) :
	QObject(parent), tcp(new QTcpSocket(this)) {
    mapusercommand["QUIT"] = 1;
    mapusercommand["PRIVMSG"] = 2;
    mapusercommand["PART"] = 3;
    mapusercommand["JOIN"] = 4;
    mapusercommand["NOTICE"] = 5;
    QStringList sl;
    nick = s;
    connect(tcp, SIGNAL(readyRead()),this, SLOT(tcpread()));
    whoreceivedcompletely = 1;
}
void ircnet::setip(const QString &ip) {
    wnip = ip;
}
bool firstMessageArrived=false;
void ircnet::start() {
    firstMessageArrived=false;
    connect(tcp, SIGNAL(connected()),this, SLOT(connected()));
    connect(tcp, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(tcpError(QAbstractSocket::SocketError)));
    tcp->connectToHost(wnip, 6667, QIODevice::ReadWrite);
}
void ircnet::tcpError(QAbstractSocket::SocketError s){
    qDebug()<<tr("There was an error while connecting to Wormnet: ")<<s;
}
void ircnet::connected() {
    QStringList sl = inihandler.stringlistfromini("[irc password]");
    if (!sl.isEmpty()) {
        if (sl.first() == "PASS standard") {
            this->sendrawcommand("PASS ELSILRACLIHP ");
        } else {
            this->sendrawcommand(sl.first());
        }
    }
    tcp->write(QString(QString("NICK ") + nick + "\n").toAscii());
    QString s = inihandler.stringlistfromini("[irc register]").first();
    sl = s.split(" ", QString::SkipEmptyParts);
    s = sl.takeFirst() + " ";
    s.append(" " + singleton<snpsettings>().map["clan"].value<QString> () + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    s.append(singleton<snpsettings>().map["flag"].value<QString> () + " "
             + singleton<snpsettings>().map["rank"].value<QString> () + " ");
    if (singleton<snpsettings>().map["flag"].value<QString> ().toInt() > 49) {
        switch (singleton<snpsettings>().map["flag"].value<QString> ().toInt()) {
        case 53:
            s.append("CL ");
            break;
        case 54:
            s.append("CS ");
            break;
        case 55:
            s.append("SI ");
            break;
        case 56:
            s.append("LB ");
            break;
        case 57:
            s.append("MD ");
            break;
        case 58:
            s.append("UA ");
            break;
        case 59:
            s.append("LV ");
            break;
        case 60:
            s.append("SK ");
            break;
        case 61:
            s.append("CR ");
            break;
        case 62:
            s.append("EE ");
            break;
        case 63:
            s.append("CN ");
            break;
        case 64:
            s.append("CO ");
            break;
        case 65:
            s.append("EC ");
            break;
        case 66:
            s.append("UY ");
            break;
        case 67:
            s.append("VE ");
            break;
        }
    } else
        s.append(sl.takeFirst() + " ");
    s.append(singleton<snpsettings>().map["client"].value<QString> ());
    tcp->write(qPrintable(s));
    tcp->write("\n");
    tcp->write("list\n");
    emit sigconnected();
}
void ircnet::tcpread() {
    ircreadstring.append(tcp->readAll());
    QStringList sl = ircreadstring.split("\n");
    ircreadstring = sl.takeLast(); //is "" or incomplete
    foreach(QString s,sl) {
        if(!firstMessageArrived){
            servermessageindicator=s.left(s.indexOf(" "));
            firstMessageArrived=true;
        }
        if (s.startsWith(servermessageindicator))
            readservermassege(s.remove(servermessageindicator).trimmed());
        else if (s.startsWith("PING")) {
            tcp->write("PONG\n");
        } else if (s.startsWith("ERROR")) {
            qDebug() << s;
        } else {
            readusermessage(s);
        }
    }
}
void ircnet::readusermessage(QString &s) {
    QStringList sl = s.split(" ");
    Q_ASSERT(sl.size()>=3);
    QString user = sl.takeFirst().split("!").first().remove(":");
    QString garbage = sl.join(" ").remove("\r").remove("\n");
    QString command = sl.takeFirst();
    QString receiver = sl.takeFirst().remove("\r").remove(":");
    switch (mapusercommand[command]) {
        //mapusercommand["QUIT"]=1;
        //mapusercommand["PRIVMSG"]=2;
        //mapusercommand["PART"]=3;
	//mapusercommand["JOIN"]=4;
	//mapusercommand["NOTICE"]=5;
    case 1:
        foreach(QString s,joinlist.keys()) {
            joinlist[s].removeAll(user);
        }
        gotusergarbage(user, garbage);
        emit sigusergarbagequit(user, garbage);
        break;
	case 2:
        emit sigmsg(user, receiver, sl.join(" ").remove(0, 1));
        break;
	case 3:
        if (user.toLower() == nick.toLower())
            joinlist[receiver].clear();
        joinlist[receiver].removeAll(user);
        gotusergarbage(user, garbage);
        emit sigusergarbagepart(user, garbage);
        break;
	case 4:
        joinlist[receiver] << user;
        gotusergarbage(user, garbage);
        emit sigusergarbagejoin(user, garbage);
        break;
	case 5:
        emit signotice(user, receiver, sl.join(" ").remove(0, 1));
        break;
	default:
        qDebug() << "readusermessage: " << s;
    }
}
void ircnet::gotusergarbage(QString &user, QString &s) {
    if (usergarbagemap[user.toLower()].size()
        < singleton<settingswindow>().from_map("sbmaximumoftextblocksinlog").toInt())
        usergarbagemap[user.toLower()]
                << QDate::currentDate().toString("dd.MM") + " "
                + QTime::currentTime().toString("hh:mm") + " " + s;
    else {
        usergarbagemap[user.toLower()] = usergarbagemap[user.toLower()].mid(
                singleton<settingswindow>().from_map("sbmaximumoftextblocksinlog").toInt() * 2
                / 3);
        usergarbagemap[user.toLower()]
                << QDate::currentDate().toString("dd.MM") + " "
                + QTime::currentTime().toString("hh:mm") + " " + s;
    }
    emit sigusergarbage(user, usergarbagemap[user.toLower()].last());
}
void ircnet::disconnected() {
    wholist.clear();
    channellist.clear();
    reconnecttimer.singleShot(300 * 1000, this, SLOT(start()));
    tcp->disconnectFromHost();
    qDebug() << tr("disconnected from irc server.");
}
void ircnet::readservermassege(QString s) {
    static bool b=false;
    QStringList sl = s.split(" ");
    Q_ASSERT(sl.size()>=4);
    int command = sl.takeFirst().toInt(&b);
    Q_ASSERT(b);
    sl.takeFirst();
    QString channel;
    switch (command) {
    case 323: //end of list command
        channellist = tempchannellist;
        tempchannellist.clear();
        justgetlist = false;
        emit siggetlist(channellist);
        break;
    case 322: //channel added
        tempchannellist << sl.join(" ");
        break;
    case 315: //end of who command
        wholist = templist;
        templist.clear();
        whoreceivedcompletely = 1;
        break;
    case 352: //user added
        templist.push_back(userstruct(sl));
        break;
    case 353: //lists the user in a channel
        sl.takeFirst();
        channel = sl.takeFirst();
        sl[0].remove(":");
        joinlist[channel] << sl;
        if (joinlist[channel].count(nick) > 1) {
            joinlist[channel].removeOne(nick);
        }
        break;
	case 301: //Auto Away at Sun Nov 23 20:25:36 2008
            emit sigmsg(sl.takeFirst(), nick, sl.join(" "));
            break;
	case 366: //end of /NAME list happens after join
            break;
	case 321: //Users  Name
            break;
	case 311: //311 loOkias`twsnp ```MihaiS`sW` ~sW no.address.for.you * :40 0 ?? The Wheat Snooper
            break;
	case 319: //319 loOkias`twsnp ```MihaiS`sW` :#AnythingGoes
            break;
	case 312: //312 loOkias`twsnp ```MihaiS`sW` wormnet1.team17.com :Team17 Ltd.
            break;
	case 318: //318 loOkias`twsnp ```MihaiS`sW` :End of /WHOIS list.
            break;
	case 317: //317 loOkias`twsnp ```MihaiS`sW` 57 1231682995 :seconds idle, signon time
            if (sl.size() >= 2) {
                QString s = sl.takeFirst();
                int i = sl.first().toInt();
                emit siggotidletime(s, i);
            }
            break;
	case 401: //401 lookias`twsnp lololol :No such nick/channel
            if (sl.size() >= 2) {
                QString s = sl.takeFirst();
                emit signosuchnick(s);
            }
            break;
        case 433: //nickname allready in use
            QMessageBox::information(0,tr("Nickname collision!"),
                                     tr("The server things that your nickname is allready in use. This is usually caused by a uncomplete logout at the last session. If its like that you have to wait a few seconds or change your nickname! \n\nIf your nick got faked... bl :)"),
                                     QMessageBox::Ok);
            break;
	case 412: //No text to send
	case 462: //You may not reregister	
	case 421: //unknown command
	case 409: //No origin specified
	case 403: //No such channel
	default:
            qDebug() << s;
	}
}
void ircnet::joinchannel(const QString &chan) {
    tcp->write(QString(QString("JOIN ") + chan + "\n").toAscii());
}
void ircnet::partchannel(const QString &chan) {
    tcp->write(QString(QString("PART ") + chan + "\n").toAscii());
}
void ircnet::sendmessage(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp->write(QString("PRIVMSG " + receiver + " :" + s + "\n").toAscii());
}
void ircnet::sendnotice(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp->write(QString("NOTICE " + receiver + " :" + s + "\n").toAscii());
}
void ircnet::sendrawcommand(const QString &raw) {
    tcp->write(QString(raw + "\n").toAscii());
}
void ircnet::refreshlist() {
    if (justgetlist == false) {
        tcp->write("list\n");
        justgetlist = true;
    }
}
void ircnet::who() {
    if (whoreceivedcompletely) {
        tcp->write("who\n");
        whoreceivedcompletely = 0;
    }
}
void ircnet::quit() {
    tcp->write(QString("QUIT : The Wheat Snooper "
                       + about::version +
                       +" \n").toAscii());
    tcp->waitForDisconnected(5000);
    emit sigdisconnected();
}
ircnet::~ircnet() {
}
