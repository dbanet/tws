#include"leagueserverhandler.h"
#include"myDebug.h"
#include"singleton.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"settings.h"
#include"global_functions.h"
#include"quithandler.h"
#include"mainwindow.h"
#include"about.h"

#include<QMessageBox>
#include<QUrl>
#include<QTimer>
#include<QDesktopServices>
#include<QNetworkRequest>
#include<QNetworkReply>

leagueserverhandler::leagueserverhandler()
{   
    connecttimer.setSingleShot(true);
    connect(&connecttimer,SIGNAL(timeout()),this,SLOT(logintTimeOut()));

    informationrefreshtimer.setSingleShot(true);
    connect(&informationrefreshtimer,SIGNAL(timeout()),this,SLOT(refresh()));
    loggingstate=false;
}
leagueserverhandler::~leagueserverhandler(){}

void leagueserverhandler::login(QString n, QString p){    
    map.clear();
    leagueloginnick=n;
    reset();
    myconnect(n,p);    
}
void leagueserverhandler::myconnect(const QString n,const QString p){
    QUrl url=serveraddress+"testlogin.php?u="+n+"&p="+p;
    loginreply=qnam.get(QNetworkRequest(url));
    connect(loginreply, SIGNAL(finished()),this, SLOT(loginFinished()));      
    connecttimer.start(5000);
}
void leagueserverhandler::logintTimeOut(){
    loginFinished();
}
void leagueserverhandler::loginFinished(){            
    connecttimer.stop();
    QStringList sl=QString(loginreply->readAll()).split(" ",QString::SkipEmptyParts);
    if(sl.isEmpty() || loginreply->error() != QNetworkReply::NoError){
        QMessageBox::information(0,QObject::tr("Warning"),tr("The Server %1 doesnt seem to support the secure logging feature.\n errormessage: %2").arg(servicename).arg(loginreply->errorString()));
        qobjectwrapper<mainwindow>::ref().show();
        qobjectwrapper<mainwindow>::ref().raise();
        emit sigloginfailed();
        return;
    }
    loggingstate=true;              
    if(sl.takeFirst()=="0"){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Your %1 Account seems to be wrong, please try again.").arg(servicename));
        emit sigloginfailed();
        return;
    }   
    if(sl.isEmpty()){
        QMessageBox::warning(0,QObject::tr("Warning"),tr("The server responses with an invalid nickname: '%1'\nIts not possible to login, please contact the server admin.").arg(nick));
        qobjectwrapper<mainwindow>::ref().show();
        qobjectwrapper<mainwindow>::ref().raise();
        return;
    }
    nick=sl.takeFirst();
    QRegExp regexp;
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`){1,15}");
    if(!regexp.exactMatch(nick)){
        QMessageBox::warning(0,QObject::tr("Warning"),tr("The server responses with an invalid nickname: '%1'\nIts not possible to login, please contact the server admin.").arg(nick));
        qobjectwrapper<mainwindow>::ref().show();
        qobjectwrapper<mainwindow>::ref().raise();
        return;
    }
    startrefresh();
    emit sigloginsuccess();
    loginreply->deleteLater();
    singleton<balloon_handler>().connectedtoleagueserver(servicename);
    if(sl.size()<2)
        return;
    bool b=false;    
    int number=sl.takeFirst().toInt(&b);
    if(!b)
        return;
    if(S_S.getint("tusloginmessagenumber")==number)
        return;
    S_S.set("tusloginmessagenumber", number);
    QDesktopServices::openUrl(QUrl(sl.takeFirst()));    
}
void leagueserverhandler::startrefresh(){
    map.clear();
    informationrefreshtimer.start(0);    
}
void leagueserverhandler::stoprefresh(){
    informationrefreshtimer.stop();
    map.clear();
}
void leagueserverhandler::reset(){
    stoprefresh();
    connecttimer.stop();
}
//###################################
//###################################
//###################################
//###################################

void leagueserverhandler::refresh(){
    QUrl url;
    if(singleton<leagueserverhandler>().islogged())
        url=serveraddress+"userlist.php?update="+leagueloginnick+"&v="+about::version;
    else
        url=serveraddress+"userlist.php?v="+about::version;
    refreshreply=qnam.get(QNetworkRequest(url));
    connect(refreshreply, SIGNAL(finished()),this, SLOT(refreshFinished()));    
}
void leagueserverhandler::refreshFinished(){
    static QStringList sl;
    if(refreshreply->error()!=QNetworkReply::NoError){
        myDebug()<<tr("Unable to get the user information from")+" "+servicename;
        map.clear();
        refreshreply->deleteLater();
        informationrefreshtimer.start(qMax(100*1000,S_S.sbsecureloggingrepeatdelay));
        return;
    }
    map.clear();
    foreach(QString s,QString(refreshreply->readAll()).split("\n",QString::SkipEmptyParts)){
        sl=s.split(" ",QString::SkipEmptyParts);
        if(sl.isEmpty())
            break;
        QString nick=sl.takeFirst();
        QString s;
        if(sl.size()>5)
            s=QStringList(sl.mid(5)).join(" ");
        map[nick]=sl.mid(0,5) + QStringList()<<s;
        if(map[nick].size()>e_clan && map[nick][e_clan].trimmed()==".")
            map[nick][e_clan]="";
    }
    refreshreply->deleteLater();
    informationrefreshtimer.start(S_S.sbsecureloggingrepeatdelay);
}
void leagueserverhandler::setleague(const QString league, QString server){
    if(!server.endsWith("/"))
        server+="/";
    if(!server.startsWith("http://"))
        server="http://"+server;
    servicename=league;
    serveraddress=server;
}
bool leagueserverhandler::contains_key(QString key){
    return map.contains(key);
}
QString leagueserverhandler::service_name(){
    return servicename;
}
//###################################
//###################################
//###################################
//###################################
void leagueserverhandler::logout(){
    stoprefresh();
    QUrl url=serveraddress+"testlogin.php?logout="+leagueloginnick;
    logoutreply=qnam.get(QNetworkRequest(url));
    connect(logoutreply, SIGNAL(finished()),this, SLOT(logoutFinished()));        
}
void leagueserverhandler::logoutFinished(){       
    loggingstate=false;
    emit siglogout();
    logoutreply->deleteLater();
}
int leagueserverhandler::map_at_toInt(const QString key,const int i){
    static bool b;
    if(!map.contains(key))
        return -1;
    if(i>=map[key].size())
        return -1;
    int j=map[key].at(i).toInt(&b);
    if(!b)
        return -1;
    return j;
}
QString leagueserverhandler::map_at_toString(const QString key,const int i){
    if(!map.contains(key))
        return QString();
    if(i>=map[key].size())
        return QString();
    return map[key].at(i);
}
bool leagueserverhandler::islogged(){
    return loggingstate;
}
//###################################
//###################################
//###################################
//###################################
void leagueserverhandler::profile(QString s){
    QUrl url=serveraddress+"testlogin.php?profile="+s+";snooper";
    profilereply=qnam.get(QNetworkRequest(url));
    connect(profilereply, SIGNAL(finished()),this, SLOT(profileFinished()));    
}
void leagueserverhandler::profileFinished(){
    if(profilereply->error() != QNetworkReply::NoError){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
        return;
    }
    emit sigprofile(profilereply->readAll());
}
