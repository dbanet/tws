#include "clantowebpagemapper.h"
#include"settingswindow.h"
#include"snpsettings.h"
#include"leagueserverhandler.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QUrl>
#include<QNetworkReply>
clantowebpagemapper::clantowebpagemapper()
{
}
clantowebpagemapper::~clantowebpagemapper()
{
}
void clantowebpagemapper::load(){
//    QMap<QString,QString> temp;
//    QFile f(QApplication::applicationDirPath() + "/snpini/clanpages");
//    if (!f.open(QFile::ReadOnly))
//        loadDefaults();
//    else {
//        QDataStream ds(&f);
//        ds.setVersion(QDataStream::Qt_4_3);
//        ds >> temp;
//    }
//    foreach(QString key,temp.keys()){
//        map[key.toLower()]=temp[key];
//    }
    refresh();
}
void clantowebpagemapper::safe() {
//    QFile f(QApplication::applicationDirPath() + "/snpini/clanpages");
//    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
//        QDataStream ds(&f);
//        ds.setVersion(QDataStream::Qt_4_3);
//        ds << map;
//    }
}
bool clantowebpagemapper::contains(const QString &key){
    return map.contains(key.toLower());
}
QString clantowebpagemapper::value(const QString &key){
    return map[key.toLower()];
}
void clantowebpagemapper::loadDefaults(){
}
void clantowebpagemapper::refresh(){
    QUrl url("http://lookias.worms2d.info/wheat/clanlist.txt.php");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}
void clantowebpagemapper::httpFinished(){
    reply->deleteLater();
    if(reply->error()!=QNetworkReply::NoError)
        return;
    QStringList sl=file.split("\n");
    foreach(QString s,sl){
        if(s.isEmpty())
            continue;
        QStringList temp=s.trimmed().split(" ");
        if(temp.size()<2)
            continue;
        QString s=temp.takeFirst().toLower();
        map[s]=temp.takeFirst();
        InformationMap[s]=temp.join(" ");
    }
}
void clantowebpagemapper::httpReadyRead(){
    file.append(reply->readAll());
}
QString clantowebpagemapper::getInformation(const userstruct &u){
    QString clan=u.clan.toLower();
    if(S_S.map["spectateleagueserver"].toBool()){
        if(singleton<settingswindow>().from_map("cbshowranksonlyfromsecureloggedusers").toBool()){
            clan=singleton<leagueserverhandler>().map_at_toString(u.nick,leagueserverhandler::e_clan);
        }
    }
    if(!InformationMap.contains(clan))
        return QString();
    return InformationMap[clan];
}
