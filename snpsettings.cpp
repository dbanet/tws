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
snpsettings::snpsettings(){}
snpsettings::~snpsettings(){}
void snpsettings::load(){
    QFile f(QApplication::applicationDirPath()+QDir::separator()+"snpini"+QDir::separator()+"snpini");
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
        if(ds.status()==2 ||ds.status()==1){
            QMessageBox::critical(0,"Error","An error occured while loading the snpini/snpini and snpini/snpinifromlastquit files\n"
                                  "some of your settings might be lost... sry!");
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
