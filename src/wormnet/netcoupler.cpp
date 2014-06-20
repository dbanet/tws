
#include <QFile>
#include <QClipboard>
#include <QTime>
#include <QMessageBox>

#include "netcoupler.h"
#include "ircnet.h"
#include "snoppanet.h"
#include "inihandlerclass.h"
#include "settings.h"
#include "window.h"
#include "joinprvgame.h"
#include "settingswindow.h"
#include "volumeslider.h"
#include "mainwindow.h"
#include "playername.h"
#ifdef PHONON
#include "sound_handler.h"
#endif
#include "global_functions.h"
#include "global_functions.h"
#include "balloon_handler.h"
#include "hostbox.h"
#include "awayhandler.h"
#include "chatwindow.h"
#include "ctcphandler.h"

extern qint64 waProcessID;
extern volumeslider *volume;
extern inihandlerclass inihandler;
namespace looki {
    QString currentchannel;
}
netcoupler::netcoupler() {
    connectState=e_stoped;
    irc=NULL;
    http=NULL;
    p=NULL;
}
void netcoupler::start(QString s){
    if(!p){                                                     //indicates first start
        p=new QProcess(this);
        p->setProcessChannelMode(QProcess::MergedChannels);
#ifdef PHONON
        connect(volume, SIGNAL(valueChanged (int)),&singleton<soundHandler>(), SLOT(volumechange(int)));
#endif
        connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloonHandler>(), SLOT(buddyArrived()));
        connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloonHandler>(), SLOT(buddyLeft()));
        connect(this,SIGNAL(sigConnected()),&singleton<balloonHandler>(),SLOT(connected()));
        connect(this,SIGNAL(sigDisconnected()),&singleton<balloonHandler>(),SLOT(disconnected()));
        connect(p, SIGNAL(readyRead()),this, SLOT(readProcess()));

        connect(this, SIGNAL(sigSettingsWindowChanged()),this, SLOT(initSoundAndStartWho()));
        connect(this, SIGNAL(sigSettingsWindowChanged()),&users, SLOT(usesettingswindow()));
    }
    connectState=e_started;
    nick=s;
    S_S.append("mutedusers", nick);
    irc = new ircnet(s, this);
    connect(irc,SIGNAL(sigConnected()),this,SLOT(ircConnected()));
    connect(irc,SIGNAL(sigDisconnected()),this,SLOT(ircDisconnected()));
    http = new snoppanet(this);
    connect(http, SIGNAL(sigchannelscheme(QString,QString)),this, SLOT(getScheme(QString,QString)));
    connect(irc, SIGNAL(sigGotUserMessage(const usermessage)),this,SLOT(getUserMessage(const usermessage)));
    connect(irc, SIGNAL(sigGotIdleTime(const QString&,int,int)),this, SIGNAL(sigGotIdleTime(const QString&,int,int)));
    connect(irc, SIGNAL(sigNoSuchNick(const QString&)),this, SIGNAL(sigNoSuchNick(const QString&)));
    connect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(getHostList(QList<hoststruct>,QString)));
    connect(http,SIGNAL(sigloginfailed()),this,SIGNAL(sigDisconnected()));
    connect(http,SIGNAL(sigstarthost(QString)),this,SLOT(getMyWormnetHost(QString)));
//    connect(http,SIGNAL(sighoststarts(QString)),this,SLOT(getmywormnethost(hoststruct)));

    /****************************************************************/
    connect(irc, SIGNAL(sigIRCReceivedChanList(QStringList)),this,SLOT(emitSigGotChanList(QStringList)));
    connect(irc, SIGNAL(sigIRCJoinedChannel(QString,int)),this,SLOT(emitSigJoinedChannel(QString,int)));
    connect(irc, SIGNAL(sigIRCUpdatedAmountOfUsers(QString,int)),this,SLOT(emitSigUpdatedAmountOfUsers(QString,int)));
    /**************************************************** ~~dbanet **/

    QStringList sl = inihandler.stringlistfromini("[irc ip]");
    if (sl.isEmpty()) {
        connect(http, SIGNAL(sigircip(QString)),this, SLOT(getIRCIP(QString)));
        http->start();
    } else {
        ircIP = sl.first();
        irc->setIP(ircIP);
        irc->start();
        initSoundAndStartWho();
    }    
    connect(&loopTimer,SIGNAL(timeout()),this,SLOT(loopTimerTimeout()));
    loopTimer.start(30*1000);
}
netcoupler::~netcoupler() {        
    irc->deleteLater();
    http->deleteLater();    
}
void netcoupler::stop(){
    connectState=e_stoped;
    sendQuit(S_S.getString("information"));
}
void netcoupler::joinChannel(const QString &s) {
    if(irc)
        irc->joinChannel(s);
    if(!http)
        return;
    http->getscheme(s);
    if (!listOfJoinedChannels.contains(s))
        listOfJoinedChannels << s;
    http->setChannelList(listOfJoinedChannels);
}
void netcoupler::partChannel(const QString &s) {
    irc->partChannel(s);
    listOfJoinedChannels.removeOne(s);
    http->setChannelList(listOfJoinedChannels);
}
void netcoupler::emitSigGotChanList(QStringList chanList) {
    emit sigGotChanList(chanList);
}
void netcoupler::emitSigJoinedChannel(QString channel,int amountOfUsers) {
    emit sigJoinedChannel(channel,amountOfUsers);
}
void netcoupler::emitSigUpdatedAmountOfUsers(QString channel,int amountOfUsers){
    emit sigUpdatedAmountOfUsers(channel,amountOfUsers);
}
void netcoupler::getIRCIP(QString s) {
    if(!irc)
        return;
    irc->setIP(s);
    irc->start();
    initSoundAndStartWho();
}
void netcoupler::getHostList(QList<hoststruct> l, QString s) {
    refreshList();
    hosts.sethoststruct(l, s);
}
void netcoupler::getUserMessage(const usermessage u){
    emit sigGotUserMessage(u);
}
void netcoupler::sendUserMessage(const usermessage u){
    if(!irc)
        return;
    if(u.has_type(e_SCRIPTCOMMAND))
        getUserMessage(u);
    else
        irc->sendUserMessage(u);
}
void netcoupler::refreshList() {
    if(!irc)
        return;
    irc->refreshList();
}
void netcoupler::sendQuit(QString s){
    if(irc==NULL)
        return;
    irc->quit(s);
}
void netcoupler::getScheme(QString chan, QString scheme) {
    schemeMap[chan] = scheme;
}
void netcoupler::refreshWho() {
    qDebug()<<"void netcoupler::refreshWho()";
}
void netcoupler::ircConnected(){
    emit sigConnected();
}
void netcoupler::ircDisconnected(){
    users.clear();
    emit sigDisconnected();
}
void netcoupler::joinGameLink(const QString &gamelink) {
    joinprvgame *cast = qobject_cast<joinprvgame*> (sender());
    Q_CHECK_PTR(cast);
    looki::currentchannel = cast->chan;
    QString temp = getProcessString();
    if (temp == QString())
        return;
    startProcess(temp, QStringList(gamelink));
}
void netcoupler::joinGame(const QString &hostinfo, const QString &channel, const QString &gamename) {
    looki::currentchannel = channel;
    QString temp = getProcessString();
    if (temp == QString())
        return;
    startProcess(temp, QStringList(hostinfo));
    if (S_S.getbool("chbactionwhenjoining"))
        sendInfoToChan(channel, "joined a game: " + gamename);
}
void netcoupler::lookedUpSnoopersIPAddress(const QHostInfo &host){
    if(host.error()==QHostInfo::NoError)
        myIP=host.addresses().first().toString();
}
void netcoupler::lookedUpSnoopersIPAddress(QString address){
    myIP=address;
}
void netcoupler::createHost(QString id){
    if(!http)
        return;
    if (users.users.indexOf(userstruct::whoami(nick)) == -1)
        return;
    QString temp = getProcessString();
    if (temp == QString())
        return;
    QStringList args;
    QString sLink = "wa://" + "?gameid="+ id + "&scheme=" + schemeMap[looki::currentchannel];
    if(!http->lasthost.pwd().isEmpty())
        sLink += "&password=" + http->lasthost.pwd();
    args << sLink;
#ifdef WITH_WORMNAT_SUPPORT
    if(S_S.getbool ("cbwormnat2"))
        args << getwormnat2commandline();
#endif
    startProcess(temp, args);
}
//void netcoupler::createhost(hoststruct h) {
//    if(!http)
//        return;
//    if (users.users.indexOf(userstruct::whoami(nick)) == -1)
//        return;
//    QString temp = getprocessstring();
//    if (temp == QString())
//        return;
//    QString s;
//    if(!http->lasthost.pwd().isEmpty())
//        s="&password="+http->lasthost.pwd();
//    if(S_S.getbool ("cbwormnat2"))
//        temp += getwormnat2commandline();
//    temp = temp + " \"" + "wa://" + "?gameid="+ h.id() + "&scheme=" + schememap[looki::currentchannel] + s+"\"";
//    startprocess(temp);
//}
void netcoupler::sendHostInfoToServerAndHost(const QString &name,const QString &pwd, const QString &chan,int flag){
    if(!http)
        return;
    looki::currentchannel=chan;
    QString s=nick;
    if (!S_S.getString("leplayername").isEmpty())
        s=S_S.getString("leplayername");
    hoststruct h;
    QString hostcountrynumber=singleton<pictureHandler>().mapHostCountryCodeToNumber(singleton<pictureHandler>().mapNumberToCountryCode(flag));
    h.sethost(name,s,getMyHostIP(),flag,"??","",pwd,chan,hostcountrynumber);
    http->sendhost(h);
//    connect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
}
void netcoupler::getMyWormnetHost(QString id){
    QString ip=getMyHostIP();
//    disconnect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));    
    QString host = QString("wa://%1?gameid="+id + "&scheme=%2").arg(ip + ":" + lasthostport ()).arg(schemeMap[looki::currentchannel]);
    QString msg = QString("hosted a game: %1, %2").arg(S_S.getString("legamename")).arg(host);
    if (S_S.getbool("chbsendhostinfotochan"))
        sendInfoToChan(looki::currentchannel, msg);
    createHost(id);
}
//void netcoupler::getmywormnethost(hoststruct h){
//    QString address=getmyhostip();
//    disconnect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
//    QString host = QString("wa://%1?gameid="+h.id()+"&scheme=%2").arg(address).arg(schememap[looki::currentchannel]);
//    QString msg = QString(" is hosting a game: %1, %2").arg(h.name()).arg(host);
//    if (S_S.getbool("chbsendhostinfotochan"))
//        sendinfotochan(looki::currentchannel, msg);
//    createhost(h);
//}
QString netcoupler::getMyHostIP(){
    QString address=myIP;
    if(S_S.getbool ("cbwormnat2"))
        address=S_S.getString ("wormnat2address");
    else if(S_S.getbool("useacostumipforhosting"))
        address=S_S.getString("costumipforhosting");
    return address;
}
void netcoupler::readProcess() {
    qWarning() << qobject_cast<QProcess*> (sender())->readAll();
}
QString netcoupler::getProcessString() {
    QStringList sl = S_S.getStringList("joinstrings");
    if (sl.isEmpty()){
        QMessageBox::warning(0, "", tr("No executables are given.\n"
                                       "you must choose a game executable,\n"
                                       "for example wa.exe, to join a game.\n"),
                             QMessageBox::Ok);
        return QString();
    }
#ifdef Q_OS_UNIX
    if (!sl.isEmpty()) {
        QString file = sl.first();
        QFile f(file);
        if (f.open(QFile::ReadOnly)) {
            QTextStream ts(&f);
            QString s = ts.readLine();
            int i = 0;
            while (!s.startsWith("Exec=") && !ts.atEnd() && i < 50) {
                i++;
                s = ts.readLine();
            }
            if (!ts.atEnd() && i < 50) {
                QString temp = s;
                temp.remove("Exec=");
                return temp;
            }
        }
    }
#elif defined(Q_OS_WIN32) | defined(Q_OS_OS2)
    if(!sl.isEmpty()) {
        if(sl.first().contains("WormKit.exe",Qt::CaseInsensitive)) {            
            return QString("\"")+sl.first()+"\" wa.exe";
        }
        else {
            return QString("\"")+sl.first()+"\"";
        }
    }
#endif
    return "";
}
void netcoupler::sendInfoToChan(const QString &chan, const QString &msg) {
    usermessage u(msg, usermessage_type(e_PRIVMSG | e_ACTION) , chan);
    sendUserMessage(u);
    emit sigGotUserMessage(u);
}
void netcoupler::initSoundAndStartWho() {
#ifdef PHONON
    singleton<soundHandler>().init();
#endif
}
void netcoupler::settingsWindowEmitFunction() { //signals are protected?!
    emit sigSettingsWindowChanged();
}
void netcoupler::refreshHostList() {
    http->refreshHostList();
}
void netcoupler::startProcess(const QString &filePath, const QStringList &args){
    if(S_S.getbool("chbhidechannelwindowsongame")){
        foreach(window *w,qobjectwrapper<mainwindow>::ref().windowList)
            w->minimize();       
    }
    if(S_S.getbool("chbdisconnectongame"))
        qobjectwrapper<mainwindow>::ref().returnToLoginTab();
    if(S_S.getbool("cbsetawaywhilegaming")){
        qobjectwrapper<awayhandler>::ref().setawaywhilegameing();
    }
    p->startDetached(filePath, args, QFileInfo(filePath).dir().canonicalPath(), &waProcessID);
}
int netcoupler::ircState(){
    if(irc)
        return irc->state();
    else
        return QAbstractSocket::UnconnectedState;

}
void netcoupler::loopTimerTimeout(){
    safeusergarbage();
}
bool netcoupler::buddyListContains(QString user){
    return users.usermap[usermodel::tr("Buddylist")].count(userstruct::whoami(user));
}
bool netcoupler::ignoreListContains(QString user){
    return users.usermap[usermodel::tr("Ignorelist")].count(userstruct::whoami(user));
}
