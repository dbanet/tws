#include"irc_ircnet.h"
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QTime>
#include<QMessageBox>
#include"src/wormnet/inihandlerclass.h"
#include"src/wormnet/snpsettings.h"
#include"src/wormnet/settingswindow.h"
#include"src/wormnet/about.h"
extern inihandlerclass inihandler;
extern QMap<QString, QStringList> usergarbagemap;
irc_ircnet::irc_ircnet(QString s, QObject *parent, QString channel) :
        QObject(parent), tcp(new QTcpSocket(this)) {
    mapusercommand["QUIT"] = 1;
    mapusercommand["PRIVMSG"] = 2;
    mapusercommand["PART"] = 3;
    mapusercommand["JOIN"] = 4;
    mapusercommand["NOTICE"] = 5;
    QStringList sl;
    nick = s;
    connect(tcp, SIGNAL(readyRead()),this, SLOT(tcpread()));
    servermessageindicator=":";
    ircChannel=channel;
}
void irc_ircnet::setip(const QString &ip) {
    wnip = ip;
}
void irc_ircnet::start() {
    connect(tcp, SIGNAL(connected()),this, SLOT(connected()));
    connect(tcp, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(tcpError(QAbstractSocket::SocketError)));
    tcp->connectToHost(wnip, 6667, QIODevice::ReadWrite);
}
void irc_ircnet::tcpError(QAbstractSocket::SocketError s){
    if(s==QAbstractSocket::RemoteHostClosedError)
        return;
}
void irc_ircnet::connected() {
    tcp->write(qPrintable("NICK " + nick + "\n"));
    QString s = inihandler.stringlistfromini("[irc register]").first();
    QStringList sl;
    sl = s.split(" ", QString::SkipEmptyParts);
    s = sl.takeFirst() + "  ";
    s.append(singleton<snpsettings>().map["clan"].value<QString> () + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    s.append(singleton<snpsettings>().map["flag"].value<QString> () + " ");
    s.append(singleton<snpsettings>().map["rank"].value<QString> () + " ");    
    s.append("?? ");
    s.append(singleton<snpsettings>().map["client"].value<QString> ());
    tcp->write(qPrintable(s+"\n"));
    emit sigconnected();
}
void irc_ircnet::tcpread() {
    ircreadstring.append(tcp->readAll());
    QStringList sl = ircreadstring.split("\n");
    ircreadstring = sl.takeLast(); //is "" or incomplete
    foreach(QString s,sl) {
        if (s.startsWith(servermessageindicator)) {
            readservermassege(s.remove(servermessageindicator).trimmed());
        }
        else if (s.startsWith("PING")) {
            QString auth=s.split(":").at(1);
            tcp->write(qPrintable("PONG "+auth+"\n"));
        } else if (s.startsWith("ERROR")) {
            ;
        } else
            readusermessage(s);        
    }
}
void irc_ircnet::readusermessage(QString &s) {
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
        {
            foreach(QString s,joinlist.keys()) {
                joinlist[s].removeAll(userstruct::whoami(user));
            }            
            break;
        }
    case 2:
        emit sigmsg(user, receiver, sl.join(" ").remove(0, 1));
        break;
    case 3:
        if (user.toLower() == nick.toLower())
            joinlist[receiver].clear();
        joinlist[receiver].removeAll(userstruct::whoami(user));        
        break;
    case 4:
        if(nick==user)
            break;
        joinlist[receiver] << userstruct::whoami(user);
        gotusergarbage(user, garbage);
        emit sigusergarbagejoin(user, garbage);
        break;
    case 5:
        emit signotice(user, receiver, sl.join(" ").remove(0, 1));
        break;
    }
}
void irc_ircnet::gotusergarbage(QString &user, QString &s) {
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
void irc_ircnet::disconnected() {
    reconnecttimer.singleShot(200 *1000, this, SIGNAL(sigreconnect()));
}
void irc_ircnet::readservermassege(QString s) {
    static bool b=false;
    static bool firstMessage=false;
    if(!firstMessage){
        servermessageindicator=":"+s.split(" ").first();
        firstMessage=true;
        emit sigFirstServerMessage();
        joinchannel(ircChannel);
        listofjoinedchannels<<ircChannel;
    }
    QStringList sl = s.split(" ");
    Q_ASSERT(sl.size()>=3);
    int command = sl.takeFirst().toInt(&b);
    if(!b)
        return;
    sl.takeFirst();
    switch (command) {
    case 352: //user added
        //userList.push_back(userstruct(sl));
        break;
    case 353: //lists the user in a channel
        {
            sl.takeFirst();
            QString channel = sl.takeFirst();
            sl[0].remove(":");
            foreach(QString s,sl)
                joinlist[channel] << userstruct::whoami(s);            
            break;
        }
    case 366: //end of /NAME list happens after join
        break;
    case 301: //Auto Away at Sun Nov 23 20:25:36 2008
        emit sigmsg(sl.takeFirst(), nick, sl.join(" "));
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
        case 323: //end of list command
        case 322: //channel added
        case 315: //end of who command
            break;
        }
}
void irc_ircnet::joinchannel(const QString &chan) {
    tcp->write(qPrintable("JOIN " + chan + "\n"));
}
void irc_ircnet::partchannel(const QString &chan) {
    tcp->write(qPrintable("PART " + chan + "\n"));
}
void irc_ircnet::sendmessage(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp->write(qPrintable("PRIVMSG " + receiver + " :" + s + "\n"));
}
void irc_ircnet::sendnotice(const QString &msg, const QString &receiver) {
    QString s = msg;
    s.replace("\n", " ");
    tcp->write(qPrintable("NOTICE " + receiver + " :" + s + "\n"));
}
void irc_ircnet::sendrawcommand(const QString &raw) {
    tcp->write(qPrintable(raw + "\n"));
}
void irc_ircnet::quit() {
    tcp->write(qPrintable("QUIT : The Wheat Snooper " + about::version+" \n"));
    tcp->waitForDisconnected(5000);
    emit sigdisconnected();
}
irc_ircnet::~irc_ircnet() {
}

