#include "picturehandler.h"
#include"snpsettings.h"
#include"global_macros.h"
#include"leagueserverhandler.h"
#include"settingswindow.h"
#include"myDebug.h"
#include"global_functions.h"
#include<QPixmap>
#include<QDir>
#include<QComboBox>
#include<QIcon>
#include<QApplication>

picturehandler::picturehandler(){
    QDir dir(":/flags");
    QString s;
    int i=0;
    foreach(QString f,dir.entryList(QStringList()<<"*.png",QDir::Files,QDir::Name)) {
        s=f;
        flaglist[s.remove(".png").toLower().trimmed()]=new QPixmap(dir.path() + "/" + f);
        i++;
    }
    dir.setPath(QApplication::applicationDirPath() + "/snppictures/ranks");
    foreach(QString f,dir.entryList(QStringList()<<"*.png",QDir::Files,QDir::Name)) {
        ranklist.push_back(new QPixmap(dir.path() + "/" + f));
    }
    dir.setPath(":/tusranks");
    foreach(QString f,dir.entryList(QStringList()<<"*.png",QDir::Files,QDir::Name)) {
        leagueranklist[QString(f).remove(".png",Qt::CaseInsensitive)]=new QPixmap(dir.path() + "/" + f);
    }
    locked = new QPixmap;
    unlocked = new QPixmap;
    if (!locked->load(QApplication::applicationDirPath() + "/snppictures/locked.png"))
        myDebug() << QObject::tr("some pictures are missing!");
    if (!unlocked->load(QApplication::applicationDirPath() + "/snppictures/unlocked.png"))
        myDebug() << QObject::tr("some pictures are missing!");   
}
picturehandler::~picturehandler(){}
//----------------------------------------------------------------------------------------------
QPixmap *picturehandler::getflag(QString country){
    if(!flaglist.contains(country.toLower().trimmed()))
        return flaglist["q"];
    return flaglist[country.toLower().trimmed()];
}
//----------------------------------------------------------------------------------------------
void picturehandler::fillflags(QComboBox *cb){
    QMap<QString,QPixmap*>::ConstIterator it=flaglist.begin();
    while(it!=flaglist.end()){
        cb->addItem(**it,it.key());
        it++;
    }
}
//----------------------------------------------------------------------------------------------
void picturehandler::fillranks(QComboBox *cb){
    QList<QPixmap*>::ConstIterator it=ranklist.begin();
    int i=0;
    while(it!=ranklist.end()){
        cb->addItem(**it,QString::number(i++));
        it++;
    }
}
//----------------------------------------------------------------------------------------------
QVariant picturehandler::getflag(const userstruct &u) const{
    if(singleton<snpsettings>().map["spectateleagueserver"].toBool()){
        QString s=singleton<leagueserverhandler>().map_at_toString(u.nick,leagueserverhandler::e_flag);
        if(!s.isEmpty()){
            if(!flaglist.contains(s.toLower().trimmed()))
                return *flaglist["q"];
            return *flaglist[s.toLower().trimmed()];
        }
        if(singleton<settingswindow>().from_map("cbonlyshowflagsfromverifiedusers").toBool())
            return QVariant();
    }
    if(!flaglist.contains(u.country.toLower().trimmed())){
        if(u.country.toLower().trimmed()!="??")
            myDebug()<<"flag: "<<u.country.toLower().trimmed()<<" is missing. used by "<<u.nick;
        return *flaglist["q"];
    }
    return *flaglist[u.country.toLower().trimmed()];
}
//----------------------------------------------------------------------------------------------
QPixmap *picturehandler::getrank(int i){
    if(i>ranklist.size())
        return ranklist[12];
    return ranklist[i];
}
QPixmap *picturehandler::getleaguerank(QString s){
    if(!leagueranklist.contains(s))
        return ranklist[12];
    return leagueranklist[s];
}
int picturehandler::ranklistsize(){
    return ranklist.size();
}
QString picturehandler::map_number_to_countrycode(const int i){
    QString s="??";
    switch(i){
        //written by steps
    case 0:
        s=("UK ");
        break;
    case 1:
        s=("AR ");
        break;
    case 2:
        s=("AU ");
        break;
    case 3:
        s=("AT ");
        break;
    case 4:
        s=("BE ");
        break;
    case 5:
        s=("BR ");
        break;
    case 6:
        s=("CA ");
        break;
    case 7:
        s=("HR ");
        break;
    case 8:
        s=("BA ");
        break;
    case 9:
        s=("CY ");
        break;
    case 10:
        s=("CZ ");
        break;
    case 11:
        s=("DK ");
        break;
    case 12:
        s=("FI ");
        break;
    case 13:
        s=("FR ");
        break;
    case 14:
        s=("GE ");
        break;
    case 15:
        s=("DE ");
        break;
    case 16:
        s=("GR ");
        break;
    case 17:
        s=("HK ");
        break;
    case 18:
        s=("HU ");
        break;
    case 19:
        s=("IS ");
        break;
    case 20:
        s=("IN ");
        break;
    case 21:
        s=("MC ");
        break;
    case 22:
        s=("IR ");
        break;
    case 23:
        s=("IQ ");
        break;
    case 24:
        s=("IE ");
        break;
    case 25:
        s=("IL ");
        break;
    case 26:
        s=("IT ");
        break;
    case 27:
        s=("JP ");
        break;
    case 28:
        s=("LI ");
        break;
    case 29:
        s=("LU ");
        break;
    case 30:
        s=("MY ");
        break;
    case 31:
        s=("MT ");
        break;
    case 32:
        s=("MX ");
        break;
    case 33:
        s=("MA ");
        break;
    case 34:
        s=("NL ");
        break;
    case 35:
        s=("NZ ");
        break;
    case 36:
        s=("NO ");
        break;
    case 37:
        s=("PL ");
        break;
    case 38:
        s=("PT ");
        break;
    case 39:
        s=("CR ");
        break;
    case 40:
        s=("RO ");
        break;
    case 41:
        s=("RU ");
        break;
    case 42:
        s=("SG ");
        break;
    case 43:
        s=("ZA ");
        break;
    case 44:
        s=("ES ");
        break;
    case 45:
        s=("SW ");
        break;
    case 46:
        s=("CH ");
        break;
    case 47:
        s=("TR ");
        break;
    case 48:
        s=("US ");
        break;
    case 49:
        s=("Q ");
        break;
    case 50:
        s=("E ");
        break;
    case 51:
        s=("E ");
        break;
    case 52:
        s=("E ");
        break;
    }
    return s;
}
//----------------------------------------------------------------------------------------------
int picturehandler::map_countrycode_to_number(const QString country){
    int i=999;
    if (compareCI(country, "UK"))
        i = 0;
    else if (compareCI(country , "AR"))
        i = 1;
    else if (compareCI(country , "AU"))
        i = 2;
    else if (compareCI(country , "AT"))
        i = 3;
    else if (compareCI(country , "BE"))
        i = 4;
    else if (compareCI(country , "BR"))
        i = 5;
    else if (compareCI(country , "CA"))
        i = 6;
    else if (compareCI(country , "HR"))
        i = 7;
    else if (compareCI(country , "BA"))
        i = 8;
    else if (compareCI(country , "CY"))
        i = 9;
    else if (compareCI(country , "CZ"))
        i = 10;
    else if (compareCI(country , "DK"))
        i = 11;
    else if (compareCI(country , "FI"))
        i = 12;
    else if (compareCI(country , "FR"))
        i = 13;
    else if (compareCI(country , "GE"))
        i = 14;
    else if (compareCI(country , "DE"))
        i = 15;
    else if (compareCI(country , "GR"))
        i = 16;
    else if (compareCI(country , "HK"))
        i = 17;
    else if (compareCI(country , "HU"))
        i = 18;
    else if (compareCI(country , "IS"))
        i = 19;
    else if (compareCI(country , "IN"))
        i = 20;
    else if (compareCI(country , "MC"))
        i = 21;
    else if (compareCI(country , "IR"))
        i = 22;
    else if (compareCI(country , "IQ"))
        i = 23;
    else if (compareCI(country , "IE"))
        i = 24;
    else if (compareCI(country , "IL"))
        i = 25;
    else if (compareCI(country , "IT"))
        i = 26;
    else if (compareCI(country , "JP"))
        i = 27;
    else if (compareCI(country , "LI"))
        i = 28;
    else if (compareCI(country , "LU"))
        i = 29;
    else if (compareCI(country , "MY"))
        i = 30;
    else if (compareCI(country , "MT"))
        i = 31;
    else if (compareCI(country , "MX"))
        i = 32;
    else if (compareCI(country , "MA"))
        i = 33;
    else if (compareCI(country , "NL"))
        i = 34;
    else if (compareCI(country , "NZ"))
        i = 35;
    else if (compareCI(country , "NO"))
        i = 36;
    else if (compareCI(country , "PL"))
        i = 37;
    else if (compareCI(country , "PT"))
        i = 38;
    else if (compareCI(country , "CR"))
        i = 39;
    else if (compareCI(country , "RO"))
        i = 40;
    else if (compareCI(country , "RU"))
        i = 41;
    else if (compareCI(country , "SG"))
        i = 42;
    else if (compareCI(country , "ZA"))
        i = 43;
    else if (compareCI(country , "ES"))
        i = 44;
    else if (compareCI(country , "SW"))
        i = 45;
    else if (compareCI(country , "CH"))
        i = 46;
    else if (compareCI(country , "TR"))
        i = 47;
    else if (compareCI(country , "US"))
        i = 48;
    else if (compareCI(country , "??"))
        i = 49;
    else if (compareCI(country , "e"))
        i = 50;
    else if (compareCI(country , "e"))
        i = 51;    
    return i;
}
