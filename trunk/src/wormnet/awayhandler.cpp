#include"awayhandler.h"
#include"global_functions.h"
#include"global_macros.h"
#include"settings.h"
#include"settingswindow.h"
#include"netcoupler.h"
#include"singleton.h"
#include"balloon_handler.h"
#include"awaybox.h"

#ifdef Q_WS_WIN
#undef UNICODE
#undef _UNICODE
#include<windows.h>
#endif

awayhandler::awayhandler(QObject *parent) :
        QObject(parent)
{
    connect(&lookingForGameTimer,SIGNAL(timeout()),this,SLOT(gameTimerTimeout()));
    connect(this,SIGNAL(siggameended()),this,SLOT(gamefinished()));
    wasaway=0;
    isaway=0;
}
awayhandler::~awayhandler(){}

void awayhandler::startLookingForGame(){
    lookingForGameTimer.start(5*1000);
}
void awayhandler::setaway(const QString &s) {
    singleton<balloon_handler>().set_away_tray_icon();
    awaymessage = s;
    isaway = 1;
}
void awayhandler::gamefinished() {
    if (singleton<settingswindow>().from_map("cbsetawaywhilegaming").toBool()) {
        if (wasaway) {
            isaway = 1;
            wasaway=0;
            awaymessage = S_S.getstringlist("awaymessage").last();
            emit sigawaystringchanged();
            return;
        }
        singleton<balloon_handler>().set_normal_tray_icon();
        isaway = 0;
        wasaway = 0;
        emit sigawaystringchanged();
        sendBack();
    }    
}
void awayhandler::sendBack(){
    foreach(QString s,rememberwhogotaway.keys()) {
        if (singleton<settingswindow>().from_map("chbbacktonormals").toBool()
            && !containsCI(S_S.getstringlist("ignorelist"), s)) {
            singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + s + " :\001back\001");
        } else if (singleton<settingswindow>().from_map("chbbacktobuddys").toBool()) {
            if (containsCI(S_S.getstringlist("buddylist"), s))
                singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + s + " :\001back\001");
        }
    }
    rememberwhogotaway.clear();
}
void awayhandler::setawaywhilegameing() {
#ifdef Q_WS_WIN
    singleton<balloon_handler>().set_away_tray_icon();
    if (singleton<settingswindow>().from_map("cbsetawaywhilegaming").toBool()) {        
        if (isaway)
            wasaway = 1;
        else
            wasaway = 0;
        isaway = 1;
        awaymessage = singleton<settingswindow>().from_map("leawaystring").toString();
        emit sigawaystringchanged();
    }
#endif
}
void awayhandler::gameTimerTimeout(){
#ifdef Q_WS_WIN    
    HWND h = FindWindow(NULL,"Worms Armageddon");
    if(h)
        return;    
    lookingForGameTimer.stop();
    emit siggameended();
#endif
}
void awayhandler::back(){
    isaway = 0;
    wasaway = 0;
    singleton<balloon_handler>().set_normal_tray_icon();
    sendBack();
}
bool awayhandler::away(){
    return isaway;
}
QString awayhandler::message(){
    return awaymessage;
}
void awayhandler::sendaway(const QString &user){
    if (rememberwhogotaway[user] != qobjectwrapper<awayhandler>::ref().message()) {
        singleton<netcoupler>().sendmessage(user, message());
        int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(user));
        if (i != -1 && singleton<netcoupler>().users.users[i].clan != "Username")
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + user + " :\001away " + message() + "\001");
    }
    qobjectwrapper<awayhandler>::ref().rememberwhogotaway[user] = qobjectwrapper<awayhandler>::ref().message();
}
