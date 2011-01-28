#include<QFile>
#include<QClipboard>
#include<QTime>
#include<QMessageBox>
#include"netcoupler.h"
#include"ircnet.h"
#include"snoppanet.h"
#include"inihandlerclass.h"
#include"snpsettings.h"
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

extern volumeslider *volume;
extern inihandlerclass inihandler;
namespace looki {
    QString currentchannel;
}
netcoupler::netcoupler() {
    connectstate=e_stoped;
    p=new QProcess(this);
    connect(volume, SIGNAL(valueChanged (int)),&singleton<sound_handler>(), SLOT(volumechange(int)));

    connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloon_handler>(), SLOT(buddyarrived()));
    connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloon_handler>(), SLOT(buddyleft()));
    connect(this,SIGNAL(sigconnected()),&singleton<balloon_handler>(),SLOT(connected()));
    connect(this,SIGNAL(sigdisconnected()),&singleton<balloon_handler>(),SLOT(disconnected()));    
    p->setProcessChannelMode(QProcess::MergedChannels);    
    connect(p, SIGNAL(readyRead()),this, SLOT(readprocess()));    
    usesettingswindow("cbsetawaywhilegaming");
    usesettingswindow("leawaystring");

    mutedusers = S_S.map["mutedusers"].toStringList();
    connect(this, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(this, SIGNAL(sigsettingswindowchanged()),&users, SLOT(usesettingswindow()));
}
void netcoupler::start(QString nick){
    connectstate=e_started;
    this->nick=nick;
    irc = new ircnet(nick, this);    
    connect(irc, SIGNAL(sigusergarbage(const QString&,const QString&)),this, SIGNAL(sigusergarbage(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagejoin(const QString&,const QString&)),this, SIGNAL(sigusergarbagejoin(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagepart(const QString&,const QString&)),this, SIGNAL(sigusergarbagepart(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagequit(const QString&,const QString&)),this, SIGNAL(sigusergarbagequit(const QString&,const QString&)));
    connect(irc,SIGNAL(sigconnected()),this,SLOT(ircconnected()));
    connect(irc,SIGNAL(sigdisconnected()),this,SLOT(ircdisconnected()));
    http = new snoppanet(this);
    connect(http, SIGNAL(sigchannelscheme(QString,QString)),this, SLOT(getscheme(QString,QString)));
    connect(irc, SIGNAL(siggetlist(QStringList)),this, SLOT(getchannellist(QStringList)));
    connect(irc, SIGNAL(sigmsg(const QString&,const QString&,const QString&)),this, SLOT(getmsg(const QString&,const QString&,const QString&)));
    connect(irc, SIGNAL(signotice(const QString&,const QString&,const QString&)),this, SLOT(getnotice(const QString&,const QString&,const QString&)));
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
        usesettingswindow("sbwhorepead");
    }
    qobjectwrapper<awayhandler>::ref().isaway = 0;
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
    irc->joinchannel(s);
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
    foreach(QString s,sl) {
        channellist << s.split(" ").first();
    }
    foreach(QString s, channellist) {
        if(!users.usermap_channellist_helper.contains(s,Qt::CaseInsensitive))
            users.usermap_channellist_helper.push_back(s);
    }
    emit siggotchanellist(sl);
}
void netcoupler::getircip(QString s) {
    irc->setip(s);
    irc->start();
    usesettingswindow("sbwhorepead");
}
void netcoupler::gethostlist(QList<hoststruct> l, QString s) {
    refreshlist();
    hosts.sethoststruct(l, s);
}
void netcoupler::getwholist() {
    users.setuserstruct(irc->wholist, irc->joinlist);
    irc->who();
}
void netcoupler::getmsg(const QString &user, const QString &receiver,
                        const QString &msg) {
    if (containsCI(channellist, receiver)) {
        emit siggotmsg(user, receiver, msg);
    } else {
        bool b = compareCI(receiver, nick);
        Q_UNUSED(b);
        Q_ASSERT_X(b==1,"getmsg netcoupler",qPrintable(receiver));
        emit siggotprivmsg(user, receiver, msg);
        QApplication::processEvents();
    }
}
void netcoupler::getnotice(const QString &user, const QString &receiver,
                           const QString &msg) {
    emit siggotnotice(user, receiver, msg);
}
void netcoupler::sendmessage(const QString &receiver, const QString &msg) {
    irc->sendmessage(msg, receiver);
}
void netcoupler::senduncheckedmessage(const QString &user, const QString &msg) {
    if (msg.startsWith(">!")) {
        sendrawcommand(QString("PRIVMSG ") + user + " :\001"
                       + QString(msg).remove(0, 2).simplified() + "\001");
    } else if (msg.startsWith("/")) {
        sendrawcommand(QString(msg).remove(0, 1).simplified());
    } else if (msg.startsWith(">>>")) {
        sendrawcommand(QString("NOTICE ") + user + " :\001ACTION "
                       + QString(msg).remove(0, 3).simplified() + " \001");
    } else if (msg.startsWith(">>")) {
        sendnotice(user, QString(msg).remove(0, 2).simplified());
    } else if (msg.startsWith(">")) {
        sendrawcommand(QString("PRIVMSG ") + user + " :\001ACTION " + QString(
                msg).remove(0, 1).simplified() + " \001\n");
    } else {
        sendmessage(user, msg);
    }
}
void netcoupler::sendnotice(const QString &receiver, const QString &msg) {
    irc->sendnotice(msg, receiver);
}
void netcoupler::sendrawcommand(const QString &raw) {
    irc->sendrawcommand(raw);
}
void netcoupler::refreshlist() {
    irc->refreshlist();
}
void netcoupler::sendquit() {
    irc->quit();
}
void netcoupler::getscheme(QString chan, QString scheme) {
    schememap[chan] = scheme;
}
void netcoupler::refreshwho() {

}
void netcoupler::sendmessagetoallbuddys(const QString &msg) {
    foreach(QString s,S_S.map["buddylist"].value<QStringList>()) {
        if (users.users.contains(userstruct(userstruct::whoami(s)))) {
            QString nick = users.users[users.users.indexOf(userstruct(
                    userstruct::whoami(s)))].nick;
            this->sendnotice(nick, msg);
        }
    }
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
    temp = temp + " \"" + gamelink + "\"";
    startprocess(temp);
}
void netcoupler::joingame(const QString &hostinfo, const QString &channel,
                          const QString &gamename) {
    looki::currentchannel = channel;
    QString temp = getprocessstring();
    temp = temp + hostinfo;
    startprocess(temp);
    if (singleton<settingswindow>().from_map("chbactionwhenjoining").toBool())
        sendinfotochan(channel, " is joining a game: " + gamename);
}
void netcoupler::createhost(hoststruct h) {
    if (users.users.indexOf(userstruct::whoami(nick)) == -1)
        return;
    QString temp = getprocessstring();
    if (temp == "")
        return;
    QString s;
    if(!http->lasthost.pwd().isEmpty())
        s="&password="+http->lasthost.pwd();
    temp = temp + " \"" + "wa://" + "?gameid="+ h.id() + "&scheme="
           + schememap[looki::currentchannel] + s+"\"";
    startprocess(temp);
}
void netcoupler::sendhostinfotoserverandhost(const QString &name,const QString &pwd, const QString &chan,int flag){
    looki::currentchannel=chan;
    QString nick=this->nick;
    if (!S_S.map["leplayername"].toString().isEmpty())
        nick=S_S.map["leplayername"].toString();
    hoststruct h;
    QString hostcountrynumber=singleton<picturehandler>().map_hostcountrycode_to_number(singleton<picturehandler>().map_number_to_countrycode(flag));
    h.sethost(name,nick,getmyhostip(),flag,"??","",pwd,chan,hostcountrynumber);
    http->sendhost(h);
    connect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
}
void netcoupler::getmywormnethost(hoststruct h){
    QString address=getmyhostip();
    disconnect(http,SIGNAL(sighoststarts(hoststruct)),this,SLOT(getmywormnethost(hoststruct)));
    QString host = QString("wa://%1?gameid="+h.id()+"&scheme=%2").arg(address).arg(schememap[looki::currentchannel]);
    QString msg = QString(" is hosting a game: %1, %2").arg(h.name()).arg(host);
    if (S_S.map["chbsendhostinfotochan"].toBool())
        sendinfotochan(looki::currentchannel, msg);
    createhost(h);
}
QString netcoupler::getmyhostip(){
    QString address=myip;
    if(S_S.map["useacostumipforhosting"].value<bool> ())
        address=S_S.map["costumipforhosting"].value<QString>();
    return address;
}
void netcoupler::readprocess() {
    qWarning() << qobject_cast<QProcess*> (sender())->readAll();
}
QString netcoupler::getprocessstring() {
    QStringList sl = S_S.map["joinstrings"].value<QStringList> ();
    if (sl.isEmpty())
        QMessageBox::warning(0, "", tr("No executables are given.\n"
                                       "you must choose a game executable,\n"
                                       "for example wa.exe, to join a game.\n"),
                             QMessageBox::Ok);
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
    QString command = QString("PRIVMSG %1 :\001ACTION %2 \001").arg(chan).arg(
            msg);
    sendrawcommand(command);
    command = QString("\001ACTION %1 \001").arg(msg);
    getmsg(nick, chan, command);
}
void netcoupler::usesettingswindow(const QString &s) {
    if (s == "sbwhorepead" || s == "") {
        int i = singleton<settingswindow>().from_map("sbwhorepead").value<int> ();
        timer.disconnect();
        connect(&timer, SIGNAL(timeout()),this, SLOT(getwholist()));        
        timer.start(i);
    }
    singleton<sound_handler>().init();
}
void netcoupler::settingswindowemitfunktion() { //signals are protected?!
    emit sigsettingswindowchanged();
}
void netcoupler::refreshhostlist() {
    http->refreshhostlist();
}
void netcoupler::startprocess(const QString &s){    
    if(singleton<settingswindow>().from_map("chbhidechannelwindowsongame").toBool()){
        foreach(window *w,qobjectwrapper<mainwindow>::ref().windowlist)
        {
            w->minimize();
        }
    }
    if(singleton<settingswindow>().from_map("chbdisconnectongame").toBool())
        qobjectwrapper<mainwindow>::ref().returntologintab();
    if(singleton<settingswindow>().from_map("cbsetawaywhilegaming").toBool()){
        qobjectwrapper<awayhandler>::ref().startLookingForGame();
        qobjectwrapper<awayhandler>::ref().setawaywhilegameing();
    }
    p->startDetached(s);
}
int netcoupler::ircstate(){
    return irc->state();
}
void netcoupler::loopTimerTimeout(){
    safeusergarbage();
}
