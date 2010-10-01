/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include "snpsettings.h"
#include"settingswindow.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QFileDialog>
#include<QDataStream>
#include<QTranslator>
#include<QDebug>
extern QStringList defaultServerList;
snpsettings::snpsettings(){}
snpsettings::~snpsettings(){}
void snpsettings::load(){
    if(!QDir().exists("query"))
        QDir().mkdir("query");
    if(!QDir().exists("snpini"))
        QDir().mkdir("snpini");
    QFile f(QApplication::applicationDirPath()+"/snpini/snpini");
    if(!f.open(QFile::ReadOnly)){
        installTranslationBySystemLocale();
        int button=QMessageBox::question(0,QApplication::tr("Question")
                                         ,QApplication::tr("If you like to keep the settings from an older Snooper installation click yes.\n"
                                                           "If you use The Wheat Snooper at the first time just click No.")
                                         ,QMessageBox::Yes | QMessageBox::No);
        if(button==QMessageBox::Yes){
            QString folder;
            while(true){
                folder=QFileDialog::getExistingDirectory(0,qApp->tr("Please choose the folder from the old Snooper.")
                                                         ,qApp->applicationDirPath());
                if(QFile::exists(folder+"/snpini/snpini")){
                    QFile::copy(folder+"/snpini/snpini",QApplication::applicationDirPath()+"/snpini/snpini");
                    break;
                } else{
                    int button=QMessageBox::warning(0,qApp->tr("Warning!"),qApp->tr("This folder doesnt seem to hold a valid installation of The Wheat Snooper. Do you want to keep searching?"),QMessageBox::Yes | QMessageBox::No);
                    if(button==QMessageBox::Yes)
                        continue;                    
                    else {
                        loadDefaults();
                        return;
                    }
                }
            }
            QFile::copy(folder+"/snpini/"+"settingswindowini",QApplication::applicationDirPath()+"/snpini/settingswindowini");
            QFile::copy(folder+"/snpini/"+"ctcp.ini",QApplication::applicationDirPath()+"/snpini/ctcp.ini");
            QFile::copy(folder+"/snpini/"+"clanpages",QApplication::applicationDirPath()+"/snpini/clanpages");

            QFile::copy(folder+"/query/"+"log",QApplication::applicationDirPath()+"/query/log");
            QFile::copy(folder+"/query/"+"querylist",QApplication::applicationDirPath()+"/query/querylist");           
        } else{
            loadDefaults();
            return;
        }        
    }
    f.open(QFile::ReadOnly);
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds>>map;
    if(ds.status()==2 ||ds.status()==1){
        map.clear();
        ds.resetStatus();
        f.setFileName(QApplication::applicationDirPath()+QDir::separator()+"snpini"+QDir::separator()+"snpinifromlastquit");
        f.open(QFile::ReadOnly);
        ds.setDevice(&f);
        ds>>map;
    }
    QTranslator *trans=new QTranslator;
    QString file=singleton<snpsettings>().map["language file"].value<QString> ().remove(".qm");
    if (trans->load(file,QApplication::applicationDirPath() + "/translations/")){
        qApp->installTranslator(trans);
    } else
        qDebug() << "The translationfile cannot be loaded! it might be corrupt.";
}
void snpsettings::safe(){
    QFile f(QApplication::applicationDirPath()+QDir::separator()+"snpini"+QDir::separator()+"snpini");
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds<<map;
}
void snpsettings::safeonquit(){
    QFile f(QApplication::applicationDirPath()+QDir::separator()+"snpini"+QDir::separator()+"snpinifromlastquit");
    f.open(QFile::WriteOnly);
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds<<map;
}
void snpsettings::installTranslationBySystemLocale(){
    QTranslator *trans=new QTranslator;
    QString language=QLocale::system().name().left(2);
    QDir dir("translations");
    foreach(QString s,dir.entryList()){
        if(s.startsWith("_") && s.mid(1,2)==language){
            trans->load(s,QApplication::applicationDirPath() + "/translations/");
            qApp->installTranslator(trans);
            return;
        }
    }
    if(trans->load("_en.Standard.qm",QApplication::applicationDirPath() + "/translations/"))
        qApp->installTranslator(trans);
    return;
}
void snpsettings::loadDefaults(){
    map["volumeslidervalue"].setValue<int>(5);
    map["chbminimized"].setValue<bool>(0);
    map["dissallowedclannames"].setValue<QStringList>(QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan");
    map["charformatfile"].setValue<QString>("comic by lookias.textscheme");
    map["chbsendhostinfotochan"].setValue<bool>(true);
    map["wormnetserverlist"].setValue<QStringList>(defaultServerList);
    installTranslationBySystemLocale();
}
