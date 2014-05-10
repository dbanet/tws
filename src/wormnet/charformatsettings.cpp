/*
 * snpsettings.cpp
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#include "charformatsettings.h"
#include "settings.h"
#include "myDebug.h"

#include <QFile>
#include <QDir>
#include <QApplication>
QHash<QString, QVariant> chatFormatSettings::map;
extern bool fontOrColorHasChanged;
chatFormatSettings::chatFormatSettings() {
    // TODO Auto-generated constructor stub
}
void chatFormatSettings::load() {
    QFile f(QApplication::applicationDirPath()+"/textschemes/" + S_S.getString("textscheme"));
    if (f.open(QFile::ReadOnly)) {
        map.clear();
        QDataStream ds(&f);
        ds.setVersion(QDataStream::Qt_4_3);
        ds >> map;
    } else {
        myDebug() << QObject::tr("cannot load textscheme file. I apply a random textscheme.");
        QString s="wood by lookias.textscheme";
        QFile f(QApplication::applicationDirPath()+"/textschemes/" + s);
        if (f.open(QFile::ReadOnly)) {
            map.clear();
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            ds >> map;
            S_S.set("textscheme",s);
        } else
            myDebug()<<QObject::tr("There is no textscheme file available.");
    }
}
void chatFormatSettings::safe() {
    fontOrColorHasChanged=0;
    QFile f("textschemes/" + S_S.getString("textscheme"));
    if (!f.open(QFile::WriteOnly | QFile::Truncate))
        myDebug() << QObject::tr("cannot write file:")+" " << S_S.getString("textscheme");
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds << map;
}
chatFormatSettings::~chatFormatSettings() {
    // TODO Auto-generated destructor stub
}
