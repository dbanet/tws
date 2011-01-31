#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QFile>
#include<QTextStream>
#include<QApplication>
#include<QDir>
#include<QSignalMapper>
#include<QProcess>
#include<QMessageBox>
#include<QTextCodec>
#include<QTextDocumentFragment>

#include"snoppanet.h"
#include"settings.h"
#include"settingswindow.h"
#include"hoststruct.h"
#include"inihandlerclass.h"
#include"global_functions.h"
#include"netcoupler.h"

extern inihandlerclass inihandler;
snoppanet::snoppanet(QObject *parent) :
	QObject(parent) {        
    signalmapper = new QSignalMapper(this);
    htmladdress = S_S.getstringlist("wormnetserverlist").first();
    if(!htmladdress.startsWith("http://"))
        htmladdress="http://"+htmladdress;
    if(!htmladdress.endsWith("/"))
        htmladdress=htmladdress+"/";
    gameliststarts = 0;
    int delay = singleton<settingswindow>().from_map("sbhostrepead").value<int> ();
    hosttimer.start(delay);
}

snoppanet::~snoppanet() {   
    signalmapper->deleteLater();
}
void snoppanet::start() {
    request = inihandler.requestfromini("[http login header]");
    request.setUrl(htmladdress + "/wormageddonweb/Login.asp?UserName=&Password=&IPAddress=");
    reply = manager.get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(readyRead()),this, SLOT(readircip()));
}
void snoppanet::readircip() {
    temp.append(reply->readAll());
    if (temp.contains(">") && temp.contains("<CONNECT ")) {
        temp = temp.remove("<CONNECT ").remove(">").trimmed();
        ircip = temp;
        emit sigircip(ircip);
        temp.clear();
        reply->disconnect();
        reply->deleteLater();
    }
}
void snoppanet::httpError(QNetworkReply::NetworkError error) {
    static bool noErrorYet=true;
    if(noErrorYet)
        myDebug() << tr("Gameserver disonnected!");
    noErrorYet=false;
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
        requestlist.last().setUrl(htmladdress
                                  + "/wormageddonweb/GameList.asp?Channel=" + s.remove("#"));
        replylist.push_back(manager.get(requestlist.last()));
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
    hosttimer.disconnect();
    connect(&hosttimer, SIGNAL(timeout()),this, SLOT(hosttimeout()));
}
void snoppanet::hosttimeout() {
    foreach(QNetworkReply *n,replylist) {
        Q_CHECK_PTR(n);
        n->deleteLater();
    }
    replylist.clear();
    int i = 0;
    foreach(QString s,currentchannellist) {
        replylist.push_back(manager.get(requestlist[i]));
        i++;
        connect(replylist.last(), SIGNAL(readyRead()),signalmapper, SLOT(map()));
        signalmapper->setMapping(replylist.last(), replylist.size() - 1);
        connect(replylist.last(), SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(httpError(QNetworkReply::NetworkError)));
    }
    connect(signalmapper, SIGNAL(mapped(int)),this, SLOT(readgamelist(int)));
}
void snoppanet::readgamelist(int i) {
    QByteArray byteArray=replylist[i]->readAll();
    static QTextCodec *codec=QTextCodec::codecForHtml(byteArray,QTextCodec::codecForName("UTF-8"));
    templist[i].append(codec->toUnicode(byteArray));
    if (templist[i].contains("<GAMELISTEND>") && templist[i].contains("<GAMELISTSTART>")) {
        QStringList sl = templist[i].split("\n", QString::SkipEmptyParts);
        templist[i].clear();
        Q_ASSERT(sl.size()>=2);
        while(sl.last().trimmed().isEmpty())
            sl.takeLast();
        sl.takeFirst();
        sl.takeLast();
        sl.replaceInStrings("<GAME ", "");
        sl.replaceInStrings("><BR>", "");
        hostlist.clear();
        QStringList sltemp;
        foreach(QString s,sl) {
            s=s.remove(QChar(65533));
            hostlist.push_back(hoststruct());
            sltemp = s.split(" ");
            hostlist.last().sethost(sltemp);
        }
        emit sighostlist(hostlist, channelmap[i]);
    } else if (templist[i].size() > 30000)
        templist[i].clear();
    else if (!templist[i].contains("<GAMELISTSTART>"))
        templist[i].clear();
}
void snoppanet::getscheme(QString s) {
    if (scheme.isEmpty()) {
        schemechannel = s;
        QNetworkRequest schemerequest = inihandler.requestfromini("[http get host header]");
        schemerequest.setUrl(htmladdress
                             + "/wormageddonweb/RequestChannelScheme.asp?Channel="
                             + s.remove("#"));
        schemereply = manager.get(schemerequest);
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
void snoppanet::sendhost(hoststruct h) {
    lasthost=h;
    closehostandstartlasthost(findduplicatedhosts(hostlist));
}
void snoppanet::sendhostrequest(){
    QNetworkRequest hostrequest = inihandler.requestfromini("[http get host header]");
    QString s = htmladdress + "/wormageddonweb/Game.asp?Cmd=Create&Name=" + lasthost.name()
                + "&HostIP=" + lasthost.ip() + "&Nick=" + lasthost.nick() + "&Pwd=" + lasthost.pwd() + "&Chan="
                + QString(lasthost.chan()).remove("#") + "&Loc=" + lasthost.flagstring() + "&Type="+lasthost.countrynumber()+"&Pass=0";
    QString port=gethostport();
    if(port!="")
        s = htmladdress + "/wormageddonweb/Game.asp?Cmd=Create&Name=" + lasthost.name()
            + "&HostIP=" + lasthost.ip()+":"+port + "&Nick=" + lasthost.nick() + "&Pwd=" + lasthost.pwd() + "&Chan="
            + QString(lasthost.chan()).remove("#") + "&Loc=" + lasthost.flagstring() + "&Type=0&Pass=0";
    hostrequest.setUrl(s);
    hostreply = manager.get(hostrequest);
    connect(hostreply, SIGNAL(finished()),this, SLOT(readhostreply()));
}
void snoppanet::closehostandstartlasthost(hoststruct h) {
    if (!h.isvalid()){
        sendhostrequest();
        return;
    }
    QNetworkRequest hostrequest = inihandler.requestfromini("[http get host header]");
    QString s = htmladdress + "/wormageddonweb/Game.asp?Cmd=Close&GameID=" + h.id()
                + "&Name=" + h.name() + "&HostID=&GuestID=&GameType=0";
    hostrequest.setUrl(s);
    closehostreply = manager.get(hostrequest);
    connect(closehostreply,SIGNAL(finished()),this,SLOT(closehostreplyfinished()));
}
void snoppanet::closelasthost() {
    hoststruct h=lasthost;
    QNetworkRequest hostrequest = inihandler.requestfromini("[http get host header]");
    QString s = htmladdress + "/wormageddonweb/Game.asp?Cmd=Close&GameID=" + h.id()
                + "&Name=" + h.name() + "&HostID=&GuestID=&GameType=0";
    hostrequest.setUrl(s);
    closehostreply = manager.get(hostrequest);
}
void snoppanet::closehostreplyfinished(){
    if(closehostreply->error()!=QNetworkReply::NoError)
        return;
    QTimer::singleShot(1000,this,SLOT(sendhostrequest()));
}
void snoppanet::refreshhostlist() {
    if (hostreply != 0) {
         hosttimeout();
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
void snoppanet::readhostreply() {
    QString s=hostreply->readAll();
    if(containsCI(s,"Object moved")){
        int start=s.indexOf("href=\"");
        start+=6;
        int stop=s.indexOf("\"",start);
        s=s.mid(start,stop-start);
        if(s.startsWith("/"));
        s=s.mid(1);
        s=htmladdress+s;
        inithosting(s);

    } else if (startswithCI(s,"<NOTHING>\n")) {
        myDebug() << tr("Wormnet wont start this game, please try again at a later time.");
        hostreply->disconnect();
        hostreply->deleteLater();
        emit sighostwontstart();
    } else
        myDebug()<<"No target for hostreply\n"<<s;
}
void snoppanet::inithosting(QString url){
    QNetworkRequest request;
    request=inihandler.requestfromini("[http get host header]");
    request.setUrl(QUrl(url));
    hostlistforhostingreply=manager.get(request);
    connect(hostlistforhostingreply,SIGNAL(finished()),this,SLOT(hostlistforhostingreplyfinished()));
}
void snoppanet::hostlistforhostingreplyfinished(){
    static int counter=0;
    if(hostlistforhostingreply->error()!=QNetworkReply::NoError){
        emit sighostwontstart();
        counter=0;
        return;
    }
    QString s=hostlistforhostingreply->readAll();
    QList<hoststruct> list=hoststruct::extracthostlist(s);
    hoststruct h=findduplicatedhosts(list);
    if(h.isvalid()){
        emit sighoststarts(h);
        counter=0;
        return;
    }
    if(counter>5){
        counter=0;
        return;
    }
    counter++;
    QTimer::singleShot(counter*500,this,SLOT(repeathostlistforhostingreplyrequest()));
}
hoststruct snoppanet::findduplicatedhosts(QList<hoststruct> list){
    foreach(hoststruct h,list){
        if(startswithCI(h.ip(),singleton<netcoupler>().getmyhostip())){
            return h;
        }
    }
    return hoststruct();
}
void snoppanet::repeathostlistforhostingreplyrequest(){
    hostlistforhostingreply->deleteLater();
    hostlistforhostingreply=manager.get(hostlistforhostingreply->request());
    connect(hostlistforhostingreply,SIGNAL(finished()),this,SLOT(hostlistforhostingreplyfinished()));
}
