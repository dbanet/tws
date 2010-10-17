#include "leagueserverhandler.h"
#include"myDebug.h"
#include"singleton.h"
#include"balloon_handler.h"
#include"settingswindow.h"
#include"snpsettings.h"
#include"global_functions.h"
#include<QNetworkReply>
#include<QMessageBox>
#include<QUrl>
#include<QTimer>
#include<QDesktopServices>
QMap<QString,QStringList> leagueserverhandler::leagueserverhandler::map;
QString leagueserverhandler::nick;
leagueserverhandler::leagueserverhandler(QObject *parent) :
        QObject(parent)
{
    connecttimer=new QTimer;
    connecttimer->setSingleShot(true);
    connect(connecttimer,SIGNAL(timeout()),this,SLOT(logintTimeOut()));

    informationrefreshtimer=new QTimer;
    informationrefreshtimer->setSingleShot(true);
    connect(informationrefreshtimer,SIGNAL(timeout()),this,SLOT(refresh()));
}
void leagueserverhandler::login(QString n, QString p){
    informationrefreshtimer->stop();
    connecttimer->stop();
    myconnect(n,p);    
}
void leagueserverhandler::myconnect(const QString n,const QString p){
    QUrl url=serveraddress+"testlogin.php?u="+n+"&p="+p;
    connectreply=qnam.get(QNetworkRequest(url));
    connect(connectreply, SIGNAL(finished()),this, SLOT(loginFinished()));
    connect(connectreply, SIGNAL(readyRead()),this, SLOT(loginReadyRead()));
    connect(connectreply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(loginerror(QNetworkReply::NetworkError)));
    connecttimer->start(5000);
}
void leagueserverhandler::logintTimeOut(){
    QMessageBox::information(0,QObject::tr("Warning"),tr("Cant connect to %1 server, please try again at a later time.").arg(servicename));
    emit sigloginfailed();
    connectreply->deleteLater();
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
    connectreply->deleteLater();
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
    connectresponse.append(connectreply->readAll());
}
//###################################
//###################################
//###################################
//###################################

void leagueserverhandler::refresh(){
    QUrl url=serveraddress+"UserList.php";
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
    static bool b;
    if(!map.contains(key))
        return QString();
    if(i>=map[key].size())
        return QString();
    return map[key].at(i);
}

void leagueserverhandler::setleague(const QString league, const QString server){
    servicename=league;
    serveraddress=server;
}
void leagueserverhandler::refresherror(QNetworkReply::NetworkError error){
    myDebug()<<tr("Unable to get the user information from")+" "+servicename;
}

void leagueserverhandler::loginerror(QNetworkReply::NetworkError error){
    myDebug()<<tr("unable to connect to")+" "+servicename;
}
bool leagueserverhandler::contains_key(QString key){
    return map.contains(key);
}
