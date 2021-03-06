/*
 * hoststruct.cpp
 *
 *  Created on: 14.10.2008
 *      Author: looki
 */

#include "hoststruct.h"
#include "global_functions.h"
#include "singleton.h"
#include "picturehandler.h"
#include "netcoupler.h"
#include <QStringList>
#include <QPixmap>
hoststruct::hoststruct(netcoupler *netc):netc(netc){
    // TODO Auto-generated constructor stub
    valid=false;
}
bool hoststruct::isvalid(){
    return valid;
}
hoststruct::~hoststruct() {
    // TODO Auto-generated destructor stub
}
void hoststruct::sethost(QStringList &sl) {    
    valid=true;
    Q_ASSERT(sl.size()>=7);
    //wa://'+Edit1.Text+Port+'?gameid='+Edit3.Text+'&scheme='+Edit4.Text;
    if (sl.size() >= 7) {
        myname = sl.takeFirst().replace((char)0xA0,' ').replace("#039","'");
        mynick = sl.takeFirst();
        myip = sl.takeFirst();
        bool b;
        myflag = sl.takeFirst().toInt(&b);
        if (!(b && myflag >= 0))
            myflag = 49;
        mycountry=singleton<pictureHandler>().mapNumberToCountryCode(myflag);
        sl.takeFirst();
        mywithkey = sl.takeFirst().toInt();
        myid = sl.takeFirst();
        myjoinstring = "wa://" + myip + "?gameid=" + myid;
        myhoststring = QString("wa://") + "?gameid=" + myid;
        mycountrynumber=sl.join(" ");
    } else {
        bool b;
        if (sl.size() >= 1)
            myname = sl.takeFirst();
        if (sl.size() >= 1)
            mynick = sl.takeFirst();
        if (sl.size() >= 1)
            myip = sl.takeFirst();
        if (sl.size() >= 1)
            myflag = sl.takeFirst().toInt(&b);
        if (sl.size() >= 1)
            if (!(b && myflag >= 0))
                myflag = 49;
        if (sl.size() >= 1)
            sl.takeFirst();
        if (sl.size() >= 1)
            mywithkey = sl.takeFirst().toInt();
        if (sl.size() >= 1)
            myid = sl.takeFirst();
        myjoinstring = "wa://" + myip + "?gameid=" + myid;
        myhoststring = QString("wa://") + "?gameid=" + myid;
        rest = sl.join(" ");
    }
}
void hoststruct::sethost(QString name,QString nick,QString ip,int flag,QString country,QString id,QString pwd,QString chan, QString countrynumber){
    valid=true;
    myname=name;
    mynick=nick;
    myip=ip;
    myflag=flag;
    mycountry=country;
    mycountrynumber=countrynumber;
    myid=id;
    mypwd=pwd;
    mychan=chan;
}

QStringList preparehostlist(QString s){
    QStringList sl;
    int start;
    int stop;
    while(true){
        start=s.indexOf("<GAME ");
        stop=s.indexOf("><BR>");
        if(start==-1)
            return sl;
        start+=6;
        sl<<s.mid(start,stop-start);
        s=s.mid(stop+5);
    }
    return sl;
}
QList<hoststruct> hoststruct::extracthostlist(QString s){
    QStringList sl=preparehostlist(s);
    QList<hoststruct> list;
    foreach(QString s,sl){
        QStringList temp=s.split(" ");
        hoststruct h(netc);
        h.valid=true;
        h.myname=temp.takeFirst();
        h.mynick=temp.takeFirst();
        h.myip=temp.takeFirst();
        temp.takeLast();
        h.myid=temp.takeLast();
        list<<h;
    }    
    return list;
}
bool hoststruct::compare(const hoststruct &h) const{
    if(startswithCI(myip,netc->getmyhostip()) && containsCI(myname,h.myname) && containsCI(mynick,h.mynick))
        return true;
    return false;
}
void hoststruct::setpwd(QString s){
    mypwd=s;
}
void hoststruct::setchan(QString s){
    mychan=s;
}
