#include "ircnet.h"
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QTime>
#include<QMessageBox>
#include<QTextCodec>
#include"inihandlerclass.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include"about.h"
#include"codecselectdia.h"
#include"global_functions.h"
#include"myDebug.h"
#include"picturehandler.h"
#include"leagueserverhandler.h"
#include"netcoupler.h"
extern inihandlerclass inihandler;
extern QMap<QString, QStringList> usergarbagemap;
ircnet::ircnet(QString s, QObject *parent) :
	QObject(parent), tcp(new QTcpSocket(this)) {    
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
    s.append(S_S.map["clan"].value<QString> () + " ");    
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    QString flag=S_S.map["countrycode"].toString().toUpper();
    s.append(singleton<picturehandler>().map_countrycode_to_number(flag)+ " ");
    int i=S_S.map["rank"].value<QString> ().toInt();
    s.append(QString::number(i) + " ");
    s.append(flag+" ");
    s.append(S_S.map["information"].value<QString> ());
    tcp_write(s);
    tcp_write("list");
    emit sigconnected();
    who();
}
void ircnet::tcpread() {
    ircreadstring.append(CodecSelectDia::codec->toUnicode(tcp->readAll()));
    QStringList sl = ircreadstring.split("\n");
    ircreadstring = sl.takeLast(); //is "" or incomplete
    foreach(QString s,sl) {
        s=s.trimmed();
        if(s.isEmpty())
            continue;
        if(!firstMessageArrived){
            servermessageindicator=s.left(s.indexOf(" "));
            firstMessageArrived=true;
        }
        if (s.startsWith(servermessageindicator))
            readservermassege(s.remove(servermessageindicator).trimmed());
        else if (s.startsWith("PING"))
            tcp_write("PONG");
        else if (s.startsWith("ERROR"))
            myDebug() << s;
        else
            readusermessage(s);        
    }
}
void ircnet::readusermessage(QString &s) {
    QStringList sl = s.split(" ");
    Q_ASSERT(sl.size()>=3);
    QString user = sl.takeFirst().split("!").first().remove(":");
    QString garbage = sl.join(" ").remove("\r").remove("\n");
    QString command = sl.takeFirst();
    QString receiver = sl.takeFirst().remove("\r").remove(":");    
    if(command=="QUIT"){
        foreach(QString s,joinlist.keys()) {
            joinlist[s].removeAll(user);
        }
        gotusergarbage(user, garbage);
        emit sigusergarbagequit(user, garbage);
    } else if(command=="PRIVMSG"){
        emit sigmsg(user, receiver, sl.join(" ").remove(0, 1));
    } else if(command=="PART"){
        if (user.toLower() == nick.toLower())
            joinlist[receiver].clear();
        joinlist[receiver].removeAll(user);
        gotusergarbage(user, garbage);
        emit sigusergarbagepart(user, garbage);
    } else if(command=="JOIN"){
        joinlist[receiver] << user;
        gotusergarbage(user, garbage);
        emit sigusergarbagejoin(user, garbage);
    } else if(command=="NOTICE"){
        emit signotice(user, receiver, sl.join(" ").remove(0, 1));
    }else
        myDebug() << tr("Servermessage: ") << s;
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
    myDebug() << tr("disconnected from irc server.");
    singleton<leagueserverhandler>().logout();
    emit sigdisconnected();
}
void ircnet::readservermassege(QString s) {
    static bool b=false;
    QStringList sl = s.split(" ");
    if(sl.size()<3){
        sl.clear();
        return;
    }
    int command = sl.first().toInt(&b);
    if(!b)
        myDebug()<<sl<<"|"+servermessageindicator+"|";
    sl.takeFirst();
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
            if(S_S.map["enablesecurelogging"].toBool())
                QMessageBox::information(0,tr("Nickname collision!"),
                                         tr("Your nickname is already in use. You can wait some minutes or click on the profile button in secure logging section to change your nickname and try again."),
                                         QMessageBox::Ok);
            else
                QMessageBox::information(0,tr("Nickname collision!"),
                                         tr("Your nickname is already in use. You can wait some minutes or change your nickname and try again."),
                                         QMessageBox::Ok);
            tcp->disconnect();
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
    tcp_write("QUIT : [The Wheat Snooper] "+ S_S.map["information"].toString());
}
void ircnet::tcp_write(const QString &msg){
    tcp->write(CodecSelectDia::codec->fromUnicode(msg)+"\n");
}
int ircnet::state(){
    return tcp->state();
}

