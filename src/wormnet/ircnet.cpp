#include <QFile>
#include <QApplication>
#include <QDir>
#include <QTime>
#include <QMessageBox>
#include <QTextCodec>
#include <QSysInfo>

#include "netcoupler.h"
#include "singleton.h"
#include "ircnet.h"
#include "inihandlerclass.h"
#include "settings.h"
#include "settingswindow.h"
#include "about.h"
#include "codecselectdia.h"
#include "global_functions.h"
#include "myDebug.h"
#include "picturehandler.h"
#include "leagueserverhandler.h"
#include "ircmessage.h"
#include "singleton.h"
extern inihandlerclass inihandler;

ircnet::ircnet(QString s, QObject *parent) :
    QObject(parent),tcp(new QTcpSocket(this)){
    nick=s;
    this->disconnectionTimeout=new QTimer();
    connect(tcp,SIGNAL(readyRead()),this,SLOT(tcpread()));
    connect(disconnectionTimeout,SIGNAL(timeout()),this,SLOT(disconnectionTimedOut()));
    disconnectionTimeout->setSingleShot(true);
}
void ircnet::setip(const QString &ip) {
    wnip = ip;
}
bool firstMessageArrived=false;
void ircnet::start() {
    firstMessageArrived=false;
    connect(tcp, SIGNAL(connected()),this, SLOT(connected()));
    connect(tcp, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(tcpError(QAbstractSocket::SocketError)));
    tcp->connectToHost(wnip, 6667, QIODevice::ReadWrite);
}
void ircnet::reconnect(){
    myDebug()<<tr("Reconnecting");
    tcp->connectToHost(wnip, 6667, QIODevice::ReadWrite);
}
void ircnet::tcpError(QAbstractSocket::SocketError s){
    if(s==QAbstractSocket::RemoteHostClosedError)
        return;
    myDebug()<<tr("There was an error with the connection to Wormnet.");
    emit sigdisconnected ();
}
void ircnet::connected() {
    QStringList sl = inihandler.stringlistfromini("[irc password]");
    if (!sl.isEmpty()) {
        if (sl.first() == "PASS standard") {
            if(wnip == "itakagames.spb.ru")
                tcp_write("PASS ELSILRACLIHP ");
            else
                tcp_write("PASS ELSILRACLIHP");
        } else
            tcp_write(sl.first());
    }
    tcp_write("NICK " + nick);
    QString s = inihandler.stringlistfromini("[irc register]").first();    
    sl = s.split(" ", QString::SkipEmptyParts);
    s = sl.takeFirst() + "  ";
    s.append(S_S.getString("clan") + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    QString flag=S_S.getString("countrycode").toUpper();
    s.append(singleton<pictureHandler>().mapCountryCodeToNumber(flag)+ " ");
    if(S_S.getbool("leaguestatecoloron") && S_S.getbool("enablesecurelogging"))
        s.append(S_S.getString("leaguestatecolorname").remove('#')+" ");
    else {
        int i=S_S.getint("rank");
        s.append(QString::number(i) + " ");
    }
    s.append(flag+" ");
    s.append(S_S.getString("information"));
    tcp_write(s);
    tcp_write("LIST");
    tcp_write("WHO"); // wanna get non-channelled users
    emit sigconnected();
}
void ircnet::tcpread() {    //arrives like this msg\nmsg\n...\n...\n
    #define updateuserlist emit sigIRCUpdatedUserList(&this->userList);

    ircreadstring.append(CodecSelectDia::codec->toUnicode(tcp->readAll()));
    QStringList messages = ircreadstring.split("\n");
    ircreadstring = messages.takeLast(); // is "" or incomplete // bcuz the message always ends with \n,
                                                                // but if it doesn't, then the part after
                                                                // the last \n is not received completely,
                                                                // so save it for the next time. ~~dbanet
    foreach(QString msg,messages){
        ircMessage *ircMsg;
        try{ ircMsg=new ircMessage(msg); }
        catch(...){ continue; }
        if(ircMsg->isServMsg()){
            if(ircMsg->command==
            "PING"){
                tcp_write("PONG :"+ircMsg->trailing);
            } else if(ircMsg->command==
            "ERROR"){
                myDebug()<<ircMsg->getRaw();
            }
        }
        if(ircMsg->isUserMsg()){
            if(ircMsg->command==
            "PRIVMSG"){
                QString from=ircMsg->prefix.split("!")[0];
                QString to  =ircMsg->paramList[0];
                QString text=ircMsg->trailing;
                if(text==QString()+(char)0x01+"VERSION"+(char)0x01){
                    tcp_write("NOTICE "+from+" :"+(char)0x01+"VERSION The Wheat Snooper "+TWS_VERSION+(char)0x01);
                    return;
                }
                emit siggotusermessage(usermessage(text,e_PRIVMSG,from,to));
            } else if(ircMsg->command==
            "NOTICE"){
                QString from=ircMsg->prefix.split("!")[0];
                QString to  =ircMsg->paramList[0];
                QString text=ircMsg->trailing;
                emit siggotusermessage(usermessage(text,e_NOTICE,from,to));
            } else if(ircMsg->command==
            "MODE"){
                QString from     =ircMsg->prefix.split("!")[0];
                QString toChannel=ircMsg->paramList[0];
                QChar side       =ircMsg->paramList[1][0];
                QChar mode       =ircMsg->paramList[1][1];
                QString toUser   =ircMsg->paramList[2];
                if (mode == 'o')
                    if (side == '+')
                        myDebug()<<tr("%1 gives channel operator status to %2").arg(from, toUser);
                    else
                        myDebug()<<tr("%1 removes channel operator status from %2").arg(from, toUser);
                else
                    myDebug()<<tr("%1 sets mode %2%3 on %4 %5").arg(from, side, mode, toChannel, toUser);
            } else if(ircMsg->command==
            "TOPIC"){
                QString nick=ircMsg->prefix.split("!")[0];
                QString newTopic=ircMsg->trailing;
                myDebug()<<tr("%1 has changed the topic to \"%2\"").arg(nick,newTopic);
            }
            
            /*******************************************************/
            /* updating the userlist on quit, part, kick, join...  */
            /*******************************************************/
            else if(ircMsg->command==
            "JOIN"){
                QString nick=ircMsg->prefix.split("!")[0];
                QString channel=ircMsg->trailing;
                if(nick==this->nick) continue; /* We already know we've joined a channel ;)    */
                this->userList.prepend(
                    userstruct(
                        channel,
                        "Username",            /* these values do mean no-*/
                        "no.address.for.you",  /* thing, are here just to */
                        "wormnet1.team17.com", /* satisfy userstruct      */
                        nick
                    )
                );

                updateuserlist;
                tcp_write("WHO "+nick+"\n");

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u(tr("has joined %1").arg(channel),
                              usermessage_type(e_GARBAGE | e_GARBAGEJOIN),nick,channel);
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                emit sigIRCUpdatedAmountOfUsers(channel,++channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==
            "PART"){
                QString nick=ircMsg->prefix.split("!")[0];
                QString channel=ircMsg->paramList[0];
                for(QList<userstruct>::iterator i=this->userList.begin();i<this->userList.end();)
                    if(i->nick==nick &&
                       i->channel.toLower()==channel.toLower())
                        i=this->userList.erase(i);
                    else ++i;
                updateuserlist;

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u(ircMsg->trailing.isEmpty() ?
                              tr("has left %1").arg(channel) :
                              tr("has left %1 (%2)").arg(channel, ircMsg->trailing),
                              usermessage_type(e_GARBAGE | e_GARBAGEPART),nick,channel);
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                emit sigIRCUpdatedAmountOfUsers(channel,--channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==
            "KICK"){
                QString master=ircMsg->prefix.split("!")[0];
                QString nick=ircMsg->paramList[1];
                QString channel=ircMsg->paramList[0];
                for(QList<userstruct>::iterator i=this->userList.begin();i<this->userList.end();)
                    if(i->nick==nick &&
                       i->channel.toLower()==channel.toLower())
                        i=this->userList.erase(i);
                    else ++i;
                updateuserlist;

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u(tr("has been kicked from %1 by %2 (%3)").arg(channel, master, ircMsg->trailing),
                              usermessage_type(e_GARBAGE | e_GARBAGEPART),nick,channel);
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                emit sigIRCUpdatedAmountOfUsers(channel,--channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==
            "QUIT"){
                QString nick=ircMsg->prefix.split("!")[0];
                QStringList channelsTheQuittedUserWasBeingOn;
                for(QList<userstruct>::iterator i=this->userList.begin();i!=this->userList.end();)
                    if(i->nick==nick){
                        channelsTheQuittedUserWasBeingOn<<i->channel;
                        i=this->userList.erase(i);
                    } else ++i;
                updateuserlist;

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u(tr("has quit (%1)").arg(ircMsg->trailing), /* reason */
                              usermessage_type(e_GARBAGE | e_GARBAGEQUIT),
                              nick,
                              channelsTheQuittedUserWasBeingOn.join(",")
                );
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                foreach(QString channel,channelsTheQuittedUserWasBeingOn)
                    emit sigIRCUpdatedAmountOfUsers(channel,--channellist[this->canonizeChannelName(channel)]);
            }
            
            /**********************************/
            /*  startup/join thingies here... */
            /**********************************/
            else if(ircMsg->command==
            "315"){
                // end of the /WHO command
                updateuserlist;
            } else if(ircMsg->command==
            "321"){ /* this command designates the start of the LIST command reply, see right below */ } else if(ircMsg->command==
            "322"){
                // part of the /LIST command. TWS issues it on connect to get the channel list.
                // The command lists the CHANNELS on the network by sending them one by one
                // with numeric 322. The channel name is in the first argument (the zero
                // argument is your nick). The channel listing ends with the numeric 322.  ~~dbanet

                            /* channel  name  */  /* amount of users */ // well, it will be updated
                channellist[ircMsg->paramList[1]]=ircMsg->paramList[2].toInt();    // l8r on /NAMES
            } else if(ircMsg->command==
            "323"){
                // end of the channel listing
                foreach(QString channel,channellist.keys())
                    if(!singleton<netcoupler>().users.usermap_channellist_helper.contains(channel,Qt::CaseInsensitive))
                        singleton<netcoupler>().users.usermap_channellist_helper.push_back(channel);

                emit sigIRCReceivedChanList(QStringList(channellist.keys()));
            } else if(ircMsg->command==
            "352"){
                // part of the /WHO command. TWS issues it for each channel it joins on to get the
                // additional info (realname) about the users, which contain their flags and ranks.
                // The command lists USERS on the channel by sending a 352 message for each user,
                // one by one, with the realname in the trailing, and the following in the args:
                // Arguments: your (snooper's) nick,
                //            channel the user is currently being on (or * if none),
                //            the username of the user (ident or ~nick)
                //            the user's host,
                //            the server the user is currently being on,
                //            the nick of the user,
                //            the mode of the user.
                //
                // The sequence of 352 messages ends with a message with numeric 315.
                //
                // The userstruct::userstruct(QStringList) takes a QStringList of >=FIVE QStrings:
                // channel, uname, uhost, server, nick. For some reason Lookie in this function
                // calls the username as "clan", and uhost as "address". The last is right, but I'm
                // not sure about the first...                                             ~~dbanet
                //
                // Oh, and also it takes FIVE ADDITIONAL fields -- the realname (description) of
                // the user, splitted by spaces. WormNet places the flag, rank, etc. information
                // there. So, userstruct takes a QStringList of TEN QStrings...            ~~dbanet

                /* Forming the needed QStringList... */
                QStringList userstructSetupQSL(ircMsg->paramList);
                userstructSetupQSL[5]=userstructSetupQSL[5][0]=='@'? /* @ prefix of the nick     */
                            userstructSetupQSL[5].remove(0,1)        /* should be removed        */
                           :userstructSetupQSL[5];
                userstructSetupQSL.
                     removeFirst(); /* the snooper's nick. Shouldn't be supplied to userstruct() */
                userstructSetupQSL<<ircMsg->trailing.split(' ');     /* adding the nickrang info */

                /* Now we iterate through all userstructs with matching nick, and replacing them */
                /* with an updated userstruct filled with the rankflag information...            */
                bool foundAndUpdated=false;
                for(int i=0;i<this->userList.length();i++)
                    if(this->userList[i].nick==ircMsg->paramList[5] &&
                       this->userList[i].channel.toLower()==ircMsg->paramList[1].toLower()){
                        /* replacing the old userstruct with a fully populated one */
                        this->userList[i]=userstruct(userstructSetupQSL);
                        foundAndUpdated=true;
                    }

                /* OOPS! If !foundAndUpdated, it seems that someone has joined on the channel    */
                /* AFTER the NAMES reply has been sent, but BEFORE the WHO reply has been sent.  */
                if(!foundAndUpdated){ /* So we just prepend a new userstruct...                  */
                    this->userList.prepend(userstruct(userstructSetupQSL));
                }
            } else if(ircMsg->command==
            "353"){
                // part of the /NAMES command. The server sends it automatically on channel join.
                // The command lists USERS on the channel by sending several numeric 353 messages.
                // Each message has exactly three arguments:
                // Arguments: your (snooper's) nick,
                //            the "equal" sign (=),
                //            the channel, that is being listed for users,
                //            OR, IF LISTING USERS NOT JOINED TO ANY CHANNEL,
                //            your (snooper's) nick,
                //            the "asterisk" sign (*),
                //            the "astersik" sign (*).
                //
                // The list of users on the channel (the second argument) is sent in the trailing
                // part of the message. The list is separated by spaces.
                //
                // The sequence of 353 messages ends with a message with numeric 366.
                // It denotes that all users has been listed.
                //
                // I parse this to fill the list of the users as soon as possible.
                // Later the /WHO #channel command is issued, and the list is substituted by the
                // fully populated one, with flags, ranks, etc.                            ~~dbanet

                QStringList names=ircMsg->trailing.split(' ');
                foreach(QString nick,names){
                    userstruct *newUser=
                                new userstruct(ircMsg->paramList[2],
                                               "Username",            /* these values do mean no-*/
                                               "no.address.for.you",  /* thing, are here just to */
                                               "wormnet1.team17.com", /* satisfy userstruct      */
                                               (nick[0]=='@'?
                                                nick.remove(0,1):nick));    /* removing @ prefix */
                    /* If a user with such a nick and a channel already exists (for example, the */
                    /* NAMES command was issued manually not in order), do nothing. No real need */
                    /* to check if our userList contains a user not listed in the NAMES reply,   */
                    /* cuz we track all quits, kicks, bans, et cetera, anyway. This check is     */
                    /* just be sure the NAMES command was not issued manually.          ~~dbanet */
                    bool userAlreadyExists=false;
                    foreach(userstruct user,this->userList)
                        if(user.nick==newUser->nick && user.channel==newUser->channel)
                            userAlreadyExists=true;
                    if(!userAlreadyExists)
                        this->userList<<*newUser;
                }
            } else if(ircMsg->command==
            "366"){
                // end of the /NAMES command
                updateuserlist;

                /* Updating the channel window title with the newest amount of users...          */
                int amountOfUsers=0;
                QString channel=ircMsg->paramList[1];
                foreach(userstruct user,this->userList)
                    if(user.channel==channel)
                        amountOfUsers++;
                emit sigIRCUpdatedAmountOfUsers(channel,channellist[channel]=amountOfUsers);
            } else if(ircMsg->command==
             
            /*******************************************************/
            /*                    other shiet                      */
            /*******************************************************/
            "004"){
                /* Server info. Sent by server automatically on connect.  */
                /* Here I output it decently to the main window. ~~dbanet */
                myDebug()<<tr("Server Info: %1").arg(ircMsg->paramList.join(" ").section(" ",1));
            } else if(
            ircMsg->command=="001" || //Welcome message
            ircMsg->command=="002" || //Host information
            ircMsg->command=="003" || //Server was created at [date]
            ircMsg->command=="005" || //RPL_BOUNCE
            ircMsg->command=="250" || //RPL_STATSDLINE
            ircMsg->command=="251" || //Amount of visible and invisible online users
            ircMsg->command=="252" || //Amount of IRC ops
            ircMsg->command=="253" || //Amount of unknown connections
            ircMsg->command=="254" || //Amount of channels
            ircMsg->command=="255" || //Amount of local connections
            ircMsg->command=="256" || //Start of ADMIN reply
            ircMsg->command=="257" || //Part of an ADMIN reply: location 1
            ircMsg->command=="258" || //Part of an ADMIN reply: location 2
            ircMsg->command=="259" || //Part of an ADMIN reply: e-mail
            ircMsg->command=="265" || //Amount of local users
            ircMsg->command=="266" || //Amount of global users
            ircMsg->command=="365" || //End of LINKS list
            ircMsg->command=="371" || //INFO reply
            ircMsg->command=="372" || //MOTD reply
            ircMsg->command=="373" || //INFO start
            ircMsg->command=="374" || //End of INFO list
            ircMsg->command=="375" || //MOTD start
            ircMsg->command=="376" ){ //End of MOTD list
                /* Other various text messages just to be displayed to the user. */
                /* Here I output it to the main window.                ~~dbanet  */
                myDebug()<<ircMsg->trailing;
            } else if(
            ircMsg->command=="303" || //ISON reply
            ircMsg->command=="311" || //Part of a WHOIS reply: user info
            ircMsg->command=="312" || //Part of a WHOIS reply: user server
            ircMsg->command=="313" || //Part of a WHOIS reply: user privileges
            ircMsg->command=="314" || //Part of a WHOWAS reply: user info
            ircMsg->command=="318" || //End of WHOIS
            ircMsg->command=="319" || //Part of a WHOIS reply: channels
            ircMsg->command=="320" || //Part of a WHOIS reply
            ircMsg->command=="332" || //TOPIC response
            ircMsg->command=="333" || //Who set the topic at which time
            ircMsg->command=="367" || //Banlist entry
            ircMsg->command=="368" || //End of banlist
            ircMsg->command=="369" ){ //End of WHOWAS
                /* Ignoring these since there's no use for them currently. */
            } else if(ircMsg->command==
            "317"){
                /* Idle info. Sent by the server as a part of the reply to the /WHOIS command.   */
                QString whoisSubject=ircMsg->paramList[1];
                int idlePeriod=ircMsg->paramList[2].toInt();
                int logonTime=ircMsg->paramList[3].toInt();
                emit siggotidletime(whoisSubject,idlePeriod,logonTime);
            } else if(ircMsg->command==
            "364"){ /* Reply to the LINKS command */
                myDebug()<<QString(ircMsg->paramList.join(" ").section(" ",1)).append(" :").append(ircMsg->trailing);
            } else if(ircMsg->command==
            "391"){
                /* Server time. Sent by requesting /TIME from the server.  */
                myDebug()<<tr("Server time: %1").arg(ircMsg->trailing);
            } else if(ircMsg->command==
            "401"){
                /* this is sent by the server if the recepient of a PRIVMSG/NOTICE/QUERY doesn't */
                /* exist. The zero argument is the not existing channel/nick itself.             */
                emit signosuchnick(ircMsg->paramList[0]);
            } else if(
            // Various errors
            ircMsg->command=="402" || // No such server
            ircMsg->command=="403" || // No such channel
            ircMsg->command=="404" || // Cannot send to channel
            ircMsg->command=="405" || // Too many channels
            ircMsg->command=="406" || // There was no such nick
            ircMsg->command=="407" || // Too many targets
            ircMsg->command=="408" || // No such service
            ircMsg->command=="409" || // No origin
            ircMsg->command=="411" || // No recipient
            ircMsg->command=="412" || // No text to send
            ircMsg->command=="421" || // Unknown command
            ircMsg->command=="451" ){ // Unregistered
                myDebug()<<QString("%1 %2 - %3").arg(tr("Server Info:"), ircMsg->paramList[1], ircMsg->trailing);
            } else if(ircMsg->command==
            "433"){
                /* this is sent by the server when a NICK message is processed that results in   */
                /* an attempt to change to a currently existing nickname.                        */

                if(S_S.getbool("enablesecurelogging"))
                    QMessageBox::information(0,tr("Nickname collision!"),
                                             tr("Your nickname is already in use. You can wait some minutes or click on the profile button in secure logging section to change your nickname and try again."),
                                             QMessageBox::Ok);
                else
                    QMessageBox::information(0,tr("Nickname collision!"),
                                             tr("Your nickname is already in use. You can wait some minutes or change your nickname and try again."),
                                             QMessageBox::Ok);
                quit("");
            } else if(ircMsg->command==
            "471"){
                myDebug()<<tr("Channel %1 is full: %2").arg(ircMsg->paramList[1], ircMsg->trailing);
            } else if(ircMsg->command==
            "473"){
                myDebug()<<tr("Channel %1 is invite-only: %2").arg(ircMsg->paramList[1], ircMsg->trailing);
            } else if(ircMsg->command==
            "474"){
                myDebug()<<tr("You are banned from talking in %1: %2").arg(ircMsg->paramList[1], ircMsg->trailing);
            } else if(ircMsg->command==
            "475"){
                myDebug()<<tr("Incorrect password for %1: %2").arg(ircMsg->paramList[1], ircMsg->trailing);
            } else if(ircMsg->command==
            "476"){
                myDebug()<<tr("Invalid channel mask for %1: %2").arg(ircMsg->paramList[1], ircMsg->trailing);
            }
            else /* FINALLY! */ myDebug()<<"The server has sent a message TWS is unable to handle. Please open an issue: \n"
                                         <<"  https://github.com/dbanet/tws/issues\n"
                                         <<"Received message: "<<ircMsg->getFancy()+" || RAW: "+ircMsg->getRaw();
        }
    }
    #undef updateuserlist
}
QString ircnet::canonizeChannelName(QString channelArg){
    foreach(QString channel,this->channellist.keys())
        if(channel.toLower()==channelArg.toLower().simplified())
            return channel;
    return channelArg.simplified();
}
/********************************************************/
/*          THESE FUNCTIONS ARE NOT IN USE              */
/*  THESE FUNCTIONS ARE LEFT HERE FOR INFORMATION ONLY  */
/*********************************************************
void ircnet::readservermassege(QString s) {
    static bool b=false;
    QStringList sl = s.split(" ");
    if(sl.size()<3){
        sl.clear();
        return;
    }
    int command = sl.first().toInt(&b);
    if(!b)
        if(sl.first() != "NOTICE")
            myDebug()<<sl<<"|"+servermessageindicator+"|";
    sl.removeFirst();
    sl.removeFirst();
    QString channel;
    switch (command) {
    case 317: //317 loOkias`twsnp ```MihaiS`sW` 57 1231682995 :seconds idle, signon time
        if (sl.size() >= 2) {
            QString s = sl.takeFirst();
            int i = sl.first().toInt();
            emit siggotidletime(s, i);
        }
        break;
    }
}
void ircnet::readusermessage(QString &s) {
    QStringList sl = s.split(" ");
    Q_ASSERT(sl.size()>=3);
    QString user = sl.takeFirst().split("!").first().remove(":");
    QString garbage = sl.join(" ").remove("\r").remove("\n");
    QString command = sl.takeFirst();
    QString receiver = sl.takeFirst().remove("\r").remove(":");


    ///////////////
    //PROBBLY THIS IS NOT ONLY USER-TO-USER NOTICE, BUT SERVER-TO-USER,//
    //ETC, U-2-U IS REIMPL. NOW ONLY, SO CHECK THIS L8R!       ~~dbanet//
                                                          ///////////////
    if(command=="NOTICE"){
        QString s=sl.join(" ").remove(0, 1);
        emit siggotusermessage(usermessage(s, e_NOTICE, user, receiver));
    } else
        myDebug() << tr("Servermessage: ") << s;
}
*********************************************************/

void ircnet::disconnected(){
    disconnectionTimeout->stop();
    myDebug()<<tr("disconnected from irc server.");
    emit sigdisconnected();
}
void ircnet::joinchannel(const QString &chan){
    /* removing everyone whose channel is the one we're joining from userList */
    for(QList<userstruct>::iterator it=userList.begin();it!=userList.end();)
        if(it->channel==chan)
            it=userList.erase(it);
        else
            ++it;

    tcp_write("JOIN "+chan+"\n"+
               "WHO "+chan+"\n");
    this->joinedchannellist<<this->canonizeChannelName(chan);
    emit sigIRCJoinedChannel(this->canonizeChannelName(chan),channellist[this->canonizeChannelName(chan)]);
}
void ircnet::partchannel(const QString &chan) {
    tcp_write("PART "+chan+"\n");
}
void ircnet::sendusermessage(const usermessage u){
    if(u.has_type(e_RAWCOMMAND))
        tcp_write(u.msg());
    else if(u.has_type(e_CTCP))
        tcp_write("PRIVMSG " + u.receiver() + " :\001" + u.msg() + "\001");
    else if(u.has_type(e_PRIVMSG)){
        if(u.has_type(e_ACTION))
            tcp_write("PRIVMSG " + u.receiver() + " :\001ACTION " + u.msg() + " \001");
        else
            tcp_write("PRIVMSG " + u.receiver() + " :" + u.msg());
    }
    else if (u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION))
            tcp_write("NOTICE " + u.receiver() + " :\001ACTION " + u.msg() + " \001");
        else
            tcp_write("NOTICE " + u.receiver() + " :" + u.msg());
    }
    else
        myDebug()<<QString() + "##################void ircnet::sendusermessage(const usermessage u)";
}
void ircnet::refreshlist() {
    if (justgetlist == false) {
        tcp_write("list");
        justgetlist = true;
    }
}
void ircnet::who(){
    tcp_write("who");
}
void ircnet::quit(QString reason){
    tcp_write(QString()+"QUIT : ["+TWS_VERSION+"] "+reason);
    myDebug()<<"Waiting for the server to gracefully disconnect...";
    disconnectionTimeout->start(5000);
}
void ircnet::disconnectionTimedOut(){
    myDebug()<<"disconnection timeout, forcing...";
    tcp->abort();
}
void ircnet::tcp_write(const QString &msg){
    tcp->write(CodecSelectDia::codec->fromUnicode(msg)+"\n");
}
int ircnet::state() const{
    return tcp->state();
}

