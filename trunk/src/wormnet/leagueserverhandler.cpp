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
}
leagueserverhandler::~leagueserverhandler(){}
void leagueserverhandler::login(QString n, QString p){
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
void leagueserverhandler::loginFinished(){
    connecttimer->stop();
    QStringList sl=connectresponse.split(" ");
    connectresponse.clear();
    if(sl.size()<2){
        QMessageBox::information(0,QObject::tr("Warning"),tr("The Server %1 doesnt seem to support the secure logging.").arg(servicename));
        emit sigloginfailed();
        return;
    }
    if(sl.takeFirst()=="0"){
        QMessageBox::information(0,QObject::tr("Warning"),tr("Your %1 Account seems to be wrong, please try again.").arg(servicename));        
        emit sigloginfailed();
        return;
    }
    nick=sl.takeFirst();    
    informationrefreshtimer->start(0);
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
void leagueserverhandler::loginReadyRead(){
    connectresponse.append(loginreply->readAll());
}
//###################################
//###################################
//###################################
//###################################

void leagueserverhandler::refresh(){
    QUrl url=serveraddress+"userlist.php?"+nick;
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
void leagueserverhandler::loginerror(QNetworkReply::NetworkError error){
    myDebug()<<tr("unable to connect to")+" "+servicename;
    loginreply->deleteLater();
}
bool leagueserverhandler::contains_key(QString key){
    return map.contains(key);
}
void leagueserverhandler::reset(){
    informationrefreshtimer->stop();
    connecttimer->stop();
}
QString leagueserverhandler::service_name(){
    return servicename;
}
void leagueserverhandler::logout(){
    QUrl url=serveraddress+"testlogin.php?logout="+nick;
    logoutreply=qnam.get(QNetworkRequest(url));
    connect(logoutreply, SIGNAL(finished()),this, SLOT(logoutFinished()));
    connect(logoutreply, SIGNAL(readyRead()),this, SLOT(logoutReadyRead()));
    connect(logoutreply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(logouterror(QNetworkReply::NetworkError)));    
}
void leagueserverhandler::logoutFinished(){
    logoutreply->deleteLater();
    singleton<quithandler>().finished(e_leagueserverhandler);
}
void leagueserverhandler::logoutReadyRead(){

}
void leagueserverhandler::logouterror(QNetworkReply::NetworkError){
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
