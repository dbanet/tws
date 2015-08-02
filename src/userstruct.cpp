#include "userstruct.h"
#include "netcoupler.h"
#include "myDebug.h"
#include "global_functions.h"
#include "singleton.h"
#include "picturehandler.h"
#include <QPointer>

userstruct::userstruct // sl example: #AnythingGoes Username no.address.for.you wormnet1.team17.com Scal H :0 48 0 US The Wheat Snooper 2.9
    (netcoupler *netc,QStringList sl):netc(netc){
    Q_ASSERT_X(sl.size()>=7,"userstruct::userstruct(QStringList)",
               "WormNET server has returned a bad reply for a WHO request");

    this->channel=sl.takeFirst();                      // the channel the user is being on
    this->clan=sl.takeFirst().remove("~");             // username of the user (ident or ~nick)
    this->address=sl.takeFirst();                      // user's IP (typically no.address.for.you for wormnets)
    this->server=sl.takeFirst();                       // the IRC server the user is connected to
    this->nick=sl.takeFirst();                         // user's nickname
    this->userMode=sl.takeFirst();                     // user's mode
    this->hopCount=sl.takeFirst().remove(":").toInt(); // how much servers the user is away. 0 means connected locally. The first digit in the trailing part.

    bool rc; // return code

    /* parsing the flag */
    this->flag=49; // the red "q" flag with a question mark
    if(!sl.isEmpty()){
        int flagNumber=sl.takeFirst().toInt(&rc);
        if(rc && flagNumber>=0)
            this->flag=flagNumber;
    }

    /* parsing the rank*/
    this->rank=12; // the rank with three question marks
    if(!sl.isEmpty()){
        int rankNumber=sl.takeFirst().toInt(&rc);
        if(rc && rankNumber<singleton<pictureHandler>().ranklistsize() && rankNumber>=0)
            this->rank=rankNumber;
    }

    /* parsing the additional country code (two letters) */
    this->country="??";
    if(!sl.isEmpty()){
        QString countryString=sl.takeFirst();
        if(flag<53 && flag!=49) // if we understand the flag number, don't take the additional country string into account
            this->country=singleton<pictureHandler>().mapNumberToCountryCode(flag);
        else if(!countryString.isEmpty())
            this->country=countryString;;
    }

    /* setting the client information to what is left in the input QStringList */
    this->client=sl.join(" ").remove("\r").remove("\n");

    /* WormNET returns the real, non-masked IP address only for the client itself. We take this address and use it globally */
    if(nick==netc->nick && address!="no.address.for.you"){
        if(address.length()>34) // a W:A bug: if the game's IP/hostname length is > 34, W:A fails to display the host at all in the hostlist
            QHostInfo::lookupHost(address,&singleton<netcoupler>(),SLOT(lookedUpSnoopersIPAddress(QHostInfo))); // so we resolve the hostname to an IP address
        else
            netc->lookedUpSnoopersIPAddress(address);
    }
}
userstruct::userstruct(
     netcoupler *netc,
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
     netc(netc),
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
userstruct userstruct::whoami(netcoupler *netc,const QString &s, const QString &t) {
    userstruct u(netc);
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
