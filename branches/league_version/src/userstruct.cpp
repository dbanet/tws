/*
 * userstruct.cpp
 *
 *  Created on: 16.10.2008
 *      Author: looki
 */

#include "userstruct.h"
#include"netcoupler.h"
#include<QHostInfo>
#include<QDebug>
#include<QPointer>
bool *userstruct::boolhelper = new bool;
QMap<int, QString> userstruct::temp;
extern int ranklistsize;
extern int flaglistsize;
extern QPointer<netcoupler> net;
bool userstruct::addressischecked=0;
userstruct::userstruct():flag(49),rank(12) {
}
userstruct::userstruct(QStringList sl) {
    Q_ASSERT(sl.size()>=5);
    chan = sl.takeFirst();
    nickfromclient = sl.takeFirst().remove("~");
    address = sl.takeFirst();
    server = sl.takeFirst();
    nick = sl.takeFirst();
    sl << "" << "" << "" << "" << "" << "";
    unknown = sl.takeFirst();
    commandstart = sl.takeFirst();
    int i = sl.takeFirst().toInt(boolhelper);
    if (*boolhelper && i < flaglistsize && i >= 0)
        flag = i;
    else flag=49;
    i = sl.takeFirst().toInt(boolhelper);
    if (*boolhelper && i < ranklistsize && i >= 0)
        rank = i;
    else rank=12;
    country = sl.takeFirst();
    if (flag >= 49) {
        if (country == "CL")
            flag = 53;
        else if (country == "CS")
            flag = 54;
        else if (country == "SI")
            flag = 55;
        else if (country == "LB")
            flag = 56;
        else if (country == "MD")
            flag = 57;
        else if (country == "UA")
            flag = 58;
        else if (country == "LV")
            flag = 59;
        else if (country == "SK")
            flag = 60;
        else if (country == "CR")
            flag = 61;
        else if (country == "EE")
            flag = 62;
        else if (country == "CN")
            flag = 63;
        else if (country == "CO")
            flag = 64;
        else if (country == "EC")
            flag = 65;
        else if (country == "UY")
            flag = 66;
        else if (country == "VE")
            flag = 67;
    }
    client = sl.join(" ").remove("\r");
    if(addressischecked==0 && nick==net->nick){
        QList<QHostAddress> a=QHostInfo::fromName (address).addresses();
        QString ip;
        if(!a.isEmpty()){
            ip=a.first().toString();
        }
        if(!ip.isEmpty())
            net->myip=ip;
        else
            net->myip=address;
        addressischecked=1;
    }
}
QStringList userstruct::returnwho() {
    QStringList modellist;
    modellist << chan << nickfromclient << address << server << nick << unknown
            << commandstart << QString::number(flag) << QString::number(rank)
            << country << client;
    return modellist;
}
userstruct userstruct::whoami(const QString &s, const QString &t) {
    userstruct u;
    u.nick = s;
    u.chan = t;
    return u;
}
bool userstruct::operator==(const userstruct &u) {
    if (u.nick.toLower() == nick.toLower() && (u.chan == "" || u.chan.toLower() == chan.toLower()))
        return true;
    else
        return false;
}
userstruct::~userstruct() {
    // TODO Auto-generated destructor stub
}
