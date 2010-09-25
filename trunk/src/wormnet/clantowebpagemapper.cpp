#include "clantowebpagemapper.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QUrl>
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
    QUrl url("http://lookias.bplaced.com/wheat/clanlist.txt.php");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}
void clantowebpagemapper::httpFinished(){
    reply->deleteLater();
    QStringList sl=file.split("\n");
    foreach(QString s,sl){
        if(s.isEmpty())
            continue;
        QStringList temp=s.trimmed().split(" ");
        QString s=temp.takeFirst().toLower();
        map[s]=temp.takeFirst();
        InformationMap[s]=temp.join(" ");
    }
}
void clantowebpagemapper::httpReadyRead(){
    file.append(reply->readAll());
}
QString clantowebpagemapper::getInformation(QString key){
    if(!InformationMap.contains(key.toLower()))
        return "";
    return InformationMap[key.toLower()];
}
