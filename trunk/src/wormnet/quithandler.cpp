#include"quithandler.h"
#include"snpsettings.h"
#include"clantowebpagemapper.h"
#include"charformatsettings.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"mainwindow.h"
#include"global_functions.h"

#include<QApplication>
quithandler::quithandler(){}
quithandler::~quithandler(){}
void quithandler::inducequit(){
    beforequit();
    quit();
}
void quithandler::beforequit(){
    singleton<mainwindow>().onquit();
    singleton<snpsettings>().safeonquit();
    singleton<snpsettings>().safe();
    singleton<clantowebpagemapper>().safe();
    bool b = singleton<settingswindow>().from_map("cbsafequerys").value<bool> (); //no need for usesettingswindow here
    if (b) {
        safeusergarbage();
        safequerylist();
    }
    singleton<balloon_handler>().hide_tray();
}
void quithandler::quit(){
    qApp->quit();
}
void quithandler::finished(CLASSES c){
    static QList<CLASSES> l;
    l<<c;
}
