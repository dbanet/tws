/*
 * userstruct.cpp
 *
 *  Created on: 16.10.2008
 *      Author: looki
 */

#include "userstruct.h"
#include"netcoupler.h"
#include"myDebug.h"
#include"global_functions.h"
#include"singleton.h"
#include"picturehandler.h"
#include<QHostInfo>
#include<QPointer>
bool *userstruct::boolhelper = new bool;
QMap<int, QString> userstruct::temp;
bool userstruct::addressischecked=0;
userstruct::userstruct():flag(49),rank(12) {
}
userstruct::userstruct(QStringList sl) {
    Q_ASSERT(sl.size()>=5);
    chan = sl.takeFirst();
    clan = sl.takeFirst().remove("~");
    address = sl.takeFirst();
    server = sl.takeFirst();
    nick = sl.takeFirst();
    sl << "" << "" << "" << "" << "" << "";
    unknown = sl.takeFirst();
    commandstart = sl.takeFirst();
    int i = sl.takeFirst().toInt(boolhelper);
    if (*boolhelper && i >= 0)
        flag = i;
    else
        flag=49;
    int clannumber = sl.takeFirst().toInt(boolhelper);
    country = sl.takeFirst();    
    if(flag<53 && flag !=49)
        country=singleton<picturehandler>().map_number_to_countrycode(flag);
    if(country.isEmpty())
        country="??";
    client = sl.join(" ").remove("\r");    
    if (*boolhelper && clannumber < singleton<picturehandler>().ranklistsize() && clannumber >= 0)
        rank = clannumber;
    else rank=12;
    if(addressischecked==0 && nick==singleton<netcoupler>().nick){
        QList<QHostAddress> a=QHostInfo::fromName (address).addresses();
        QString ip;
        if(!a.isEmpty()){
            ip=a.first().toString();
        }
        if(!ip.isEmpty())
            singleton<netcoupler>().myip=ip;
        else
            singleton<netcoupler>().myip=address;
        addressischecked=1;
    }
}
QStringList userstruct::returnwho() {
    QStringList modellist;
    modellist << chan << clan << address << server << nick << unknown
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
