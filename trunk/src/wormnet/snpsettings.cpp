/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include"snpsettings.h"
#include"settingswindow.h"
#include"myDebug.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QFileDialog>
#include<QDataStream>
#include<QTranslator>
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
    QString file=singleton<snpsettings>().map["language file"].value<QString> ().remove(".qm");
    if (trans->load(file,"translations/")){
        qApp->installTranslator(trans);
    } else
        myDebug() << QObject::tr("The translationfile cannot be loaded! it might be corrupt.");
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
            singleton<snpsettings>().map["language file"]=s;
            return;
        }
    }
    if(trans->load("_en.Standard.qm","translations/")){
        qApp->installTranslator(trans);
        singleton<snpsettings>().map["language file"]="_en.Standard.qm";
    }
    return;
}
void snpsettings::loadDefaults(){
    map["volumeslidervalue"].setValue<int>(5);
    map["chbminimized"].setValue<bool>(0);
    map["dissallowedclannames"].setValue<QStringList>(QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
    map["charformatfile"].setValue<QString>("comic by lookias.textscheme");
    map["chbsendhostinfotochan"].setValue<bool>(true);
    map["wormnetserverlist"].setValue<QStringList>(defaultServerList);
    installTranslationBySystemLocale();
}
