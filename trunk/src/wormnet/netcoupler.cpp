#include<QFile>
#include<QClipboard>
#include<QTime>
#include<QMessageBox>

#include"netcoupler.h"
#include"ircnet.h"
#include"snoppanet.h"
#include"inihandlerclass.h"
#include"settings.h"
#include"window.h"
#include"joinprvgame.h"
#include"settingswindow.h"
#include"volumeslider.h"
#include"mainwindow.h"
#include"playername.h"
#include"sound_handler.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"global_functions.h"
#include"balloon_handler.h"
#include"hostbox.h"
#include"awayhandler.h"
#include"chatwindow.h"
#include"ctcphandler.h"

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
void netcoupler::start(QString s){
    if(!p){                                                     //indicates first start
        p=new QProcess(this);
        p->setProcessChannelMode(QProcess::MergedChannels);
        connect(volume, SIGNAL(valueChanged (int)),&singleton<sound_handler>(), SLOT(volumechange(int)));
        connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloon_handler>(), SLOT(buddyarrived()));
        connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloon_handler>(), SLOT(buddyleft()));
        connect(this,SIGNAL(sigconnected()),&singleton<balloon_handler>(),SLOT(connected()));
        connect(this,SIGNAL(sigdisconnected()),&singleton<balloon_handler>(),SLOT(disconnected()));
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
    connect(irc, SIGNAL(siggetlist(QStringList)),this, SLOT(getchannellist(QStringList)));    
    connect(irc, SIGNAL(siggotusermessage(const usermessage)),this,SLOT(getusermessage(const usermessage)));
    connect(irc, SIGNAL(siggotidletime(const QString&, int)),this, SIGNAL(siggotidletime(const QString&, int)));
    connect(irc, SIGNAL(signosuchnick(const QString&)),this, SIGNAL(signosuchnick(const QString&)));
    connect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlist(QList<hoststruct>,QString)));
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
    userstruct::addressischecked=0;
    sendquit();
}
void netcoupler::joinchannel(const QString &s) {
    if(irc)
        irc->joinchannel(s);
    if(!http)
        return;
    http->getscheme(s);
    if (!listofjoinedchannels.contains(s))
        listofjoinedchannels << s;
    http->setchannellist(listofjoinedchannels);
}
void netcoupler::partchannel(const QString &s) {
    irc->partchannel(s);
    listofjoinedchannels.removeOne(s);
    http->setchannellist(listofjoinedchannels);
}
void netcoupler::getchannellist(QStringList sl) {
    channellist.clear();
    foreach(QString s,sl)
        channellist << s.split(" ").first();    
    foreach(QString s, channellist) {
        if(!users.usermap_channellist_helper.contains(s,Qt::CaseInsensitive))
            users.usermap_channellist_helper.push_back(s);
    }
    emit siggotchanellist(sl);
}
void netcoupler::getircip(QString s) {
    if(!irc)
        return;
    irc->setip(s);
    irc->start();
    initSoundAndStartWho();
}
void netcoupler::gethostlist(QList<hoststruct> l, QString s) {
    refreshlist();
    hosts.sethoststruct(l, s);
}
void netcoupler::getwholist() {
    if(!irc)
        return;
    if(connectstate!=e_started)
        return;    
    users.setuserstruct(irc->wholist, irc->joinlist);
    irc->who();
}
void netcoupler::getusermessage(const usermessage u){
    emit siggotusermessage(u);
}
void netcoupler::sendusermessage(const usermessage u){
    if(!irc)
        return;
    irc->sendusermessage(u);
}
void netcoupler::refreshlist() {
    if(!irc)
        return;
    irc->refreshlist();
}
void netcoupler::sendquit() {
    if(irc==NULL)
        return;
    irc->quit();
}
void netcoupler::getscheme(QString chan, QString scheme) {
    schememap[chan] = scheme;
}
void netcoupler::refreshwho() {

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
    temp = temp + " \"" + gamelink + "\"";
    startprocess(temp);
}
void netcoupler::joingame(const QString &hostinfo, const QString &channel, const QString &gamename) {
    looki::currentchannel = channel;
    QString temp = getprocessstring();
    if (temp == QString())
        return;
    temp = temp + hostinfo;
    startprocess(temp);
    if (S_S.getbool("chbactionwhenjoining"))
        sendinfotochan(channel, " is joining a game: " + gamename);
}
void netcoupler::createhost(hoststruct h) {
    if(!http)
        return;
    if (users.users.indexOf(userstruct::whoami(nick)) == -1)
        return;
    QString temp = getprocessstring();
    if (temp == QString())
        return;
    QString s;
    if(!http->lasthost.pwd().isEmpty())
        s="&password="+http->lasthost.pwd();
    temp = temp + " \"" + "wa://" + "?gameid="+ h.id() + "&scheme=" + schememap[looki::currentchannel] + s+"\"";
    startprocess(temp);
}
void netcoupler::sendhostinfotoserverandhost(const QString &name,const QString &pwd, const QString &chan,int flag){
    if(!http)
        return;
    looki::currentchannel=chan;
    QString s=nick;
    if (!S_S.getstring("leplayername").isEmpty())
        s=S_S.getstring("leplayername");
    hoststruct h;
    QString hostcountrynumber=singleton<picturehandler>().map_hostcountrycode_to_number(singleton<picturehandler>().map_number_to_countrycode(flag));
    h.sethost(name,s,getmyhostip(),flag,"??","",pwd,chan,hostcountrynumber);
    http->sendhost(h);
    connect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
}
void netcoupler::getmywormnethost(hoststruct h){
    QString address=getmyhostip();
    disconnect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
    QString host = QString("wa://%1?gameid="+h.id()+"&scheme=%2").arg(address).arg(schememap[looki::currentchannel]);
    QString msg = QString(" is hosting a game: %1, %2").arg(h.name()).arg(host);
    if (S_S.getbool("chbsendhostinfotochan"))
        sendinfotochan(looki::currentchannel, msg);
    createhost(h);
}
QString netcoupler::getmyhostip(){
    QString address=myip;
    if(S_S.getbool("useacostumipforhosting"))
        address=S_S.getstring("costumipforhosting");
    return address;
}
void netcoupler::readprocess() {
    qWarning() << qobject_cast<QProcess*> (sender())->readAll();
}
QString netcoupler::getprocessstring() {
    QStringList sl = S_S.getstringlist("joinstrings");
    if (sl.isEmpty()){
        QMessageBox::warning(0, "", tr("No executables are given.\n"
                                       "you must choose a game executable,\n"
                                       "for example wa.exe, to join a game.\n"),
                             QMessageBox::Ok);
        return QString();
    }
#ifdef Q_WS_MAC
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
#endif
#ifdef Q_WS_X11
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
#endif
#ifdef Q_WS_WIN
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
    timer.disconnect();
    connect(&timer, SIGNAL(timeout()),this, SLOT(getwholist()));
    timer.start(S_S.getint("sbwhorepead"));
    singleton<sound_handler>().init();
}
void netcoupler::settingswindowemitfunktion() { //signals are protected?!
    emit sigsettingswindowchanged();
}
void netcoupler::refreshhostlist() {
    http->refreshhostlist();
}
void netcoupler::startprocess(const QString &s){    
    if(S_S.getbool("chbhidechannelwindowsongame")){
        foreach(window *w,qobjectwrapper<mainwindow>::ref().windowlist)        
            w->minimize();       
    }
    if(S_S.getbool("chbdisconnectongame"))
        qobjectwrapper<mainwindow>::ref().returntologintab();
    if(S_S.getbool("cbsetawaywhilegaming")){
        qobjectwrapper<awayhandler>::ref().setawaywhilegameing();
    }
    p->startDetached(s);
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
