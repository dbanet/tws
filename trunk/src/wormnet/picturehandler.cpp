#include"picturehandler.h"
#include"settings.h"
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
    dir.setPath(":/ranks");
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
    if(S_S.getbool("spectateleagueserver")){
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
//----------------------------------------------------------------------------------------------
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
    case 53:
        s=("CL ");
        break;
    case 54:
        s=("CS ");
        break;
    case 55:
        s=("SI ");
        break;
    case 56:
        s=("LB ");
        break;
    case 57:
        s=("MD ");
        break;
    case 58:
        s=("UA ");
        break;
    case 59:
        s=("LV ");
        break;
    case 60:
        s=("SK ");
        break;
    case 61:
        s=("CR ");
        break;
    case 62:
        s=("EE ");
        break;
    case 63:
        s=("CN ");
        break;
    case 64:
        s=("CO ");
        break;
    case 65:
        s=("EC ");
        break;
    case 66:
        s=("UY ");
        break;
    case 67:
        s=("VE ");
        break;
    case 68:
        s=("LT ");
        break;
    case 69:
        s=("BG ");
        break;
    case 70:
        s=("EG ");
        break;
    case 71:
        s=("SA ");
        break;
    case 72:
        s=("KR ");
        break;
    case 73:
        s=("BY ");
        break;
    case 74:
        s=("PE ");
        break;
    case 75:
        s=("DZ ");
        break;
    case 76:
        s=("KZ ");
        break;
    case 77:
        s=("SV ");
        break;
    case 78:
        s=("TW ");
        break;
    case 79:
        s=("JM ");
        break;
    case 80:
        s=("GT ");
        break;
    case 81:
        s=("MH ");
        break;
    case 82:
        s=("MK ");
        break;
    case 83:
        s=("AE ");
        break;
    default:
        s=("?? ");
        break;
        //*******
    }
    return s;
}
//----------------------------------------------------------------------------------------------
QString picturehandler::map_countrycode_to_number(QString country){
    country=country.trimmed().toUpper();
    int i=49;
    if (country == "UK")
        i = 0;
    else if (country == "AR")
        i = 1;
    else if (country == "AU")
        i = 2;
    else if (country == "AT")
        i = 3;
    else if (country == "BE")
        i = 4;
    else if (country == "BR")
        i = 5;
    else if (country == "CA")
        i = 6;
    else if (country == "HR")
        i = 7;
    else if (country == "BA")
        i = 8;
    else if (country == "CY")
        i = 9;
    else if (country == "CZ")
        i = 10;
    else if (country == "DK")
        i = 11;
    else if (country == "FI")
        i = 12;
    else if (country == "FR")
        i = 13;
    else if (country == "GE")
        i = 14;
    else if (country == "DE")
        i = 15;
    else if (country == "GR")
        i = 16;
    else if (country == "HK")
        i = 17;
    else if (country == "HU")
        i = 18;
    else if (country == "IS")
        i = 19;
    else if (country == "IN")
        i = 20;
    else if (country == "MC")
        i = 21;
    else if (country == "IR")
        i = 22;
    else if (country == "IQ")
        i = 23;
    else if (country == "IE")
        i = 24;
    else if (country == "IL")
        i = 25;
    else if (country == "IT")
        i = 26;
    else if (country == "JP")
        i = 27;
    else if (country == "LI")
        i = 28;
    else if (country == "LU")
        i = 29;
    else if (country == "MY")
        i = 30;
    else if (country == "MT")
        i = 31;
    else if (country == "MX")
        i = 32;
    else if (country == "MA")
        i = 33;
    else if (country == "NL")
        i = 34;
    else if (country == "NZ")
        i = 35;
    else if (country == "NO")
        i = 36;
    else if (country == "PL")
        i = 37;
    else if (country == "PT")
        i = 38;
    else if (country == "CR")
        i = 39;
    else if (country == "RO")
        i = 40;
    else if (country == "RU")
        i = 41;
    else if (country == "SG")
        i = 42;
    else if (country == "ZA")
        i = 43;
    else if (country == "ES")
        i = 44;
    else if (country == "SW")
        i = 45;
    else if (country == "CH")
        i = 46;
    else if (country == "TR")
        i = 47;
    else if (country == "US")
        i = 48;
    else if (country == "AQ")
        i = 49;
    else if (country == "??")
        i = 50;
    else if (country == "e")
        i = 51;
    else if (country == "e")
        i = 52;
    else if (country == "CL")
        i = 53;
    else if (country == "CS")
        i = 54;
    else if (country == "SI")
        i = 55;
    else if (country == "LB")
        i = 56;
    else if (country == "MD")
        i = 57;
    else if (country == "UA")
        i = 58;
    else if (country == "LV")
        i = 59;
    else if (country == "SK")
        i = 60;
    else if (country == "CR")
        i = 61;
    else if (country == "EE")
        i = 62;
    else if (country == "CN")
        i = 63;
    else if (country == "CO")
        i = 64;
    else if (country == "EC")
        i = 65;
    else if (country == "UY")
        i = 66;
    else if (country == "VE")
        i = 67;
    else if (country == "LT")
        i = 68;
    else if (country == "BG")
        i = 69;
    else if (country == "EG")
        i = 70;
    else if (country == "SA")
        i = 71;
    else if (country == "KR")
        i = 72;
    else if (country == "BY")
        i = 73;
    else if (country == "PE")
        i = 74;
    else if (country == "DZ")
        i = 75;
    else if (country == "KZ")
        i = 76;
    else if (country == "SV")
        i = 77;
    else if (country == "TW")
        i = 78;
    else if (country == "JM")
        i = 79;
    else if (country == "GT")
        i = 80;
    else if (country == "MH")
        i = 81;
    else if (country == "MK")
        i = 82;
    else if (country == "AE")
        i = 83;
    return QString::number(i);
}
QString picturehandler::map_hostcountrycode_to_number(QString country){
    country=country.trimmed().toUpper();
    QString s;
    if(country=="DZ") s="1686592068";
    else if(country=="AE") s="1686586689";
    else if(country=="BY") s="1686591810";
    else if(country=="BG") s="1686587202";
    else if(country=="EG") s="1686587205";
    else if(country=="GT") s="1686590535";
    else if(country=="JM") s="1686588746";
    else if(country=="KZ") s="1686592075";
    else if(country=="KR") s="1686590027";
    else if(country=="LT") s="1686590540";
    else if(country=="MK") s="1686588237";
    else if(country=="MH") s="1686587469";
    else if(country=="PE") s="1686586704";
    else if(country=="SA") s="1686585683";
    else if(country=="TW") s="1686591316";
    return s;
}
QString picturehandler::map_hostnumber_to_countrycode(QString number){
    QString s;
    if(s=="1686592068") s="DZ";
    else if(s=="1686586689") s="AE";
    else if(s=="1686591810") s="BY";
    else if(s=="1686587202") s="BG";
    else if(s=="1686587205") s="EG";
    else if(s=="1686590535") s="GT";
    else if(s=="1686588746") s="JM";
    else if(s=="1686592075") s="KZ";
    else if(s=="1686590027") s="KR";
    else if(s=="1686590540") s="LT";
    else if(s=="1686588237") s="MK";
    else if(s=="1686587469") s="MH";
    else if(s=="1686586704") s="PE";
    else if(s=="1686585683") s="SA";
    else if(s=="1686591316") s="TW";
    return s;
}
