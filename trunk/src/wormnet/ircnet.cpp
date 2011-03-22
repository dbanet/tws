#include<QFile>
#include<QApplication>
#include<QDir>
#include<QTime>
#include<QMessageBox>
#include<QTextCodec>

#include"ircnet.h"
#include"inihandlerclass.h"
#include"settings.h"
#include"settingswindow.h"
#include"about.h"
#include"codecselectdia.h"
#include"global_functions.h"
#include"myDebug.h"
#include"picturehandler.h"
#include"leagueserverhandler.h"
extern inihandlerclass inihandler;

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
    emit sigdisconnected ();
}
void ircnet::connected() {
    QStringList sl = inihandler.stringlistfromini("[irc password]");
    if (!sl.isEmpty()) {
        if (sl.first() == "PASS standard") {
            if(wnip == "itakagames.spb.ru")
                tcp_write("PASS ELSILRACLIHP ");
            else
                tcp_write("PASS ELSILRACLIHP");
        } else
            tcp_write(sl.first());
    }
    tcp_write("NICK " + nick);
    QString s = inihandler.stringlistfromini("[irc register]").first();    
    sl = s.split(" ", QString::SkipEmptyParts);
    s = sl.takeFirst() + "  ";
    s.append(S_S.getstring("clan") + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    QString flag=S_S.getstring("countrycode").toUpper();
    s.append(singleton<picturehandler>().map_countrycode_to_number(flag)+ " ");   
    if(S_S.getbool("leaguestatecoloron") && S_S.getbool("enablesecurelogging"))
        s.append(S_S.getstring("leaguestatecolorname").remove('#')+" ");
    else {
        int i=S_S.getint("rank");
        s.append(QString::number(i) + " ");
    }
    s.append(flag+" ");
    s.append(S_S.getstring("information"));
    tcp_write(s);
    tcp_write("list");    
    tcp_write("who");
    emit sigconnected();    
}
void ircnet::tcpread() {    //arrives like this msg\msg\n...\n...\n
    ircreadstring.append(CodecSelectDia::codec->toUnicode(tcp->readAll()));
    QStringList sl = ircreadstring.split("\n");
    ircreadstring = sl.takeLast(); //is "" or incomplete
    foreach(QString s,sl) {
        s=s.simplified();
        if(s.isEmpty())
            continue;
        if(!firstMessageArrived){
            servermessageindicator=s.left(s.indexOf(" "));
            firstMessageArrived=true;
        }
        if (s.startsWith(servermessageindicator))
            readservermassege(s.remove(servermessageindicator).simplified());
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
        foreach(QString s,joinlist.keys())
            joinlist[s].removeAll(user);                
        usermessage u(garbage, usermessage_type(e_GARBAGE | e_GARBAGEQUIT), user, "");
        u.settime(time());
        appendhistory(u);
        emit siggotusermessage(u);
    } else if(command=="PRIVMSG"){
        QString s=sl.join(" ").remove(0, 1);
        emit siggotusermessage(usermessage(s,e_PRIVMSG, user, receiver));
    } else if(command=="PART"){
        if (user.toLower() == nick.toLower())
            joinlist[receiver].clear();
        joinlist[receiver].removeAll(user);
        usermessage u(garbage,usermessage_type(e_GARBAGE | e_GARBAGEPART), user, receiver);
        u.settime(time());
        appendhistory(u);
        emit siggotusermessage(u);
    } else if(command=="JOIN"){
        joinlist[receiver] << user;        
        usermessage u(garbage,usermessage_type(e_GARBAGE | e_GARBAGEJOIN), user, receiver);
        u.settime(time());
        appendhistory(u);
        emit siggotusermessage(u);
    } else if(command=="NOTICE"){
        QString s=sl.join(" ").remove(0, 1);
        emit siggotusermessage(usermessage(s, e_NOTICE, user, receiver));
    } else
        myDebug() << tr("Servermessage: ") << s;    
}
void ircnet::disconnected() {                   
    myDebug() << tr("disconnected from irc server.");
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
        if (joinlist[channel].count(nick) > 1)
            joinlist[channel].removeOne(nick);        
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
        if(S_S.getbool("enablesecurelogging"))
            QMessageBox::information(0,tr("Nickname collision!"),
                                     tr("Your nickname is already in use. You can wait some minutes or click on the profile button in secure logging section to change your nickname and try again."),
                                     QMessageBox::Ok);
        else
            QMessageBox::information(0,tr("Nickname collision!"),
                                     tr("Your nickname is already in use. You can wait some minutes or change your nickname and try again."),
                                     QMessageBox::Ok);
        quit ("");
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
void ircnet::sendusermessage(const usermessage u){
    if(u.has_type(e_RAWCOMMAND))
        tcp_write(u.msg());
    else if(u.has_type(e_CTCP))
        tcp_write("PRIVMSG " + u.receiver() + " :\001" + u.msg() + "\001");
    else if(u.has_type(e_PRIVMSG)){
        if(u.has_type(e_ACTION))
            tcp_write("PRIVMSG " + u.receiver() + " :\001ACTION " + u.msg());
        else
            tcp_write("PRIVMSG " + u.receiver() + " :" + u.msg());
    }
    else if (u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION))
            tcp_write("NOTICE " + u.receiver() + " :\001ACTION " + u.msg());
        else
            tcp_write("NOTICE " + u.receiver() + " :" + u.msg());
    }
    else
        myDebug()<<QString() + "##################void ircnet::sendusermessage(const usermessage u)";
}
void ircnet::refreshlist() {
    if (justgetlist == false) {
        tcp_write("list");
        justgetlist = true;
    }
}
void ircnet::who() {    
    if (whoreceivedcompletely) {
        templist.clear();
        tcp_write("who");
        whoreceivedcompletely = 0;
    }
}
void ircnet::quit(QString s){
    tcp_write("QUIT : [The Wheat Snooper] "+ s);
}
void ircnet::tcp_write(const QString &msg){
    tcp->write(CodecSelectDia::codec->fromUnicode(msg)+"\n");
}
int ircnet::state() const{
    return tcp->state();
}

