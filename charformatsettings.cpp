/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include "charformatsettings.h"
#include "snpsettings.h"
#include<QFile>
#include<QDir>
#include<QApplication>
#include<QDebug>
QMap<QString, QVariant> charformatsettings::map;
extern bool fontorcolorchanged;
charformatsettings::charformatsettings() {
	// TODO Auto-generated constructor stub

}
void charformatsettings::load() {
        qDebug()<<singleton<snpsettings>().map["charformatfile"].toString();
        QFile f(QApplication::applicationDirPath()+"/textschemes/" + singleton<snpsettings>().map["charformatfile"].toString());
	if (f.open(QFile::ReadOnly)) {
		map.clear();
		QDataStream ds(&f);
		ds.setVersion(QDataStream::Qt_4_3);
		ds >> map;
	} else {
		qDebug() << QObject::tr(
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
			qDebug()<<QObject::tr("There is no textscheme file available.");
	}
}
void charformatsettings::safe() {
	fontorcolorchanged=0;
        QFile f("textschemes/" + singleton<snpsettings>().map["charformatfile"].toString());
	if (!f.open(QFile::WriteOnly | QFile::Truncate)) {
		qDebug() << "cannot write file! " << f.error();
	}
	QDataStream ds(&f);
	ds.setVersion(QDataStream::Qt_4_3);
	ds << map;
}
charformatsettings::~charformatsettings() {
	// TODO Auto-generated destructor stub
}
