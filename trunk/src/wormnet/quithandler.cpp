#include"quithandler.h"
#include"settings.h"
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

void quithandler::inducequit(QString s){
    if(s==QString())
        s=S_S.getstring("information");
    S_S.transaction();
    beforequit();    
    S_S.commit();
    int state=singleton<netcoupler>().ircstate();        
    if(state==QAbstractSocket::ConnectedState) {
        connect(&singleton<netcoupler>(),SIGNAL(sigdisconnected()),this,SLOT(quit()));
        singleton<netcoupler>().sendquit(s);
    }
    else
        quit();
}
void quithandler::beforequit(){    
    qobjectwrapper<mainwindow>::ref().quit();            
    safeusergarbage();
    safequerylist();
    singleton<balloon_handler>().hide_tray();
}
void quithandler::quit(){
    if(singleton<leagueserverhandler>().islogged()){
        connect(&singleton<leagueserverhandler>(),SIGNAL(siglogout()),this,SLOT(quit()));
        singleton<leagueserverhandler>().logout();
        return;
    }    
    disconnect(&singleton<leagueserverhandler>(),SIGNAL(siglogout()),this,SLOT(quit()));
    disconnect(&singleton<netcoupler>(),SIGNAL(sigdisconnected()),this,SLOT(quit()));
    qApp->quit();
    return;
}
