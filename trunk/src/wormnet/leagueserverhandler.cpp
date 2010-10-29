#include"leagueserverhandler.h"
#include"myDebug.h"
#include"singleton.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"snpsettings.h"
#include"global_functions.h"
#include"quithandler.h"
#include"mainwindow.h"
#include"mynetworkreply.h"

#include<QMessageBox>
#include<QUrl>
#include<QTimer>
#include<QDesktopServices>
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
    connect(loginreply, SIGNAL(readyRead()),this, SLOT(loginReadyRead()));    
    connecttimer.start(5000);
}
void leagueserverhandler::logintTimeOut(){        
    qobject_cast<mynetworkreply*>(loginreply)->setError(QNetworkReply::TimeoutError);       //bad design?
}
void leagueserverhandler::loginFinished(){        
    connecttimer.stop();
    QStringList sl=connectresponse.split(" ",QString::SkipEmptyParts);
    if(sl.isEmpty() || loginreply->error() != QNetworkReply::NoError){
        QMessageBox::information(0,QObject::tr("Warning"),tr("The Server %1 doesnt seem to support the secure logging feature.\n errormessage: %2").arg(servicename).arg(loginreply->errorString()));
        qobjectwrapper<mainwindow>::ref().show();
        qobjectwrapper<mainwindow>::ref().raise();
        emit sigloginfailed();
        return;
    }
    loggingstate=true;        
    connectresponse.clear();        
    if(sl.takeFirst()=="0"){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Your %1 Account seems to be wrong, please try again.").arg(servicename));
        emit sigloginfailed();
        return;
    }   
    nick=sl.takeFirst();
    if(nick.contains(" "))
        QMessageBox::critical(0,"exception","nick in leagueserverhandler::finished contains whitespaces!!");
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
    if(singleton<snpsettings>().map["tusloginmessagenumber"].toInt()==number)
        return;
    singleton<snpsettings>().map["tusloginmessagenumber"]=number;
    QDesktopServices::openUrl(QUrl(sl.takeFirst()));    
}
void leagueserverhandler::startrefresh(){
    informationrefreshtimer.start(0);
}
void leagueserverhandler::stoprefresh(){
    informationrefreshtimer.stop();
}
void leagueserverhandler::reset(){
    stoprefresh();
    connecttimer.stop();
}
void leagueserverhandler::loginReadyRead(){
    connectresponse.append(loginreply->readAll());
}
//###################################
//###################################
//###################################
//###################################

void leagueserverhandler::refresh(){
    QUrl url;
    if(singleton<leagueserverhandler>().islogged())
        url=serveraddress+"userlist.php?update="+leagueloginnick;
    else
        url=serveraddress+"userlist.php";
    refreshreply=qnam.get(QNetworkRequest(url));
    connect(refreshreply, SIGNAL(finished()),this, SLOT(refreshFinished()));
    connect(refreshreply, SIGNAL(readyRead()),this, SLOT(refreshReadyRead()));    
}
void leagueserverhandler::refreshFinished(){
    static QStringList sl;
    if(refreshreply->error()!=QNetworkReply::NoError){
        myDebug()<<tr("Unable to get the user information from")+" "+servicename;
        return;
    }
    map.clear();
    foreach(QString s,refreshresponse.split("\n",QString::SkipEmptyParts)){
        sl=s.split(" ",QString::SkipEmptyParts);
        if(sl.isEmpty())
            break;
        map[sl.takeFirst()]=sl;
    }
    refreshresponse.clear();
    refreshreply->deleteLater();
    informationrefreshtimer.start(singleton<settingswindow>().from_map("sbsecureloggingrepeatdelay").toInt());
}

void leagueserverhandler::refreshReadyRead(){
    refreshresponse.append(refreshreply->readAll());
}
void leagueserverhandler::setleague(const QString league, const QString server){
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
    connect(logoutreply, SIGNAL(readyRead()),this, SLOT(logoutReadyRead()));    
}
void leagueserverhandler::logoutFinished(){
    logoutreply->deleteLater();    
    loggingstate=false;
    emit siglogout();
}
void leagueserverhandler::logoutReadyRead(){

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
void leagueserverhandler::profile(QString n){
    QUrl url=serveraddress+"testlogin.php?profile="+n+";snooper";
    profilereply=qnam.get(QNetworkRequest(url));
    connect(profilereply, SIGNAL(finished()),this, SLOT(profileFinished()));
    connect(profilereply, SIGNAL(readyRead()),this, SLOT(profileReadyRead()));    
}
void leagueserverhandler::profileFinished(){
    if(profilereply->error() != QNetworkReply::NoError){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
        return;
    }
    emit sigprofile(profileresponse);
    profileresponse.clear();
}
void leagueserverhandler::profileReadyRead(){
    profileresponse.append(profilereply->readAll());
}
