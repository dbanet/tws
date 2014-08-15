#include "awayhandler.h"
#include "global_functions.h"
#include "global_macros.h"
#include "settings.h"
#include "settingswindow.h"
#include "netcoupler.h"
#include "singleton.h"
#include "balloon_handler.h"
#include "awaybox.h"
#include "usermessage.h"
#include "netcoupler.h"

#ifdef Q_OS_WIN32
#undef UNICODE
#undef _UNICODE
#include <windows.h>
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
#ifdef Q_OS_WIN32
    lookingForGameTimer.start(1000);
#endif
}
void awayhandler::setaway(const QString &s) {
    singleton<balloonHandler>().setAwayTrayIcon();
    awaymessage = s;
    isaway = 1;
}
void awayhandler::gamefinished() {
    if (S_S.getbool("cbsetawaywhilegaming")) {
        if (wasaway) {
            isaway = 1;
            wasaway=0;
            awaymessage = S_S.getStringList("awaymessage").last();
            emit sigawaystringchanged();
            return;
        }
        singleton<balloonHandler>().setNormalTrayIcon();
        isaway = 0;
        wasaway = 0;
        emit sigawaystringchanged();
        sendBack();
    }    
}
void awayhandler::sendBack(){
    foreach(QString user,rememberwhogotaway.keys()) {
        if (S_S.getbool("chbbacktonormals") && !containsCI(S_S.getStringList("ignorelist"), user)) {
            singleton<netcoupler>().sendusermessage(usermessage(S_S.getString("lebackmessage"), e_NOTICE, user));
            int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(user));
            if (i != -1 && singleton<netcoupler>().users.users[i].clan != "Username")
                singleton<netcoupler>().sendusermessage(usermessage("back", e_CTCP, user));
        } else if (S_S.getbool("chbbacktobuddys")) {
            if (containsCI(S_S.getStringList("buddylist"), user)){
                singleton<netcoupler>().sendusermessage(usermessage(S_S.getString("lebackmessage"), e_NOTICE, user));
                int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(user));
                if (i != -1 && singleton<netcoupler>().users.users[i].clan != "Username")
                    singleton<netcoupler>().sendusermessage(usermessage("back", e_CTCP, user));
            }
        }
    }
    rememberwhogotaway.clear();
}
void awayhandler::setawaywhilegameing() {
#ifdef Q_OS_WIN32
    startLookingForGame();
    singleton<balloonHandler>().setAwayTrayIcon();
    if (S_S.getbool("cbsetawaywhilegaming")) {
        if (isaway)
            wasaway = 1;
        else
            wasaway = 0;
        isaway = 1;
        awaymessage = S_S.getString("leawaystring");
        emit sigawaystringchanged();
    }
#endif
}
void awayhandler::gameTimerTimeout(){
#ifdef Q_OS_WIN32
    if(isProcessRunning(waProcessId))
        return;    
    lookingForGameTimer.stop();
    emit siggameended();
#endif
}
void awayhandler::back(){
    isaway = 0;
    wasaway = 0;
    singleton<balloonHandler>().setNormalTrayIcon();
    sendBack();
}
bool awayhandler::away(){
    return isaway;
}
QString awayhandler::message(){
    return awaymessage;
}
QString trim(QString msg){
    if (msg.startsWith(">!"))
        msg.remove(0, 2);
    else if (msg.startsWith("/"))
        msg.remove(0, 1);
    else if (msg.startsWith(">>>"))
        msg.remove(0, 3);
    else if (msg.startsWith(">>"))
        msg.remove(0, 2);
    else if (msg.startsWith(">"))
        msg.remove(0, 1);
    return msg;
}
void awayhandler::sendaway(const QString &user){    
    if(!away())
        return;
    if (rememberwhogotaway[user] != message()) {
        singleton<netcoupler>().sendusermessage(usermessage::create(message(), user));
        int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(user));
        if (i != -1 && singleton<netcoupler>().users.users[i].clan != "Username")
            singleton<netcoupler>().sendusermessage(usermessage("away " + trim(message()), e_CTCP, user));
        qobjectwrapper<awayhandler>::ref().rememberwhogotaway[user] = message();
    }    
}
