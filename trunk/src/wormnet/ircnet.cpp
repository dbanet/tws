#include "ircnet.h"
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QTime>
#include<QMessageBox>
#include<QTextCodec>
#include"inihandlerclass.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include "about.h"
#include "codecselectdia.h"
#include"myDebug.h"
extern inihandlerclass inihandler;
extern QMap<QString, QStringList> usergarbagemap;
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
void ircnet::reconnect(){
    myDebug()<<tr("Reconnecting");
    tcp->connectToHost(wnip, 6667, QIODevice::ReadWrite);
}
void ircnet::tcpError(QAbstractSocket::SocketError s){
    if(s==QAbstractSocket::RemoteHostClosedError)
        return;
    myDebug()<<tr("There was an error with the connection to Wormnet.");
}
void ircnet::connected() {
    QStringList sl = inihandler.stringlistfromini("[irc password]");
    if (!sl.isEmpty()) {
        if (sl.first() == "PASS standard") {
            if(wnip == "itakagames.spb.ru")
                sendrawcommand("PASS ELSILRACLIHP ");
            else
                sendrawcommand("PASS ELSILRACLIHP");
        } else {
            this->sendrawcommand(sl.first());
        }
    }
    tcp_write("NICK " + nick);
    QString s = inihandler.stringlistfromini("[irc register]").first();
    sl = s.split(" ", QString::SkipEmptyParts);
    s = sl.takeFirst() + "  ";
    s.append(singleton<snpsettings>().map["clan"].value<QString> () + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    s.append(singleton<snpsettings>().map["flag"].value<QString> () + " ");
    s.append(singleton<snpsettings>().map["rank"].value<QString> () + " ");
    switch (singleton<snpsettings>().map["flag"].value<QString> ().toInt()) {
        //written by steps
    case 0:
                s.append("UK ");
        break;
    case 1:
        s.append("AR ");
        break;
    case 2:
        s.append("AU ");
        break;
    case 3:
        s.append("AT ");
        break;
    case 4:
        s.append("BE ");
        break;
    case 5:
        s.append("BR ");
        break;
    case 6:
        s.append("CA ");
        break;
    case 7:
        s.append("HR ");
        break;
    case 8:
        s.append("BA ");
        break;
    case 9:
        s.append("CY ");
        break;
    case 10:
        s.append("CZ ");
        break;
    case 11:
        s.append("DK ");
        break;
    case 12:
        s.append("FI ");
        break;
    case 13:
        s.append("FR ");
        break;
    case 14:
        s.append("GE ");
        break;
    case 15:
        s.append("DE ");
        break;
    case 16:
        s.append("GR ");
        break;
    case 17:
        s.append("HK ");
        break;
    case 18:
        s.append("HU ");
        break;
    case 19:
        s.append("IS ");
        break;
    case 20:
        s.append("IN ");
        break;
    case 21:
        s.append("MC ");
        break;
    case 22:
        s.append("IR ");
        break;
    case 23:
        s.append("IQ ");
        break;
    case 24:
        s.append("IE ");
        break;
    case 25:
        s.append("IL ");
        break;
    case 26:
        s.append("IT ");
        break;
    case 27:
        s.append("JP ");
        break;
    case 28:
        s.append("LI ");
        break;
    case 29:
        s.append("LU ");
        break;
    case 30:
        s.append("MY ");
        break;
    case 31:
        s.append("MT ");
        break;
    case 32:
        s.append("MX ");
        break;
    case 33:
        s.append("MA ");
        break;
    case 34:
        s.append("NL ");
        break;
    case 35:
        s.append("NZ ");
        break;
    case 36:
        s.append("NO ");
        break;
    case 37:
        s.append("PL ");
        break;
    case 38:
        s.append("PT ");
        break;
    case 39:
        s.append("CR ");
        break;
    case 40:
        s.append("RO ");
        break;
    case 41:
        s.append("RU ");
        break;
    case 42:
        s.append("SG ");
        break;
    case 43:
        s.append("ZA ");
        break;
    case 44:
        s.append("ES ");
        break;
    case 45:
        s.append("SW ");
        break;
    case 46:
        s.append("CH ");
        break;
    case 47:
        s.append("TR ");
        break;
    case 48:
        s.append("US ");
        break;
    case 49:
        s.append("?? ");
        break;
    case 50:
        s.append("?? ");
        break;
    case 51:
        s.append("?? ");
        break;
    case 52:
        s.append("?? ");
        break;
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
    case 68:
        s.append("LT ");
        break;
    case 69:
        s.append("BG ");
        break;
    case 70:
        s.append("EG ");
        break;
    case 71:
        s.append("SA ");
        break;
    case 72:
        s.append("KR ");
        break;
    case 73:
        s.append("BY ");
        break;
    case 74:
        s.append("PE ");
        break;
    case 75:
        s.append("DZ ");
        break;
    case 76:
        s.append("KZ ");
        break;
    case 77:
        s.append("SV ");
        break;
    case 78:
        s.append("TW ");
        break;
    case 79:
        s.append("JM ");
        break;
    case 80:
        s.append("GT ");
        break;
    case 81:
        s.append("MH ");
        break;
    case 82:
        s.append("MK ");
        break;
    case 83:
        s.append("AE ");
        break;
    default:
        s.append("?? ");
        break;
        //*******
    }
    s.append(singleton<snpsettings>().map["client"].value<QString> ());
    tcp_write(s);
    tcp_write("list");
    emit sigconnected();
}
void ircnet::tcpread() {
    ircreadstring.append(CodecSelectDia::codec->toUnicode(tcp->readAll()));
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
            tcp_write("PONG");
        } else if (s.startsWith("ERROR")) {
            myDebug() << s;
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
        myDebug() << tr("Servermessage: ") << s;
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
    reconnecttimer.singleShot(200 *1000, this, SIGNAL(sigreconnect()));
    myDebug() << tr("disconnected from irc server.");
}
void ircnet::readservermassege(QString s) {
    static bool b=false;
    QStringList sl = s.split(" ");
    Q_ASSERT_X(sl.size()>=3,"420",sl.join("\n").toAscii());
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
        case 404: //Cannot send to channel
        case 372: //:- info
	default:
            myDebug() << s;
	}
}
void ircnet::joinchannel(const QString &chan) {
    tcp_write("JOIN " + chan + "\n");
}
void ircnet::partchannel(const QString &chan) {
    tcp_write("PART " + chan);
}
void ircnet::sendmessage(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp_write("PRIVMSG " + receiver + " :" + s);
}
void ircnet::sendnotice(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp_write("NOTICE " + receiver + " :" + s);
}
void ircnet::sendrawcommand(const QString &raw) {
    tcp_write(raw);
}
void ircnet::refreshlist() {
    if (justgetlist == false) {
        tcp_write("list");
        justgetlist = true;
    }
}
void ircnet::who() {    
    if (whoreceivedcompletely) {
        tcp_write("who");
        whoreceivedcompletely = 0;
    }
}
void ircnet::quit() {
    tcp_write("QUIT : The Wheat Snooper "+ about::version);
    tcp->waitForDisconnected(5000);
    emit sigdisconnected();
}
void ircnet::tcp_write(const QString &msg){
    tcp->write(CodecSelectDia::codec->fromUnicode(msg)+"\n");
}

ircnet::~ircnet() {
}
