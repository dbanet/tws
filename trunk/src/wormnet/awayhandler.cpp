#include"awayhandler.h"
#include"global_functions.h"
#include"global_macros.h"
#include"snpsettings.h"
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
            return;
        }
        singleton<balloon_handler>().set_normal_tray_icon();
        isaway = 0;
        wasaway = 0;
        if (!S_S.map["awaymessage"].toStringList().isEmpty())
            awaymessage = S_S.map["awaymessage"].value<QStringList>().last();
        emit sigawaystringchanged();
        foreach(QString s,rememberwhogotaway.keys()) {
            if (singleton<settingswindow>().from_map("chbbacktonormals").toBool()
                && !S_S.map["ignorelist"].toStringList().contains(s, Qt::CaseInsensitive)) {
                singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
                singleton<netcoupler>().sendrawcommand("PRIVMSG " + s + " :\001back\001");
            } else if (singleton<settingswindow>().from_map("chbbacktobuddys").toBool()) {
                if (S_S.map["buddylist"].toStringList().contains(s,Qt::CaseInsensitive))
                    singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
                singleton<netcoupler>().sendrawcommand("PRIVMSG " + s + " :\001back\001");
            }
        }
    }
    rememberwhogotaway.clear();
}
void awayhandler::setawaywhilegameing() {
#ifdef Q_WS_WIN
    singleton<balloon_handler>().set_away_tray_icon();
    if (singleton<settingswindow>().from_map("cbsetawaywhilegaming").toBool()) {
        if (!isaway){
            isaway = 1;
            wasaway=0;
        }
        else
            wasaway = 1;
        awaymessage = singleton<settingswindow>().from_map("leawaystring").toString();
        oldawaystring = awaymessage;
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
    foreach(QString s,rememberwhogotaway.keys()) {
        if (singleton<netcoupler>().users.users.indexOf(userstruct::whoami(s))!= -1 && !compareCI(
                    singleton<netcoupler>().users.users[singleton<netcoupler>().users.users.indexOf(
                            userstruct::whoami(s))].clan,"username"))
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + s + " :\001back\001");
        if (singleton<settingswindow>().from_map("chbbacktonormals").toBool()
            && !S_S.map["ignorelist"].toStringList().contains(s, Qt::CaseInsensitive))
            singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
        else if (singleton<settingswindow>().from_map("chbbacktobuddys").toBool()) {
            if (S_S.map["buddylist"].toStringList().contains(s, Qt::CaseInsensitive))
                singleton<netcoupler>().sendnotice(s,singleton<settingswindow>().from_map("lebackmessage").toString());
        }
    }
    rememberwhogotaway.clear();
}
