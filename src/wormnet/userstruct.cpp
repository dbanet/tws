/*
 * userstruct.cpp
 *
 *  Created on: 16.10.2008
 *      Author: looki
 */

#include "userstruct.h"
#include "netcoupler.h"
#include "myDebug.h"
#include "global_functions.h"
#include "singleton.h"
#include "picturehandler.h"
#include <QHostInfo>
#include <QPointer>
bool userstruct::addressischecked=0;
userstruct::userstruct():flag(49),rank(12) {
}
userstruct::userstruct(QStringList sl){
    Q_ASSERT(sl.size()>=5); // sl example: #AnythingGoes Username no.address.for.you wormnet1.team17.com Scal H :0 48 0 US The Wheat Snooper 2.9

    this->channel=sl.takeFirst();                      // the channel the user is being on
    this->clan=sl.takeFirst().remove("~");             // username of the user (ident or ~nick)
    this->address=sl.takeFirst();                      // user's IP (typically no.address.for.you for wormnets)
    this->server=sl.takeFirst();                       // the IRC server the user is connected to
    this->nick=sl.takeFirst();                         // user's nickname
    this->userMode=sl.takeFirst();                     // user's mode
    this->hopCount=sl.takeFirst().remove(":").toInt(); // how much servers the user is away. 0 means connected locally. The first digit in the trailing part.

    bool rc; // return code

    /* parsing the flag */
    int flagNumber=sl.takeFirst().toInt(&rc);
    if (rc && flagNumber>=0) this->flag=flagNumber;
    else this->flag=49; // the red "q" flag with a question mark

    /* parsing the rank*/
    int rankNumber=sl.takeFirst().toInt(&rc);
    if(rc && rankNumber<singleton<pictureHandler>().ranklistsize() && rankNumber>=0)
        this->rank=rankNumber;
    else this->rank=12;

    /* parsing the additional country code (two letters) */
    QString countryString=sl.takeFirst();
    this->country=(flag<53 && flag!=49)? // if we understand the flag number, don't take the additional country string into account
                       singleton<pictureHandler>().mapNumberToCountryCode(flag):
                       countryString.isEmpty()? // if the country string is empty (space space in the input), set it to "??"
                           "??":
                           countryString;

    /* setting the client information to what is left in the input QStringList */
    this->client=sl.join(" ").remove("\r").remove("\n");

    /* WormNET returns the real, non-masked IP address only for the client itself. We take this address and use it globally */
    if(nick==singleton<netcoupler>().nick)
        singleton<netcoupler>().myip=this->address;
}
userstruct::userstruct(
     QString channel,
     QString clan,
     QString address,
     QString server,
     QString nick,
     QString userMode,
     int hopCount,
     int flag,
     int rank,
     QString country,
     QString client)
    :
     channel(channel),
     clan(clan),
     address(address),
     server(server),
     nick(nick),
     userMode(userMode),
     hopCount(hopCount),
     flag(flag),
     rank(rank),
     country(country),
     client(client){
}
QStringList userstruct::gamerWho() {
    QStringList gamerWho;
    gamerWho << channel << clan << address << server << nick << userMode
             << QString::number(hopCount) << QString::number(flag) << QString::number(rank)
             << country << client;
    return gamerWho;
}
userstruct userstruct::whoami(const QString &s, const QString &t) {
    userstruct u;
    u.nick = s;
    u.channel = t;
    return u;
}
bool userstruct::operator==(const userstruct &u) {
    if (u.nick.toLower() == nick.toLower() && (u.channel == "" || u.channel.toLower() == channel.toLower()))
        return true;
    else
        return false;
}
userstruct::~userstruct() {
    // TODO Auto-generated destructor stub
}
