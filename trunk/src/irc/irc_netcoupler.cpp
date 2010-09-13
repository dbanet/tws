#include<QFile>
#include<QClipboard>
#include<QTime>
#include<QMessageBox>
#include"irc_netcoupler.h"
#include"irc_ircnet.h"
#include"src/wormnet/inihandlerclass.h"
#include"src/wormnet/snpsettings.h"
#include"src/wormnet/window.h"
#include"src/wormnet/joinprvgame.h"
#include"src/wormnet/settingswindow.h"
#include"src/wormnet/volumeslider.h"
#include"src/wormnet/mainwindow.h"
#include"src/wormnet/playername.h"
#include"src/wormnet/sound_handler.h"
#include"src/wormnet/sound_handler.h"
#include"src/wormnet/global_functions.h"
#include"src/wormnet/global_functions.h"
#include"src/wormnet/balloon_handler.h"
extern inihandlerclass inihandler;
irc_netcoupler::irc_netcoupler(QString s, QObject *parent,QString channel) :
        QObject(parent), nick(s),users(this) {

    irc = new irc_ircnet(nick, this,channel);
    connect(irc,SIGNAL(sigreconnect()),this,SIGNAL(sigreconnect()));
    connect(irc, SIGNAL(sigusergarbage(const QString&,const QString&)),this, SIGNAL(sigusergarbage(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagejoin(const QString&,const QString&)),this, SIGNAL(sigusergarbagejoin(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagepart(const QString&,const QString&)),this, SIGNAL(sigusergarbagepart(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagequit(const QString&,const QString&)),this, SIGNAL(sigusergarbagequit(const QString&,const QString&)));
    connect(irc,SIGNAL(sigconnected()),this,SLOT(ircconnected()));
    connect(irc,SIGNAL(sigdisconnected()),this,SLOT(ircdiconnected()));    

    connect(irc, SIGNAL(sigmsg(const QString&,const QString&,const QString&)),this, SLOT(getmsg(const QString&,const QString&,const QString&)));
    connect(irc, SIGNAL(signotice(const QString&,const QString&,const QString&)),this, SLOT(getnotice(const QString&,const QString&,const QString&)));
    connect(irc, SIGNAL(siggotidletime(const QString&, int)),this, SIGNAL(siggotidletime(const QString&, int)));
    connect(irc, SIGNAL(signosuchnick(const QString&)),this, SIGNAL(signosuchnick(const QString&)));
    connect(irc,SIGNAL(sigFirstServerMessage()),this,SLOT(getFirstServerMessage()));

    connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloon_handler>(), SLOT(buddyarrived()));
    connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloon_handler>(), SLOT(buddyleft()));       
    ircip = "93.189.105.234";
    irc->setip(ircip);
    irc->start();
    usesettingswindow("sbwhorepead");
    connect(qApp, SIGNAL(aboutToQuit()),this, SLOT(sendquit()));
}
irc_netcoupler::~irc_netcoupler() {
    userstruct::addressischecked=0;
    irc->quit();
    irc->deleteLater();
}
void irc_netcoupler::joinchannel(const QString &s) {    
    if (!irc->listofjoinedchannels.contains(s)){
        irc->joinchannel(s);
        irc->listofjoinedchannels << s;
    }
}
void irc_netcoupler::partchannel(const QString &s) {
    irc->partchannel(s);
    irc->listofjoinedchannels.removeOne(s);
}
void irc_netcoupler::getmsg(const QString &user, const QString &receiver,
                            const QString &msg) {        
        emit siggotmsg(user, receiver, msg);   
}
void irc_netcoupler::getnotice(const QString &user, const QString &receiver,
                               const QString &msg) {
    emit siggotnotice(user, receiver, msg);
}
void irc_netcoupler::sendmessage(const QString &receiver, const QString &msg) {
    irc->sendmessage(msg, receiver);
}
void irc_netcoupler::senduncheckedmessage(const QString &user, const QString &msg) {
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
void irc_netcoupler::sendnotice(const QString &receiver, const QString &msg) {
    irc->sendnotice(msg, receiver);
}
void irc_netcoupler::sendrawcommand(const QString &raw) {
    irc->sendrawcommand(raw);
}
void irc_netcoupler::sendquit() {
    irc->quit();
}
void irc_netcoupler::ircconnected(){
    emit sigconnected();
}
void irc_netcoupler::ircdiconnected(){
    emit sigdisconnected();
}
void irc_netcoupler::sendinfotochan(const QString &chan, const QString &msg) {
    QString command = QString("PRIVMSG %1 :\001ACTION %2 \001").arg(chan).arg(
            msg);
    sendrawcommand(command);
    command = QString("\001ACTION %1 \001").arg(msg);
    getmsg(nick, chan, command);
}
void irc_netcoupler::usesettingswindow(const QString &s) {
    if (s == "sbwhorepead" || s == "") {
        int i = singleton<settingswindow>().from_map("sbwhorepead").value<int> ();
        timer.disconnect();
        connect(&timer, SIGNAL(timeout()),this, SLOT(getjoinlist()));
        timer.start(i);
    }
    singleton<sound_handler>().init();
}
void irc_netcoupler::settingswindowemitfunktion() { //signals are protected?!
    emit sigsettingswindowchanged();
}
void irc_netcoupler::getFirstServerMessage(){

}
QList<userstruct> &irc_netcoupler::getUsersInChannel(QString channel){
    return irc->joinlist[channel];
}
bool irc_netcoupler::joinListContains(QString user){
    foreach(QList<userstruct> l,irc->joinlist){
        if(l.contains(userstruct::whoami(user)))
            return true;
    }
    return false;
}
int irc_netcoupler::countUsers(){
    int i=0;   
    foreach(QList<userstruct> l,irc->joinlist){
        i+=l.count();
    }
    return i;
}
int irc_netcoupler::countUsers(QString user){
    int i=0;
    foreach(QList<userstruct> l,irc->joinlist){
        i+=l.count(userstruct::whoami(user));
    }
    return i;
}
int irc_netcoupler::countUsers(userstruct user){
    int i=0;
    foreach(QList<userstruct> l,irc->joinlist){
        i+=l.count(user);
    }
    return i;
}

void irc_netcoupler::getjoinlist(){
    users.setuserstruct(irc->joinlist);
}
userstruct irc_netcoupler::getUser(QString user){
    foreach(QString s, irc->joinlist.keys()){
        QList<userstruct> &l=getUsersInChannel(s);
        if(l.contains(userstruct::whoami(user))){
            return l[l.indexOf(userstruct::whoami(user))];
        }
    }
    return userstruct();
}
