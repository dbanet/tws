/*
 * inihandlerclass.cpp
 *
 *  Created on: 17.10.2008
 *      Author: looki
 */

#include "inihandlerclass.h"
#include<QFile>
#include<QApplication>
#include<QDir>
#include<QDebug>
inihandlerclass::inihandlerclass() {
    file="wormnet.net";

    // TODO Auto-generated constructor stub
}

inihandlerclass::~inihandlerclass() {
    // TODO Auto-generated destructor stub
}
void inihandlerclass::tcpwrite(const QString &inistring, QTcpSocket& tcp) {
    QFile f(QApplication::applicationDirPath() + "/network/" + file);
    if (!f.open(QFile::ReadOnly)) {
        qDebug() << "the file network/wormnet.net is missing!";
    }
    QTextStream ts(&f);
    while (ts.readLine() != inistring && !ts.atEnd())
        ;
    if (ts.atEnd()) {
        qDebug() << "your network/wormnet.net file is corrupt! " << inistring
                << " can not be found, but is important.";
        return;
    }
    QString s;
    s = ts.readLine();
    while (!s.startsWith("[") && !ts.atEnd()) {
        if(!s.startsWith("//") && s!=""){
            tcp.write(s.toAscii());
            tcp.write("\n");
        }
        s = ts.readLine();
    }
}
QStringList inihandlerclass::stringlistfromini(const QString &inistring) {    
    QFile f;
    f.setFileName(QApplication::applicationDirPath() + "/network/" + file);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "network/wormnet.net is missing!";
    }
    QTextStream ts(&f);
    while (ts.readLine() != inistring && !ts.atEnd())
        ;
    if (ts.atEnd()) {
        qDebug() << "your wormnet.net file is corrupt! " << inistring
                << " can not be found, but is important.";
        return QStringList("");
    }
    QStringList sl;
    QString s(ts.readLine());
    while (!s.startsWith("[") && !ts.atEnd()) {
        if(!s.startsWith("//") && s!=""){
            sl << s;
        }
        s = ts.readLine();
    }
    return sl;
}
QNetworkRequest inihandlerclass::requestfromini(const QString &inistring) {
    QFile f;
    f.setFileName(QApplication::applicationDirPath() + "/network/" + file);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "network/wormnet.net is missing!";
    }
    QTextStream ts(&f);
    while (ts.readLine() != inistring && !ts.atEnd())
        ;
    if (ts.atEnd()) {
        qDebug() << "your network/wormnet.net file is corrupt! " << inistring
                << " can not be found, but is important.";
    }

    QNetworkRequest r;
    QStringList sl;
    QString s(ts.readLine());
    while (!s.startsWith("[") && !ts.atEnd()) {
        if(!s.startsWith("//") && s!=""){
            sl = s.split(" ");
            QString s1=sl.takeFirst();
            QString s2=sl.join(" ");
            r.setRawHeader(s1.toAscii(), s2.toAscii());
        }
        s = ts.readLine();
    }
    return r;
}
