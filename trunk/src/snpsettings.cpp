/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include "snpsettings.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QMessageBox>
#include<QFileDialog>
extern QStringList defaultServerList;
snpsettings::snpsettings(){}
snpsettings::~snpsettings(){}
void snpsettings::load(){
    QFile f(QApplication::applicationDirPath()+QDir::separator()+"snpini"+QDir::separator()+"snpini");
    if(!f.open(QFile::ReadOnly)){
        int button=QMessageBox::question(0,QApplication::tr("Question")
                                         ,QApplication::tr("If you like to keep the settings from an older Snooper installation click yes.\n"
                                                           "If you use The Wheat Snooper at the first time just click No.")
                                         ,QMessageBox::Yes | QMessageBox::No);
        if(button==QMessageBox::Yes){
            QString folder=QFileDialog::getExistingDirectory(0,qApp->tr("Please choose the folder from the old Snooper.")
                                                             ,qApp->applicationDirPath());
            if(folder.isEmpty())
                return;
            folder=folder+"/snpini/";
            if(QFile::exists(folder+"snpini")){
                QFile::copy(folder+"snpini",QApplication::applicationDirPath()+"/snpini/snpini");
            } else
                return;
            if(QFile::exists(folder+"settingswindowini")){
                QFile::copy(folder+"settingswindowini",QApplication::applicationDirPath()+"/snpini/settingswindowini");
            }
            if(QFile::exists(folder+"ctcp.ini")){
                QFile::copy(folder+"ctcp.ini",QApplication::applicationDirPath()+"/snpini/ctcp.ini");
            }
        } else{
            QDir().mkdir("snpini");
            loadDefaults();
            return;
        }
    }
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
void snpsettings::loadDefaults(){
    singleton<snpsettings>().map["volumeslidervalue"].setValue<int>(5);
    singleton<snpsettings>().map["chbminimized"].setValue<bool>(0);
    singleton<snpsettings>().map["dissallowedclannames"].setValue<QStringList>(QStringList()<<"Username");
    singleton<snpsettings>().map["language file"].setValue<QString> ("The_Wheat_Snooper_untranslated");
    singleton<snpsettings>().map["charformatfile"].setValue<QString>("comic by lookias.textscheme");
    singleton<snpsettings>().map["chbsendhostinfotochan"].setValue<bool>(true);  
    singleton<snpsettings>().map["wormnetserverlist"].setValue<QStringList>(defaultServerList);
}
