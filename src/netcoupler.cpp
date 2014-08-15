
#include <QFile>
#include <QClipboard>
#include <QTime>
#include <QMessageBox>

#include "netcoupler.h"
#include "ircnet.h"
#include "snoppanet.h"
#include "inihandlerclass.h"
#include "settings.h"
#include "channel/channeltab.h"
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

qint64 waProcessId;
extern volumeslider *volume;
extern inihandlerclass inihandler;
namespace looki {
    QString currentchannel;
}
netcoupler::netcoupler() {
    connectstate=e_stoped;
    irc=NULL;
    http=NULL;
    p=NULL;
}
void netcoupler::start(QString s,MainWindow *mainWnd){
    this->mainWnd=mainWnd;
    if(!p){                                                     //indicates first start
        p=new QProcess(this);
        p->setProcessChannelMode(QProcess::MergedChannels);
#ifdef PHONON
        connect(volume, SIGNAL(valueChanged (int)),&singleton<soundHandler>(), SLOT(volumechange(int)));
#endif
        connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloonHandler>(), SLOT(buddyArrived()));
        connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloonHandler>(), SLOT(buddyLeft()));
        connect(this,SIGNAL(sigconnected()),&singleton<balloonHandler>(),SLOT(connected()));
        connect(this,SIGNAL(sigdisconnected()),&singleton<balloonHandler>(),SLOT(disconnected()));
        connect(p, SIGNAL(readyRead()),this, SLOT(readprocess()));

        connect(this, SIGNAL(sigsettingswindowchanged()),this, SLOT(initSoundAndStartWho()));
        connect(this, SIGNAL(sigsettingswindowchanged()),&users, SLOT(usesettingswindow()));
    }
    connectstate=e_started;
    nick=s;
    S_S.append("mutedusers", nick);
    irc = new ircnet(s, this);
    connect(irc,SIGNAL(sigconnected()),this,SLOT(ircconnected()));
    connect(irc,SIGNAL(sigdisconnected()),this,SLOT(ircdisconnected()));
    http = new snoppanet(this);
    connect(http, SIGNAL(sigchannelscheme(QString,QString)),this, SLOT(getscheme(QString,QString)));
    connect(irc, SIGNAL(siggotusermessage(const usermessage)),this,SLOT(getusermessage(const usermessage)));
    connect(irc, SIGNAL(siggotidletime(const QString&,int,int)),this, SIGNAL(siggotidletime(const QString&,int,int)));
    connect(irc, SIGNAL(signosuchnick(const QString&)),this, SIGNAL(signosuchnick(const QString&)));
    connect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlist(QList<hoststruct>,QString)));
    connect(http,SIGNAL(sigloginfailed()),this,SIGNAL(sigdisconnected()));
    connect(http,SIGNAL(sigstarthost(QString)),this,SLOT(getmywormnethost(QString)));
//    connect(http,SIGNAL(sighoststarts(QString)),this,SLOT(getmywormnethost(hoststruct)));

    /****************************************************************/
    connect(irc, SIGNAL(sigIRCReceivedChanList(QStringList)),this,SLOT(emitSigGotChanList(QStringList)));
    connect(irc, SIGNAL(sigIRCJoinedChannel(QString,int)),this,SLOT(emitSigJoinedChannel(QString,int)));
    connect(irc, SIGNAL(sigIRCUpdatedAmountOfUsers(QString,int)),this,SLOT(emitSigUpdatedAmountOfUsers(QString,int)));
    /**************************************************** ~~dbanet **/

    QStringList sl = inihandler.stringlistfromini("[irc ip]");
    if (sl.isEmpty()) {
        connect(http, SIGNAL(sigircip(QString)),this, SLOT(getircip(QString)));
        http->start();
    } else {
        ircip = sl.first();
        irc->setip(ircip);
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
    connectstate=e_stoped;
    sendquit(S_S.getString("information"));
}
void netcoupler::joinchannel(const QString &s) {
    if(irc)
        irc->joinchannel(s);
    if(!http)
        return;
    http->getscheme(s);
    if (!listofjoinedchannels.contains(s))
        listofjoinedchannels << s;
    http->setChannelList(listofjoinedchannels);
}
void netcoupler::partchannel(const QString &s) {
    irc->partchannel(s);
    listofjoinedchannels.removeOne(s);
    http->setChannelList(listofjoinedchannels);
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
void netcoupler::getircip(QString s) {
    if(!irc)
        return;
    irc->setip(s);
    irc->start();
    initSoundAndStartWho();
}
void netcoupler::gethostlist(QList<hoststruct> hosts, QString channel){
    refreshlist();

    /* updating each channelTab's QList of hoststructs with the new one */
    channelTab *chanTab=getChannelTabByChannelName(channel);
    if(chanTab!=0){
        QList<hoststruct> newHosts;
        foreach(hoststruct host,hosts)
            if(host.chan()==channel)
                newHosts.append(host);
        chanTab->setHosts(&newHosts);
    } else return;
}
channelTab* netcoupler::getChannelTabByChannelName(QString channel){
    foreach(channelTab *chanTab,mainWnd->channelTabs)
        if(chanTab->currentChannel==channel)
            return chanTab;
    return 0;
}
void netcoupler::getusermessage(const usermessage u){
    emit siggotusermessage(u);
}
void netcoupler::sendusermessage(const usermessage u){
    if(!irc)
        return;
    if(u.has_type(e_SCRIPTCOMMAND))
        getusermessage(u);
    else
        irc->sendusermessage(u);
}
void netcoupler::refreshlist() {
    if(!irc)
        return;
    irc->refreshlist();
}
void netcoupler::sendquit(QString s){
    if(irc==NULL)
        return;
    irc->quit(s);
}
void netcoupler::getscheme(QString chan, QString scheme) {
    schememap[chan] = scheme;
}
void netcoupler::refreshwho() {
    qDebug()<<"void netcoupler::refreshwho()";
}
void netcoupler::ircconnected(){
    emit sigconnected();
}
void netcoupler::ircdisconnected(){
    users.clear();
    emit sigdisconnected();
}
void netcoupler::joingamelink(const QString &gamelink) {
    joinprvgame *cast = qobject_cast<joinprvgame*> (sender());
    Q_CHECK_PTR(cast);
    looki::currentchannel = cast->chan;
    QString temp = getprocessstring();    
    if (temp == QString())
        return;
    startprocess(temp,QStringList(gamelink));
}
void netcoupler::joingame(const QString &hostinfo, const QString &channel, const QString &gamename) {
    looki::currentchannel = channel;
    QString temp = getprocessstring();
    if (temp == QString())
        return;
    startprocess(temp,QStringList(hostinfo));
    if (S_S.getbool("chbactionwhenjoining"))
        sendinfotochan(channel, "joined a game: " + gamename);
}
void netcoupler::lookedUpSnoopersIPAddress(const QHostInfo &host){
    if(host.error()==QHostInfo::NoError)
        myip=host.addresses().first().toString();
}
void netcoupler::lookedUpSnoopersIPAddress(QString address){
    myip=address;
}
void netcoupler::createhost(QString id){
    if(!http)
        return;
    if (users.users.indexOf(userstruct::whoami(nick)) == -1)
        return;
    QString temp = getprocessstring();
    if (temp == QString())
        return;
    QStringList args;
    QString sLink=QString("wa://?gameid=%1&scheme=%2").arg(id,schememap[looki::currentchannel]);
    if(!http->lasthost.pwd().isEmpty())
        sLink+=QString("&password=%1").arg(http->lasthost.pwd());
    args<<sLink;
#ifdef WITH_WORMNAT_SUPPORT
    if(S_S.getbool("cbwormnat2"))
        args<<getwormnat2commandline();
#endif
    startprocess(temp,args);
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
void netcoupler::sendhostinfotoserverandhost(const QString &name,const QString &pwd, const QString &chan,int flag){
    if(!http)
        return;
    looki::currentchannel=chan;
    QString s=nick;
    if (!S_S.getString("leplayername").isEmpty())
        s=S_S.getString("leplayername");
    hoststruct h;
    QString hostcountrynumber=singleton<pictureHandler>().mapHostCountryCodeToNumber(singleton<pictureHandler>().mapNumberToCountryCode(flag));
    h.sethost(name,s,getmyhostip(),flag,"??","",pwd,chan,hostcountrynumber);
    http->sendhost(h);
//    connect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
}
void netcoupler::getmywormnethost(QString id){
    QString ip=getmyhostip();
//    disconnect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));    
    QString host=QString("wa://%1:%2?gameid=%3&scheme=%4").arg(ip,lasthostport(),id,schememap[looki::currentchannel]);
    QString msg=QString("hosted a game: %1, %2").arg(S_S.getString("legamename")).arg(host);
    if (S_S.getbool("chbsendhostinfotochan"))
        sendinfotochan(looki::currentchannel, msg);
    createhost(id);
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
QString netcoupler::getmyhostip(){
    QString address=myip;
    if(S_S.getbool ("cbwormnat2"))
        address=S_S.getString ("wormnat2address");
    else if(S_S.getbool("useacostumipforhosting"))
        address=S_S.getString("costumipforhosting");
    return address;
}
void netcoupler::readprocess() {
    qWarning() << qobject_cast<QProcess*> (sender())->readAll();
}
QString netcoupler::getprocessstring() {
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
void netcoupler::sendinfotochan(const QString &chan, const QString &msg) {    
    usermessage u(msg, usermessage_type(e_PRIVMSG | e_ACTION) , chan);
    sendusermessage(u);
    emit siggotusermessage(u);
}
void netcoupler::initSoundAndStartWho() {
#ifdef PHONON
    singleton<soundHandler>().init();
#endif
}
void netcoupler::settingswindowemitfunktion() { //signals are protected?!
    emit sigsettingswindowchanged();
}
void netcoupler::refreshhostlist() {
    http->refreshHostList();
}
void netcoupler::startprocess(const QString &filePath,const QStringList &args){
    bool alreadyRunning = waProcessId!=0 && isProcessRunning(waProcessId);
    if(S_S.getbool("chbhidechannelwindowsongame")){
        foreach(channelTab *w,qobjectwrapper<MainWindow>::ref().channelTabs)
            w->minimize();       
    }
    if(S_S.getbool("chbdisconnectongame"))
        qobjectwrapper<MainWindow>::ref().returntologintab();
    if(S_S.getbool("cbsetawaywhilegaming") && !alreadyRunning){
        qobjectwrapper<awayhandler>::ref().setawaywhilegameing();
    }
    p->startDetached(filePath,args,QFileInfo(filePath).dir().canonicalPath(),alreadyRunning?0:&waProcessId);
}
int netcoupler::ircstate(){
    if(irc)
        return irc->state();
    else
        return QAbstractSocket::UnconnectedState;

}
void netcoupler::loopTimerTimeout(){
    safeusergarbage();
}
bool netcoupler::buddylistcontains(QString user){
    return users.usermap[usermodel::tr("Buddylist")].count(userstruct::whoami(user));
}
bool netcoupler::ignorelistcontains(QString user){
    return users.usermap[usermodel::tr("Ignorelist")].count(userstruct::whoami(user));
}
