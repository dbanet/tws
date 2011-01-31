/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include"snpsettings.h"
#include"settingswindow.h"
#include"myDebug.h"
#include"about.h"
#include"global_functions.h"

#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QFileDialog>
#include<QDataStream>
#include<QTranslator>
#include<QLocale>

extern QStringList defaultServerList;
const QString Version_String="1";
snpsettings::snpsettings(){}
snpsettings::~snpsettings(){}

void snpsettings::load(){
    bool isOnFirstRun=false;
    if(!QDir().exists("query"))
        QDir().mkdir("query");
    if(!QDir().exists("snpini"))
        QDir().mkdir("snpini");
    QFile f("snpini/snpini");
    if(!f.open(QFile::ReadOnly)){
        installTranslationBySystemLocale();
        int button=QMessageBox::question(0,QObject::tr("Question")
                                         ,QApplication::tr("If you like to keep the settings from an older Snooper installation click yes.\n"
                                                           "If you use The Wheat Snooper at the first time just click No.")
                                         ,QMessageBox::Yes | QMessageBox::No);
        if(button==QMessageBox::Yes){
            QString folder;
            while(true){
                folder=QFileDialog::getExistingDirectory(0,QApplication::tr("Please choose the folder from the old Snooper.")
                                                         ,QApplication::applicationDirPath());
                folder+="/";
                if(QFile::exists(folder+"snpini/snpini")){
                    QFile::copy(folder+"snpini/snpini","snpini/snpini");
                    break;
                } else{
                    int button=QMessageBox::warning(0,QObject::tr("Warning!"),QApplication::tr("This folder doesnt seem to hold a valid installation of The Wheat Snooper. Do you want to keep searching?"),QMessageBox::Yes | QMessageBox::No);
                    if(button==QMessageBox::Yes)
                        continue;
                    else {
                        loadDefaults();
                        return;
                    }
                }
            }
            QFile::copy(folder+"snpini/settingswindowini","snpini/settingswindowini");
            QFile::copy(folder+"snpini/ctcp.ini","snpini/ctcp.ini");
            QFile::copy(folder+"snpini/clanpages","snpini/clanpages");

            QFile::copy(folder+"query/log","query/log");
            QFile::copy(folder+"query/querylist","query/querylist");
            isOnFirstRun=true;
        } else{
            loadDefaults();
            return;
        }
        f.open(QFile::ReadOnly);
    }
    version=Version_String;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds>>map;
    QString s;
    if(!QDir().exists("snpini/256")){
        isOnFirstRun=true;
        QDir().mkdir("snpini/256");
    } else {
        ds>>version;
        if(version=="end"){
            s="end";
            version=Version_String;
        }
        else
            ds>>s;
    }
    if(s!="end" && !isOnFirstRun){
        QFile::remove("snpini/snpinibackup");
        QFile::copy("snpini/snpini","snpini/snpinibackup");
        map.clear();
        ds.resetStatus();
        f.setFileName("snpini/snpinifromlastquit");
        f.open(QFile::ReadOnly);
        ds.setDevice(&f);
        map.clear();
        ds>>map;
        if(QDir().exists("snpini/256"))
            ds>>version;
        ds>>s;
        if(s!="end"){
            map.clear();
            loadDefaults();
            return;
        }
    }
    QTranslator *trans=new QTranslator;
    QString file=S_S.map["language_file"].value<QString> ().remove(".qm");
    if (trans->load(file,"translations/")){
        qApp->installTranslator(trans);
    } else
        myDebug() << QObject::tr("The translationfile cannot be loaded! it might be corrupt.");
    validate();
}
void snpsettings::safe(){
    QFile f("snpini/snpini");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds<<map;
    ds<<Version_String;
    ds<<QString("end");
}
void snpsettings::safeonquit(){
    QFile f("snpini/snpinifromlastquit");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds<<map;
    ds<<Version_String;
    ds<<QString("end");
}
void snpsettings::installTranslationBySystemLocale(){
    QTranslator *trans=new QTranslator;
    QString language=QLocale::system().name().left(2);
    QDir dir("translations");
    foreach(QString s,dir.entryList()){
        if(s.startsWith("_") && s.mid(1,2)==language){
            trans->load(s,"translations/");
            qApp->installTranslator(trans);
            S_S.map["language_file"]=s;
            return;
        }
    }
    if(trans->load("_en.Standard.qm","translations/")){
        qApp->installTranslator(trans);
        S_S.map["language_file"]="_en.Standard.qm";
    }
    return;
}
void snpsettings::loadDefaults(){
    map["volumeslidervalue"].setValue<int>(5);
    map["chbminimized"].setValue<bool>(0);
    map["dissallowedclannames"].setValue<QStringList>(QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
    map["charformatfile"].setValue<QString>("comic by lookias.textscheme");
    map["chbsendhostinfotochan"].setValue<bool>(true);
    installTranslationBySystemLocale();
    validate();
}
void snpsettings::checkifexistsinstringlist(QString key,QString value){
    if(map[key].toStringList().contains(value,Qt::CaseInsensitive))
        return;
    QStringList sl=map[key].toStringList();
    sl<<value;
    map[key]=sl;
}
void snpsettings::validate(){
    checkifexistsinstringlist("leagueservers","http://www.tus-wa.com/");
    checkifexistsinstringlist("leagueservers","http://www.normalnonoobs.org/");
    checkifexistsinstringlist("leagueservers","http://lookias.worms2d.info/securelogging/");
    checkifexistsinstringlist("wormnetserverlist","wormnet1.team17.com");
    checkifexistsinstringlist("wormnetserverlist","itakagames.spb.ru");
    checkifexistsinstringlist("wormnetserverlist","212.240.191.125");
    checkifexistsinstringlist("wormnetserverlist","worms.tom.ru");
    checkifexistsinstringlist("wormnetserverlist","http://steps.servegame.com");

    if(S_S.map["rank"].toString().isEmpty())
        S_S.map["rank"]="13";
    if(S_S.map["countrycode"].toString().isEmpty())
        S_S.map["countrycode"]="49";
    if(S_S.map["qss_file"].toString().isEmpty())
        S_S.map["qss_file"]="black by lookias.qss";
    if(S_S.map["information"].toString().isEmpty() ||
       S_S.map["information"].toString().startsWith("The Wheat Snooper"))
        S_S.map["information"]="The Wheat Snooper "+about::version;
    if(!map.contains("showinformation")){
        map["showinformation"]=true;
    }
    if(!map.contains("spectatingneversettedoff"))
        map["spectatingneversettedoff"]=true;
}

QString snpsettings::getstring(QString key) const{
    return map[key].toString();
}
QStringList snpsettings::getstringlist(QString key) const{
    return map[key].toStringList();
}
bool snpsettings::getbool(QString key) const{
    return map[key].toBool();
}
int snpsettings::getint(QString key) const{
    return map[key].toInt();
}
QVariantList snpsettings::getlist(QString key) const{
    return map[key].toList();
}
void snpsettings::set(QString key, QVariant value){
    map[key]=value;
}
QByteArray snpsettings::getbytearray(QString key) const{
    return map[key].toByteArray();
}
bool snpsettings::contains(QString key) const{
    return map.contains(key);
}
