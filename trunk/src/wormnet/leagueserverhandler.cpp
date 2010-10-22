#include "leagueserverhandler.h"
#include"myDebug.h"
#include"singleton.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"snpsettings.h"
#include"global_functions.h"
#include"quithandler.h"
#include<QNetworkReply>
#include<QMessageBox>
#include<QUrl>
#include<QTimer>
#include<QDesktopServices>
leagueserverhandler::leagueserverhandler()
{
    connecttimer=new QTimer;
    connecttimer->setSingleShot(true);
    connect(connecttimer,SIGNAL(timeout()),this,SLOT(logintTimeOut()));

    informationrefreshtimer=new QTimer;
    informationrefreshtimer->setSingleShot(true);
    connect(informationrefreshtimer,SIGNAL(timeout()),this,SLOT(refresh()));
    loggingstate=false;
}
leagueserverhandler::~leagueserverhandler(){}

void leagueserverhandler::login(QString n, QString p){
    leagueloginnick=n;
    informationrefreshtimer->stop();
    connecttimer->stop();
    myconnect(n,p);    
}
void leagueserverhandler::myconnect(const QString n,const QString p){    
    QUrl url=serveraddress+"testlogin.php?u="+n+"&p="+p;
    loginreply=qnam.get(QNetworkRequest(url));
    connect(loginreply, SIGNAL(finished()),this, SLOT(loginFinished()));
    connect(loginreply, SIGNAL(readyRead()),this, SLOT(loginReadyRead()));
    connect(loginreply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(loginerror(QNetworkReply::NetworkError)));
    connecttimer->start(5000);
}
void leagueserverhandler::logintTimeOut(){
    QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
    emit sigloginfailed();
    loginreply->deleteLater();
}
void leagueserverhandler::loginerror(QNetworkReply::NetworkError error){
    QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
    emit sigloginfailed();
    loginreply->deleteLater();
}
void leagueserverhandler::loginFinished(){
    loggingstate=true;    
    QStringList sl=connectresponse.split(" ");
    connectresponse.clear();
    if(sl.isEmpty()){
        QMessageBox::information(0,QObject::tr("Warning"),tr("The Server %1 doesnt seem to support the secure logging.").arg(servicename));
        emit sigloginfailed();
        return;
    }
    if(sl.takeFirst()=="0"){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Your %1 Account seems to be wrong, please try again.").arg(servicename));
        emit sigloginfailed();
        return;
    }
    if(sl.isEmpty()){
        QMessageBox::information(0,QObject::tr("Warning"),tr("The Server %1 doesnt seem to support the secure logging.").arg(servicename));
        emit sigloginfailed();
        return;
    }
    nick=sl.takeFirst();        
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
    informationrefreshtimer->start(0);
}
void leagueserverhandler::stoprefresh(){
    informationrefreshtimer->stop();
}
void leagueserverhandler::reset(){
    stoprefresh();
    connecttimer->stop();
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
    connect(refreshreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(refresherror(QNetworkReply::NetworkError)));
}
void leagueserverhandler::refreshFinished(){
    static QStringList sl;
    map.clear();
    foreach(QString s,refreshresponse.split("\n",QString::SkipEmptyParts)){
        sl=s.split(" ",QString::SkipEmptyParts);
        if(sl.isEmpty())
            break;
        map[sl.takeFirst()]=sl;
    }
    refreshresponse.clear();
    refreshreply->deleteLater();
    informationrefreshtimer->start(singleton<settingswindow>().from_map("sbsecureloggingrepeatdelay").toInt());
}

void leagueserverhandler::refreshReadyRead(){
    refreshresponse.append(refreshreply->readAll());
}
void leagueserverhandler::setleague(const QString league, const QString server){
    servicename=league;
    serveraddress=server;
}
void leagueserverhandler::refresherror(QNetworkReply::NetworkError error){
    myDebug()<<tr("Unable to get the user information from")+" "+servicename;
    refreshreply->deleteLater();
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
    connect(logoutreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(logouterror(QNetworkReply::NetworkError)));    
}
void leagueserverhandler::logoutFinished(){
    logoutreply->deleteLater();    
    loggingstate=false;
    emit siglogout();
}
void leagueserverhandler::logoutReadyRead(){

}
void leagueserverhandler::logouterror(QNetworkReply::NetworkError){
    logoutreply->deleteLater();
    loggingstate=false;
    emit siglogout();
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
    connect(profilereply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(profileerror(QNetworkReply::NetworkError)));
}
void leagueserverhandler::profileFinished(){
    emit sigprofile(profileresponse);
    profileresponse.clear();
}
void leagueserverhandler::profileReadyRead(){
    profileresponse.append(profilereply->readAll());
}
void leagueserverhandler::profileerror(QNetworkReply::NetworkError){
    QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
}
