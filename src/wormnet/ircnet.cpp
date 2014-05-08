#include <QFile>
#include <QApplication>
#include <QDir>
#include <QTime>
#include <QMessageBox>
#include <QTextCodec>
#include <QSysInfo>

#include "netcoupler.h"
#include "singleton.h"
#include"ircnet.h"
#include"inihandlerclass.h"
#include"settings.h"
#include"settingswindow.h"
#include"about.h"
#include"codecselectdia.h"
#include"global_functions.h"
#include"myDebug.h"
#include"picturehandler.h"
#include"leagueserverhandler.h"
#include "ircmessage.h"
#include "singleton.h"
extern inihandlerclass inihandler;

ircnet::ircnet(QString s, QObject *parent) :
    QObject(parent), tcp(new QTcpSocket(this)) {
    nick = s;
    connect(tcp, SIGNAL(readyRead()),this, SLOT(tcpread()));
    whoreceivedcompletely = 1;
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
    s.append(S_S.getstring("clan") + " ");
    s.append(sl.takeFirst() + " ");
    s.append(sl.takeFirst() + " :");
    QString flag=S_S.getstring("countrycode").toUpper();
    s.append(singleton<picturehandler>().map_countrycode_to_number(flag)+ " ");   
    if(S_S.getbool("leaguestatecoloron") && S_S.getbool("enablesecurelogging"))
        s.append(S_S.getstring("leaguestatecolorname").remove('#')+" ");
    else {
        int i=S_S.getint("rank");
        s.append(QString::number(i) + " ");
    }
    s.append(flag+" ");
    s.append(S_S.getstring("information"));
    tcp_write(s);
    tcp_write("list");    
    // tcp_write("who"); Bcuz now TWS will issue WHO only on channel, and only when join it.
    emit sigconnected();
}
void ircnet::tcpread() {    //arrives like this msg\nmsg\n...\n...\n

    /* have no idea what actually happens in this define, but it somehow updates the userlist with              */
    /* QList of userstructs this->wholist. I don't know WTF is the second argument... ;)                        */
    #define updateuserlist singleton<netcoupler>().users.setuserstruct(this->wholist,QHash<QString,QStringList>())

    ircreadstring.append(CodecSelectDia::codec->toUnicode(tcp->readAll()));
    QStringList messages = ircreadstring.split("\n");
    ircreadstring = messages.takeLast(); // is "" or incomplete // bcuz the message always ends with \n,
                                                                // but if it doesn't, then the part after
                                                                // the last \n is not received completely,
                                                                // so save it for the next time. ~~dbanet
    foreach(QString msg,messages) {
        msg=msg.simplified();
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

            /**********************************/
            /*  startup/join thingies here... */
            /**********************************/
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
            "353"){
                // part of the /NAMES command. The server sends it automatically on channel join.
                // The command lists USERS on the channel by sending several numeric 353 messages.
                // Each message has exactly three arguments:
                // Arguments: your (snooper's) nick,
                //            the "equal" sign (=),
                //            the channel, that is being listed for users.
                //
                // The list of users on the channel (the second argument) is sent in the trailing
                // part of the message. The list is separated by spaces.
                //
                // The sequence of 353 messages ends with a message with numeric 366.
                // It denotes that all users has been listed.
                //
                // I parse this to fill the list of the users as soon as possible.
                // Later, the /WHO #channel command is issued, and the list is substituted by the
                // fully populated one, with flags, ranks, etc.                            ~~dbanet

                QStringList names=ircMsg->trailing.split(' ');
                foreach(QString nick,names)
                    this->wholist<<userstruct(QStringList()
                                              <<ircMsg->paramList[2]
                                              <<"Username"            /* these values do mean no-*/
                                              <<"no.address.for.you"  /* thing, are here just to */
                                              <<"wormnet1.team17.com" /* satisfy userstruct      */
                                              <<(nick[0]=='@'?
                                                 nick.remove(0,1):nick));   /* removing @ prefix */

            } else if(ircMsg->command==
            "366"){
                // end of the /NAMES command
                updateuserlist;

                /* Updating the channel window title with the newest amount of users...          */
                int amountOfUsers=0;
                QString channel=ircMsg->paramList[1];
                foreach(userstruct user,this->wholist)
                    if(user.chan==channel)
                        amountOfUsers++;
                emit sigIRCUpdatedAmountOfUsers(channel,channellist[channel]=amountOfUsers);
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
                for(int i=0;i<this->wholist.length();i++)
                    if(this->wholist[i].nick==ircMsg->paramList[5] &&
                       this->wholist[i].chan.toLower()==ircMsg->paramList[1].toLower()){
                        /* replacing the old userstruct with a fully populated one */
                        this->wholist[i]=userstruct(userstructSetupQSL);
                        foundAndUpdated=true;
                    }

                /* OOPS! If !foundAndUpdated, it seems that someone has joined on the channel    */
                /* AFTER the NAMES reply has been sent, but BEFORE the WHO reply has been sent.  */
                if(!foundAndUpdated){ /* So we just add a new userstruct to the end...           */
                    this->wholist<<userstruct(userstructSetupQSL);
                }
            } else if(ircMsg->command==
            "315"){
                // end of the /WHO command
                updateuserlist;
            } else if(ircMsg->command==

            /*******************************************************/
            /* updating the userlist on quit, part, kick, join...  */
            /*******************************************************/
            "QUIT"){
                QString nick=ircMsg->prefix.split("!")[0];
                QStringList channelsTheQuittedUserWasBeingOn;
                for(QList<userstruct>::iterator i=this->wholist.begin();i!=this->wholist.end();)
                    if(i->nick==nick){
                        channelsTheQuittedUserWasBeingOn<<i->chan;
                        i=this->wholist.erase(i);
                    } else ++i;
                updateuserlist;

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u("QUIT :"+ircMsg->trailing /* reason */,
                              usermessage_type(e_GARBAGE | e_GARBAGEQUIT),
                              nick,
                              channelsTheQuittedUserWasBeingOn.join(",")
                );
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                foreach(QString channel,channelsTheQuittedUserWasBeingOn)
                    emit sigIRCUpdatedAmountOfUsers(channel,--channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==
            "PART"){
                QString nick=ircMsg->prefix.split("!")[0];
                QString channel=ircMsg->paramList[0];
                for(QList<userstruct>::iterator i=this->wholist.begin();i<this->wholist.end();)
                    if(i->nick==nick &&
                       i->chan.toLower()==channel.toLower())
                        i=this->wholist.erase(i);
                    else ++i;
                updateuserlist;

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u("PART "+channel+" :"+ircMsg->trailing,
                              usermessage_type(e_GARBAGE | e_GARBAGEPART),nick,channel);
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                emit sigIRCUpdatedAmountOfUsers(channel,--channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==
            "JOIN"){
                QString nick=ircMsg->prefix.split("!")[0];
                QString channel=ircMsg->trailing;
                if(nick==this->nick) continue; /* We already know we've joined a channel ;)    */
                this->wholist<<userstruct(QStringList()
                                          <<channel
                                          <<"Username"            /* these values do mean no-*/
                                          <<"no.address.for.you"  /* thing, are here just to */
                                          <<"wormnet1.team17.com" /* satisfy userstruct      */
                                          <<nick);
                updateuserlist;
                tcp_write("WHO "+nick+"\n");

                /* Lookias code. Write in chatwindow, store in history, etc. */
                usermessage u("JOIN "+channel,
                              usermessage_type(e_GARBAGE | e_GARBAGEJOIN),nick,channel);
                u.settime(time());
                appendhistory(u);
                emit siggotusermessage(u);
                emit sigIRCUpdatedAmountOfUsers(channel,++channellist[this->canonizeChannelName(channel)]);
            } else if(ircMsg->command==

            /*******************************************************/
            /*                    other shiet                      */
            /*******************************************************/
            "332"){ /*   TOPIC. TWS doesn't use it anyhow yet.  */ } else if(ircMsg->command==
            "333"){ /* This one is not declared in RFC, however */ } else if(ircMsg->command==
                    /* WormNet sends it just after the TOPIC.   */
            "001"){
                /* Welcome message. Sent by server automatically on connect.  */
                /* Here I output it decently to the main window.     ~~dbanet */
                myDebug()<<ircMsg->trailing;
            } else if(ircMsg->command==
            "002"){
                /* Host information. Sent by server automatically on connect.  */
                /* Here I output it decently to the main window.      ~~dbanet */
                myDebug()<<ircMsg->trailing;
            } else if(ircMsg->command==
            "376" || "372" || "375" || "250" || "260" || "265" || "255" || "254" || "252" || "251" || "005"){
                /* Other various text messages just to be displayed to the user  */
                /* upon connection I'm tired to describe separately.             */
                /* Here I output it to the main window.                ~~dbanet  */
                myDebug()<<ircMsg->trailing;
            } else if(ircMsg->command==
            "001"){
            } else if(ircMsg->command==
            "004"){
                /* Server info. Sent by server automatically on connect.  */
                /* Here I output it decently to the main window. ~~dbanet */
                myDebug()<<"Server Info: "<<ircMsg->paramList.join(" ").section(" ",1);
            } else if(ircMsg->command==
            "401"){
                /* this is sent by the server if the recepient of a PRIVMSG/NOTICE/QUERY doesn't */
                /* exist. The zero argument is the not existing channel/nick itself.             */
                emit signosuchnick(ircMsg->paramList[0]);
            } else if(ircMsg->command==
            "433"){
                /* this is sent by the server when a NICK message is processed that results in   */
                /* an attempt to change to a currently existing nickname.                        */

                /* I have no idea what's below, but should work as expected. Copypasted from     */
                /* void ircnet::readusermessage(QString).                                        */

                if(S_S.getbool("enablesecurelogging"))
                    QMessageBox::information(0,tr("Nickname collision!"),
                                             tr("Your nickname is already in use. You can wait some minutes or click on the profile button in secure logging section to change your nickname and try again."),
                                             QMessageBox::Ok);
                else
                    QMessageBox::information(0,tr("Nickname collision!"),
                                             tr("Your nickname is already in use. You can wait some minutes or change your nickname and try again."),
                                             QMessageBox::Ok);
                quit("");
            }
            else /* FINALLY! */ myDebug()<<"The server has sent a message TWS is unable to handle. Please open an issue: \n"
                                         <<"  https://github.com/dbanet/tws/issues\n"
                                         <<"Received message: "<<ircMsg->getFancy()+" || RAW: "+ircMsg->getRaw();
        }
    }
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

void ircnet::disconnected() {                   
    myDebug() << tr("disconnected from irc server.");
    emit sigdisconnected();
}
void ircnet::joinchannel(const QString &chan) {
    tcp_write("JOIN "+chan       +"\n"+
              "WHO " +chan       +"\n");
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
void ircnet::who() {
    if (whoreceivedcompletely) {
        templist.clear();
        tcp_write("who");
        whoreceivedcompletely = 0;
    }
}
void ircnet::quit(QString s){
    tcp_write("QUIT : [The Wheat Snooper] "+ s);
}
void ircnet::tcp_write(const QString &msg){
    tcp->write(CodecSelectDia::codec->fromUnicode(msg)+"\n");
}
int ircnet::state() const{
    return tcp->state();
}

