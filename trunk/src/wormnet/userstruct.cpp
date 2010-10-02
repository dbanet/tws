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
    if (flag >= 49 && flag <=83) {
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
        //written by steps
        else if (country == "LT")
            flag = 68;
        else if (country == "BG")
            flag = 69;
        else if (country == "EG")
            flag = 70;
        else if (country == "SA")
            flag = 71;
        else if (country == "KR")
            flag = 72;
        else if (country == "BY")
            flag = 73;
        else if (country == "PE")
            flag = 74;
        else if (country == "DZ")
            flag = 75;
        else if (country == "KZ")
            flag = 76;
        else if (country == "SV")
            flag = 77;
        else if (country == "TW")
            flag = 78;
        else if (country == "JM")
            flag = 79;
        else if (country == "GT")
            flag = 80;
        else if (country == "MH")
            flag = 81;
        else if (country == "MK")
            flag = 82;
        else if (country == "AE")
            flag = 83;
        //*******
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
