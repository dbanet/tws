#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDir>
#include <QSignalMapper>
#include <QProcess>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextDocumentFragment>

#include "snoppanet.h"
#include "settings.h"
#include "settingswindow.h"
#include "hoststruct.h"
#include "inihandlerclass.h"
#include "global_functions.h"
#include "netcoupler.h"

extern inihandlerclass inihandler;
snoppanet::snoppanet(QObject *parent) :
    QObject(parent) {
    signalMapper = new QSignalMapper(this);
    htmlAddress = S_S.getStringList("wormnetserverlist").first();
    if(!htmlAddress.startsWith("http://"))
        htmlAddress="http://"+htmlAddress;
    if(!htmlAddress.endsWith("/"))
        htmlAddress=htmlAddress+"/";
    gameListStarts = 0;
    hostTimer.start(S_S.getint("sbhostrepead"));
    connect(&loginTimer,SIGNAL(timeout()),this,SLOT(logintimertimeout()));
    loginTimer.setSingleShot (true);
}

snoppanet::~snoppanet() {}
void snoppanet::start() {
    request = inihandler.requestfromini("[http login header]");
    request.setUrl(htmlAddress + "/wormageddonweb/Login.asp?UserName=&Password=&IPAddress=");
    reply = manager.get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(readyRead()),this, SLOT(readircip()));
    connect(reply,SIGNAL(finished()),reply,SLOT(deleteLater()));
    loginTimer.start (5000);
}
void snoppanet::logintimertimeout(){
    emit sigloginfailed();
}
void snoppanet::readircip() {    
    temp.append(reply->readAll());
    if (temp.contains(">") && temp.contains("<CONNECT ")) {
        loginTimer.stop ();
        temp = temp.remove("<CONNECT ");
        temp=temp.left(temp.indexOf(">"));
        temp=temp.simplified();
        ircip = temp;
        emit sigircip(ircip);
        temp.clear();        
    }
}
void snoppanet::httpError(QNetworkReply::NetworkError error) {
    static bool noErrorYet=true;
    if(noErrorYet)
        myDebug() << tr("Gameserver disonnected!");
    emit sigloginfailed();
    noErrorYet=false;
}
void snoppanet::setChannelList(const QStringList &sl) {
    currentChannelList = sl;
    requestList.clear();
    foreach(QNetworkReply *n,replyList) {
        Q_CHECK_PTR(n);
        n->deleteLater();
    }
    replyList.clear();
    foreach(QString s,currentChannelList) {
        requestList.push_back(request);
        requestList.last() = inihandler.requestfromini(
                    "[http get host header]");
        channelMap[requestList.size() - 1] = s;
        requestList.last().setUrl(htmlAddress
                                  + "/wormageddonweb/GameList.asp?Channel=" + s.remove("#"));
        replyList.push_back(manager.get(requestList.last()));
        connect(replyList.last(), SIGNAL(readyRead()),signalMapper, SLOT(map()));
        signalMapper->setMapping(replyList.last(), replyList.size() - 1);
        connect(replyList.last(), SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(httpError(QNetworkReply::NetworkError)));
    }
    connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(readgamelist(int)));
    templist.clear();
    for (int i = 0; i < currentChannelList.size(); i++) {
        templist << "";
    }
    hostTimer.disconnect();
    connect(&hostTimer, SIGNAL(timeout()),this, SLOT(hosttimertimeout()));
}
void snoppanet::hosttimertimeout() {
    foreach(QNetworkReply *n,replyList) {
        Q_CHECK_PTR(n);
        n->deleteLater();
    }
    replyList.clear();
    int i = 0;
    foreach(QString s,currentChannelList) {
        replyList.push_back(manager.get(requestList[i]));
        i++;
        connect(replyList.last(), SIGNAL(readyRead()),signalMapper, SLOT(map()));
        signalMapper->setMapping(replyList.last(), replyList.size() - 1);
        connect(replyList.last(), SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(httpError(QNetworkReply::NetworkError)));
    }
    connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(readgamelist(int)));
}
void snoppanet::readgamelist(int i) {
    QByteArray byteArray=replyList[i]->readAll();
    static QTextCodec *codec=QTextCodec::codecForHtml(byteArray,QTextCodec::codecForName("WormsArmageddon"));
    templist[i].append(codec->toUnicode(byteArray));
    if (templist[i].contains("<GAMELISTEND>") && templist[i].contains("<GAMELISTSTART>")) {
        QStringList sl = templist[i].split("\n", QString::SkipEmptyParts);
        templist[i].clear();
        Q_ASSERT(sl.size()>=2);
        while(sl.last().simplified().isEmpty())
            sl.takeLast();
        sl.takeFirst();
        sl.takeLast();
        sl.replaceInStrings("<GAME ", "");
        sl.replaceInStrings("><BR>", "");
        hostlist.clear();
        QStringList sltemp;
        foreach(QString s,sl) {
            hostlist.push_back(hoststruct());
            sltemp = s.split(" ");
            hostlist.last().sethost(sltemp);
        }
        emit sighostlist(hostlist, channelMap[i]);
    } else if (templist[i].size() > 30000)
        templist[i].clear();
    else if (!templist[i].contains("<GAMELISTSTART>"))
        templist[i].clear();
}
void snoppanet::getscheme(QString s) {
    if (scheme.isEmpty()) {
        schemechannel = s;
        QNetworkRequest schemerequest = inihandler.requestfromini("[http get host header]");
        schemerequest.setUrl(htmlAddress
                             + "/wormageddonweb/RequestChannelScheme.asp?Channel="
                             + s.remove("#"));
        schemereply = manager.get(schemerequest);
        connect(schemereply, SIGNAL(readyRead()),this, SLOT(getscheme()));
    }
}
void snoppanet::getscheme() {
    scheme.append(schemereply->readAll());
    scheme = scheme.trimmed();
    if (scheme.startsWith("<SCHEME=",Qt::CaseInsensitive)) {
        scheme.remove("<SCHEME=",Qt::CaseInsensitive);
        int fin = scheme.indexOf(">");
        if (fin != -1)
            scheme.truncate(fin);
        emit
                sigchannelscheme(schemechannel, scheme);
        scheme.clear();
        schemereply->disconnect();
        schemereply = 0;
    }
}
void snoppanet::sendhost(hoststruct h) {
    lasthost=h;
    closehostandstartlasthost(findduplicatedhosts (hostlist));
}
void snoppanet::closehostandstartlasthost(hoststruct h,bool startLastHost) {
    if (!h.isvalid()){
        sendHostRequest();
        return;
    }
    QNetworkRequest hostrequest = inihandler.requestfromini("[http get host header]");
    QString s = htmlAddress + "/wormageddonweb/Game.asp?Cmd=Close&GameID=" + h.id()
            + "&Name=" + h.name() + "&HostID=&GuestID=&GameType=0";
    hostrequest.setUrl(s);
    closehostreply = manager.get(hostrequest);
    if(startLastHost)
        connect(closehostreply,SIGNAL(finished()),this,SLOT(closeHostReplyFinished()));
    connect(closehostreply,SIGNAL(finished()),closehostreply,SLOT(deleteLater()));
}
void snoppanet::sendHostRequest(){
    QNetworkRequest hostRequest = inihandler.requestfromini("[http get host header]");
    QString port;
#ifdef WITH_WORMNAT_SUPPORT
    if(S_S.getbool ("cbwormnat2"))
        port = ":" + getwormnatport ();
    else
        port = ":" + gethostportbyini();
#else
    port = ":" + gethostportbyini();
#endif // WITH_WORMNAT_SUPPORT
    QUrl url(htmlAddress+"/wormageddonweb/Game.asp");
    QHash<QString,QString> parameters;
    parameters.insert("Cmd","Create");
    parameters.insert("Name",lasthost.name());
    parameters.insert("HostIP",lasthost.ip()+port);
    parameters.insert("Nick",lasthost.nick());
    parameters.insert("Pwd",lasthost.pwd());
    parameters.insert("Chan",QString(lasthost.chan()).remove("#"));
    parameters.insert("Loc",lasthost.flagstring());
    parameters.insert("Type","0");
    parameters.insert("Pass","0");
    for(QHash<QString,QString>::iterator i=parameters.begin();i!=parameters.end();i++)
        url.addEncodedQueryItem(QTextCodec::codecForName("WormsArmageddon")->fromUnicode(i.key()),
                                QTextCodec::codecForName("WormsArmageddon")->fromUnicode(i.value()));
    hostRequest.setUrl(url);
    hostreply = manager.get(hostRequest);
    connect(hostreply, SIGNAL(finished()),this, SLOT(hostreplyfinished()));
    connect(hostreply, SIGNAL(finished()),hostreply, SLOT(deleteLater()));
}
void snoppanet::closeLastHost() {
    hoststruct h=lasthost;
    QNetworkRequest hostrequest = inihandler.requestfromini("[http get host header]");
    QString s = htmlAddress + "/wormageddonweb/Game.asp?Cmd=Close&GameID=" + h.id()
            + "&Name=" + h.name() + "&HostID=&GuestID=&GameType=0";
    hostrequest.setUrl(s);
    closehostreply = manager.get(hostrequest);
}
void snoppanet::closeHostReplyFinished(){
    if(closehostreply->error()!=QNetworkReply::NoError)
        return;
    QTimer::singleShot(1000,this,SLOT(sendHostRequest()));
}
void snoppanet::refreshHostList() {
    if (hostreply != 0) {
        hosttimertimeout();
    }
}
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
//###################################################################################
void snoppanet::hostreplyfinished() {
    QString s=hostreply->rawHeader ("SetGameId");
    QString gameid=s.remove (':').simplified ();

    if(gameid.isEmpty ()) {
        myDebug() << tr("WormNET has refused to add your host to the list, please try again later.");
        emit sighostwontstart();
    } else
        emit sigstarthost(gameid);

    /*
    if(gameid.isEmpty ())
        gameid="999";

    s=hostreply->readAll();
    if(containsCI(s,"Object moved")){
        int start=s.indexOf("href=\"");
        start+=6;
        int stop=s.indexOf("\"",start);
        s=s.mid(start,stop-start);
        if(s.startsWith("/"));
        s=s.mid(1);
        s=htmladdress+s;
        //inithosting(s);
        emit sigstarthost(gameid);
    } else if (startswithCI(s,"<NOTHING>")) {
        myDebug() << tr("Wormnet wont start this game, please try again at a later time.");                
        emit sighostwontstart();
    } else
        myDebug()<<tr("No target for hostreply\n")<<s<<tr("Error: ") + hostreply->errorString();
    */
}
hoststruct snoppanet::findmyhost(QList<hoststruct> list){
    QString ip;
    if(S_S.getbool ("cbwormnat2"))
        ip=S_S.getString ("wormnat2address")+ ":" + lasthostport ();
    else
        ip=singleton<netcoupler>().getMyHostIP();
    foreach(hoststruct h,list){
        if(startswithCI(h.ip(),ip))
            return h;
    }
    return hoststruct();
}
hoststruct snoppanet::findduplicatedhosts(QList<hoststruct> list){
    foreach(hoststruct h,list){
        if(startswithCI(h.ip(),singleton<netcoupler>().getMyHostIP())){
            return h;
        }
    }
    return hoststruct();
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

//void snoppanet::inithosting(QString url){
//    QNetworkRequest request;
//    request=inihandler.requestfromini("[http get host header]");
//    request.setUrl(QUrl(url));
//    hostlistforhostingreply=manager.get(request);
//    connect(hostlistforhostingreply,SIGNAL(finished()),this,SLOT(hostlistforhostingreplyfinished()));
//}
//void snoppanet::hostlistforhostingreplyfinished(){
//    static int counter=0;
//    if(hostlistforhostingreply->error()!=QNetworkReply::NoError){
//        emit sighostwontstart();
//        counter=0;
//        return;
//    }
//    QString s=hostlistforhostingreply->readAll();
//    QList<hoststruct> list=hoststruct::extracthostlist(s);
//    hoststruct h=findmyhost(list);
//    if(h.isvalid()){
//        emit sighoststarts(h);
//        counter=0;
//        return;
//    }
//    if(counter>5){
//        counter=0;
//        return;
//    }
//    counter++;
//    QTimer::singleShot(counter*500,this,SLOT(repeathostlistforhostingreplyrequest()));
//}
//void snoppanet::repeathostlistforhostingreplyrequest(){
//    hostlistforhostingreply->deleteLater ();
//    hostlistforhostingreply=manager.get(hostlistforhostingreply->request());
//    connect(hostlistforhostingreply,SIGNAL(finished()),this,SLOT(hostlistforhostingreplyfinished()));
//}
