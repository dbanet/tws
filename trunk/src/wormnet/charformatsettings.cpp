/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include "charformatsettings.h"
#include "snpsettings.h"
#include"myDebug.h"
#include<QFile>
#include<QDir>
#include<QApplication>
QMap<QString, QVariant> charformatsettings::map;
extern bool fontorcolorchanged;
charformatsettings::charformatsettings() {
	// TODO Auto-generated constructor stub

}
void charformatsettings::load() {
        myDebug()<<S_S.getstring("charformatfile");
        QFile f(QApplication::applicationDirPath()+"/textschemes/" + S_S.getstring("charformatfile"));
	if (f.open(QFile::ReadOnly)) {
		map.clear();
		QDataStream ds(&f);
		ds.setVersion(QDataStream::Qt_4_3);
		ds >> map;
	} else {
                myDebug() << QObject::tr(
				"cannot load textscheme file. I apply a random textscheme.");
                QDir dir(QApplication::applicationDirPath()+"/textschemes");
		QStringList sl = dir.entryList(QStringList("*.textscheme"),QDir::Files);
		if (!sl.isEmpty()) {
			QFile f(QApplication::applicationDirPath()+"/textschemes/" + sl.first());
			if (f.open(QFile::ReadOnly)) {
				map.clear();
				QDataStream ds(&f);
				ds.setVersion(QDataStream::Qt_4_3);
				ds >> map;
			}
		} else
                        myDebug()<<QObject::tr("There is no textscheme file available.");
	}
}
void charformatsettings::safe() {
	fontorcolorchanged=0;
        QFile f("textschemes/" + S_S.getstring("charformatfile"));
	if (!f.open(QFile::WriteOnly | QFile::Truncate)) {
            myDebug() << QObject::tr("cannot write file:")+" " << S_S.getstring("charformatfile");
	}
	QDataStream ds(&f);
	ds.setVersion(QDataStream::Qt_4_3);
	ds << map;
}
charformatsettings::~charformatsettings() {
	// TODO Auto-generated destructor stub
}
