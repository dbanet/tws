#include"quithandler.h"
#include"snpsettings.h"
#include"clantowebpagemapper.h"
#include"charformatsettings.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"mainwindow.h"
#include"global_functions.h"
#include"netcoupler.h"
#include"leagueserverhandler.h"

#include<QApplication>
quithandler::quithandler(){
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(beforequit()));
}
quithandler::~quithandler(){}

void quithandler::inducequit(){
    beforequit();
    int state=singleton<netcoupler>().ircstate();
    if(state!=QAbstractSocket::ConnectedState){
        quit();
        return;
    }
    singleton<netcoupler>().sendquit();
    connect(&singleton<netcoupler>(),SIGNAL(sigdisconnected()),this,SLOT(quit()));        
}
void quithandler::beforequit(){    
    singleton<mainwindow>().quit();
    singleton<snpsettings>().safeonquit();
    singleton<snpsettings>().safe();
    singleton<clantowebpagemapper>().safe();
    bool b = singleton<settingswindow>().from_map("cbsafequerys").value<bool> ();
    if (b) {
        safeusergarbage();
        safequerylist();
    }
    singleton<balloon_handler>().hide_tray();
}
void quithandler::quit(){
    if(singleton<leagueserverhandler>().islogged()){
        connect(&singleton<leagueserverhandler>(),SIGNAL(siglogout()),this,SLOT(quit()));
        singleton<leagueserverhandler>().logout();
        return;
    }
    this->disconnect();
    QTimer::singleShot(0,qApp,SLOT(quit()));
    return;
}