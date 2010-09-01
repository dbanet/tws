#include<QFile>
#include<QClipboard>
#include<QTime>
#include<QMessageBox>
#include"netcoupler.h"
#include"ircnet.h"
#include"snoppanet.h"
#include"inihandlerclass.h"
#include"snpsettings.h"
#include"window.h"
#include"joinprvgame.h"
#include"settingswindow.h"
#include"volumeslider.h"
#include"mainwindow.h"
#include"playername.h"
#include"sound_handler.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"global_functions.h"
#include"balloon_handler.h"
extern volumeslider *volume;
extern inihandlerclass inihandler;
QProcess *netcoupler::p= new QProcess;
netcoupler::netcoupler(QString s, QObject *parent) :
        QObject(parent), nick(s) {

    connect(volume, SIGNAL(valueChanged (int)),&singleton<sound_handler>(), SLOT(volumechange(int)));

    irc = new ircnet(nick, this);
    connect(irc, SIGNAL(sigusergarbage(const QString&,const QString&)),this, SIGNAL(sigusergarbage(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagejoin(const QString&,const QString&)),this, SIGNAL(sigusergarbagejoin(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagepart(const QString&,const QString&)),this, SIGNAL(sigusergarbagepart(const QString&,const QString&)));
    connect(irc, SIGNAL(sigusergarbagequit(const QString&,const QString&)),this, SIGNAL(sigusergarbagequit(const QString&,const QString&)));
    connect(irc,SIGNAL(sigconnected()),this,SLOT(ircconnected()));
    connect(irc,SIGNAL(sigdisconnected()),this,SLOT(ircdiconnected()));
    http = new snoppanet(this);
    connect(http, SIGNAL(sigchannelscheme(QString,QString)),this, SLOT(getscheme(QString,QString)));
    connect(irc, SIGNAL(siggetlist(QStringList)),this, SLOT(getchannellist(QStringList)));
    connect(irc, SIGNAL(sigmsg(const QString&,const QString&,const QString&)),this, SLOT(getmsg(const QString&,const QString&,const QString&)));
    connect(irc, SIGNAL(signotice(const QString&,const QString&,const QString&)),this, SLOT(getnotice(const QString&,const QString&,const QString&)));
    connect(irc, SIGNAL(siggotidletime(const QString&, int)),this, SIGNAL(siggotidletime(const QString&, int)));
    connect(irc, SIGNAL(signosuchnick(const QString&)),this, SIGNAL(signosuchnick(const QString&)));
    connect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlist(QList<hoststruct>,QString)));
    connect(&users, SIGNAL(sigbuddyarrived()),&singleton<balloon_handler>(), SLOT(buddyarrived()));
    connect(&users, SIGNAL(sigbuddyleft()),&singleton<balloon_handler>(), SLOT(buddyleft()));
    connect(this,SIGNAL(sigconnected()),&singleton<balloon_handler>(),SLOT(connected()));
    connect(this,SIGNAL(sigdisconnected()),&singleton<balloon_handler>(),SLOT(disconnected()));
    QStringList sl = inihandler.stringlistfromini("[irc ip]");
    if (sl.isEmpty()) {
        connect(http, SIGNAL(sigircip(QString)),this, SLOT(getircip(QString)));
        http->start();
    } else {
        ircip = sl.first();
        irc->setip(ircip);
        irc->start();
        usesettingswindow("sbwhorepead");
    }
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),this, SLOT(sendquit()));
    p->setProcessChannelMode(QProcess::MergedChannels);
    connect(p, SIGNAL(finished (int , QProcess::ExitStatus)),this, SLOT(processfinished(int , QProcess::ExitStatus)));
    connect(p, SIGNAL(readyRead()),this, SLOT(readprocess()));
    isaway = 0;
    usesettingswindow("cbsetawaywhilegaming");
    usesettingswindow("leawaystring");

    mutedusers = singleton<snpsettings>().map["mutedusers"].toStringList();
    connect(this, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(this, SIGNAL(sigsettingswindowchanged()),&users, SLOT(usesettingswindow()));
}
netcoupler::~netcoupler() {
    userstruct::addressischecked=0;
    irc->quit();
    irc->deleteLater();
    http->deleteLater();
}
void netcoupler::joinchannel(const QString &s) {
    irc->joinchannel(s);
    http->getscheme(s);
    if (!listofjoinedchannels.contains(s))
        listofjoinedchannels << s;
    http->setchannellist(listofjoinedchannels);
}
void netcoupler::partchannel(const QString &s) {
    irc->partchannel(s);
    listofjoinedchannels.removeOne(s);
    http->setchannellist(listofjoinedchannels);
}
void netcoupler::getchannellist(QStringList sl) {
    channellist.clear();
    foreach(QString s,sl) {
        channellist << s.split(" ").first();
    }
    foreach(QString s, channellist) {
        if(!users.usermap_channellist_helper.contains(s))
            users.usermap_channellist_helper.push_back(s);
    }
    emit siggotchanellist(sl);
}
void netcoupler::getircip(QString s) {
    irc->setip(s);
    irc->start();
    usesettingswindow("sbwhorepead");
}
void netcoupler::gethostlist(QList<hoststruct> l, QString s) {
    refreshlist();
    hosts.sethoststruct(l, s);
}
void netcoupler::getwholist() {
    users.setuserstruct(irc->wholist, irc->joinlist);
    irc->who();
}
void netcoupler::getmsg(const QString &user, const QString &receiver,
                        const QString &msg) {
    if (containsCI(channellist, receiver)) {
        emit siggotmsg(user, receiver, msg);
    } else {
        bool b = compareCI(receiver, nick);
        Q_UNUSED(b);
        Q_ASSERT_X(b==1,"getmsg netcoupler",qPrintable(receiver));
        emit siggotprivmsg(user, receiver, msg);
        QApplication::processEvents();
    }
}
void netcoupler::getnotice(const QString &user, const QString &receiver,
                           const QString &msg) {
    emit siggotnotice(user, receiver, msg);
}
void netcoupler::sendmessage(const QString &receiver, const QString &msg) {
    irc->sendmessage(msg, receiver);
}
void netcoupler::senduncheckedmessage(const QString &user, const QString &msg) {
    if (msg.startsWith(">!")) {
        sendrawcommand(QString("PRIVMSG ") + user + " :\001"
                       + QString(msg).remove(0, 2).simplified() + "\001");
    } else if (msg.startsWith("/")) {
        sendrawcommand(QString(msg).remove(0, 1).simplified());
    } else if (msg.startsWith(">>>")) {
        sendrawcommand(QString("NOTICE ") + user + " :\001ACTION "
                       + QString(msg).remove(0, 3).simplified() + " \001");
    } else if (msg.startsWith(">>")) {
        sendnotice(user, QString(msg).remove(0, 2).simplified());
    } else if (msg.startsWith(">")) {
        sendrawcommand(QString("PRIVMSG ") + user + " :\001ACTION " + QString(
                msg).remove(0, 1).simplified() + " \001\n");
    } else {
        sendmessage(user, msg);
    }
}
void netcoupler::sendnotice(const QString &receiver, const QString &msg) {
    irc->sendnotice(msg, receiver);
}
void netcoupler::sendrawcommand(const QString &raw) {
    irc->sendrawcommand(raw);
}
void netcoupler::refreshlist() {
    irc->refreshlist();
}
void netcoupler::sendquit() {
    irc->quit();
}
void netcoupler::getscheme(QString chan, QString scheme) {
    schememap[chan] = scheme;
}
void netcoupler::refreshwho() {

}
void netcoupler::copyprvhosttoclipboard(const QString &scheme) {
    int i = users.users.indexOf(userstruct::whoami(nick));
    if (i > -1) {
        QString address = myip;
        QString host = "wa://" + address + "?gameid=999" + "&scheme=" + scheme;
        QString port = gethostport();
        if (port != "")
            QString host = "wa://" + address + ":" + port + "?gameid=999"
                           + "&scheme=" + scheme;

        QApplication::clipboard()->setText(host);
    }
}
void netcoupler::sendprvhosttobuddys(const QString &scheme) {
    int i = users.users.indexOf(userstruct::whoami(nick));
    if (i > -1) {
        QString address = myip;
        QString host = "wa://" + address + "?gameid=999" + "&scheme=" + scheme;
        QString port = gethostport();
        if (port != "")
            QString host = "wa://" + address + ":" + port + "?gameid=999"
                           + "&scheme=" + scheme;
        sendmessagetoallbuddys(nick + " " + "is hosting a private game:" + " "
                               + host);
    }
}
void netcoupler::sendmessagetoallbuddys(const QString &msg) {
    foreach(QString s,singleton<snpsettings>().map["buddylist"].value<QStringList>()) {
        if (users.users.contains(userstruct(userstruct::whoami(s)))) {
            QString nick = users.users[users.users.indexOf(userstruct(
                    userstruct::whoami(s)))].nick;
            this->sendnotice(nick, msg);
        }
    }
}
void netcoupler::setaway(const QString &s) {
    awaymessage = s;
    isaway = 1;
}
void netcoupler::ircconnected(){
    emit sigconnected();
}
void netcoupler::ircdiconnected(){
    emit sigdisconnected();
}
//*****************************join your own host**********************
namespace looki {
    QString gamename;
    QString currentchannel;
    QStringList lasthost;
    int gethostlistcount;
    bool iswormkitgame = 0;
}
void netcoupler::joingamelink(const QString &gamelink) {
    joinprvgame *cast = qobject_cast<joinprvgame*> (sender());
    Q_CHECK_PTR(cast);
    looki::currentchannel = cast->chan;
    QString temp = getprocessstring();
    temp = temp + " \"" + gamelink + "\"";
    startprocess(temp);
    setaway();
}

void netcoupler::joingame(const QString &hostinfo, const QString &channel,
                          const QString &gamename) {
    looki::gamename = gamename;
    looki::currentchannel = channel;
    QString temp = getprocessstring();
    temp = temp + hostinfo;
    startprocess(temp);
    setaway();
    if (singleton<settingswindow>().from_map("chbactionwhenjoining").toBool())
        sendinfotochan(channel, " is joining a game: " + gamename);
}
void netcoupler::createhost(const QString &name, const QString &pwd,
                            const QString &chan, const QString &flag) {
    usesettingswindow("sbhosttimeout");
    looki::gethostlistcount = 0;
    looki::gamename = name;
    looki::currentchannel = chan;
    int i = users.users.indexOf(userstruct::whoami(nick));
    if (i > -1) {
        QString address;
        if(singleton<snpsettings>().map["useacostumipforhosting"].value<bool> ()){
            address=singleton<snpsettings>().map["costumipforhosting"].value<QString>();
        }else{
            address = myip;
        }
        singleton<snpsettings>().map["leplayername"];
        if (singleton<snpsettings>().map["leplayername"].toString().isEmpty())
            http->sendhost(name, address, nick, pwd, chan, flag);
        else
            http->sendhost(name, address,
                           singleton<snpsettings>().map["leplayername"].toString(), pwd, chan, flag);
        if (singleton<snpsettings>().map["waitforhostappearsinwormnet"].value<bool> ()) {
            connect(&getownhosttimer, SIGNAL(timeout()),http, SLOT(refreshhostlist()));
            getownhosttimer.start(2000);
            connect(http, SIGNAL(sighostwontstart()),this, SLOT(hostwontstart()));
            connect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlistforhosting(QList<hoststruct>,QString)));
        } else {
            QString temp = getprocessstring();
            if (temp != "") {
                temp = temp + " \"" + "wa://" + "?gameid=999" + "&scheme="
                       + schememap[looki::currentchannel] + "\"";
                startprocess(temp);
                setaway();
                int i = users.users.indexOf(userstruct::whoami(nick));
                if (i > -1) {
                    QString address;
                    if(singleton<snpsettings>().map["useacostumipforhosting"].value<bool> ()){
                        address=singleton<snpsettings>().map["costumipforhosting"].value<QString>();
                    }else{
                        address = myip;
                    }
                    QString host = QString("wa://%1?gameid=999&scheme=%2").arg(
                            address).arg(schememap[looki::currentchannel]);
                    QString msg = QString(" is hosting a game: %1, %2").arg(
                            looki::gamename).arg(host);
                    if (singleton<snpsettings>().map["chbsendhostinfotochan"].toBool())
                        sendinfotochan(looki::currentchannel, msg);
                }
                looki::lasthost.clear();
            }
        }
    }
}
void netcoupler::createprvhost(const QString &chan, const QString &scheme) {
    //gamename = name;
    looki::currentchannel = chan;
    int i = users.users.indexOf(userstruct::whoami(nick));
    if (i > -1) {
        QString address;
        if(singleton<snpsettings>().map["useacostumipforhosting"].value<bool> ()){
            address=singleton<snpsettings>().map["costumipforhosting"].value<QString>();
        }else{
            address = myip;
        }
        QString temp = getprocessstring();
        if (temp != "") {
            temp = temp + " \"" + "wa://" + "?gameid=999" + "&scheme=" + scheme
                   + "\"";
            startprocess(temp);
            setaway();
            //lasthost.clear();
            //lasthost << h.gameid << gamename;
        }
    }
}
void netcoupler::gethostlistforhosting(QList<hoststruct> l, QString) {
    QString address =myip;
    foreach(hoststruct h,l) {
        if (nick == h.playername) {
            if (!address.isEmpty() && h.gameip.split(":",
                                                     QString::SkipEmptyParts).first() == address
                && h.gamename == looki::gamename) {
                QString temp = getprocessstring();
                if (temp != "") {
                    temp = temp + " \"" + h.hoststring + "&scheme="
                           + schememap[looki::currentchannel] + "\"";
                    startprocess(temp);
                    setaway();
                    int i = users.users.indexOf(userstruct::whoami(nick));
                    if (i > -1) {
                        QString port=gethostport();
                        QString address;
                        if(singleton<snpsettings>().map["useacostumipforhosting"].value<bool> ()){
                            address=singleton<snpsettings>().map["costumipforhosting"].value<QString>();
                        }else{
                            address = myip;
                        }
                        address = address+":" +port;
                        QString
                                host =
                                QString(
                                        "wa://%1?gameid=999&scheme=%2").arg(
                                                address).arg(
                                                        schememap[looki::currentchannel]);
                        QString msg =
                                QString(" is hosting a game: %1, %2").arg(
                                        looki::gamename).arg(host);
                        if (singleton<snpsettings>().map["chbsendhostinfotochan"].toBool())
                            sendinfotochan(looki::currentchannel, msg);
                    }
                    looki::lasthost.clear();
                    looki::lasthost << h.gameid << looki::gamename;
                }
            }
            getownhosttimer.stop();
            getownhosttimer.disconnect();
            disconnect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlistforhosting(QList<hoststruct>,QString)));
            hostlifetimer.singleShot(180 * 1000, this, SLOT(hosttimetout()));
            return;
        }
    }
    looki::gethostlistcount++;
    if (looki::gethostlistcount > hosttimeoutdelay) {
        getownhosttimer.stop();
        getownhosttimer.disconnect();
        qDebug() << "hostrequest timed out";
        disconnect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlistforhosting(QList<hoststruct>,QString)));
        hosttimetout();
    }
}
void netcoupler::processfinished(int , QProcess::ExitStatus e) {
    if (!looki::iswormkitgame) {
        http->closehost(looki::lasthost);
        hostlifetimer.stop();
        hostlifetimer.disconnect();
    }
    if (e == 0)
        qDebug() << "joining/hosting a game finished normally";
    else
        qDebug() << "joining/hosting a game crashed";
    if (setawayingame) {
        if (wasaway)
            isaway = 1;
        else {
            isaway = 0;
            wasaway = 0;
            if (!singleton<snpsettings>().map["awaymessage"].value<QStringList> ().isEmpty())
                this->awaymessage
                        = singleton<snpsettings>().map["awaymessage"].value<QStringList> ().last();emit
                          sigawaystringchanged();
            foreach(QString s,mainwindow::rememberwhogotaway.keys()) {
                if (singleton<settingswindow>().from_map("chbbacktonormals").toBool()
                    && !singleton<snpsettings>().map["ignorelist"].toStringList().contains(
                            s, Qt::CaseInsensitive)) {
                    this->sendnotice(s,
                                     singleton<settingswindow>().from_map("lebackmessage").toString());
                    sendrawcommand("PRIVMSG " + s + " :\001back\001");
                } else if (singleton<settingswindow>().from_map("chbbacktobuddys").toBool()) {
                    if (singleton<snpsettings>().map["buddylist"].toStringList().contains(s,
                                                                                          Qt::CaseInsensitive))
                        this->sendnotice(
                                s,
                                singleton<settingswindow>().from_map("lebackmessage").toString());
                    sendrawcommand("PRIVMSG " + s + " :\001back\001");
                }
            }
        }
        mainwindow::rememberwhogotaway.clear();
    }
}
void netcoupler::hosttimetout() {
    http->closehost(looki::lasthost);
}
void netcoupler::hostwontstart() {
    getownhosttimer.stop();
    getownhosttimer.disconnect();
    disconnect(http, SIGNAL(sighostlist(QList<hoststruct>,QString)),this, SLOT(gethostlistforhosting(QList<hoststruct>,QString)));
}
void netcoupler::readprocess() {
    qWarning() << qobject_cast<QProcess*> (sender())->readAll();
}
QString netcoupler::getprocessstring() {
    QStringList sl = singleton<snpsettings>().map["joinstrings"].value<QStringList> ();
    if (sl.isEmpty())
        QMessageBox::warning(0, "", tr("No executables are given.\n"
                                       "you must choose a game executable,\n"
                                       "for example wa.exe, to join a game.\n"
                                       "if you dont know what this means,\n"
                                       "please read the tutorial.pdf inside the snooper folder."),
                             QMessageBox::Ok);
#ifdef Q_WS_MAC
    if (!sl.isEmpty()) {
        QString file = sl.first();
        QFile f(file);
        if (f.open(QFile::ReadOnly)) {
            QTextStream ts(&f);
            QString s = ts.readLine();
            int i = 0;
            while (!s.startsWith("Exec=") && !ts.atEnd() && i < 50) {
                i++;
                s = ts.readLine();
            }
            if (!ts.atEnd() && i < 50) {
                QString temp = s;
                temp.remove("Exec=");
                return temp;
            }
        }
    }
#endif
#ifdef Q_WS_X11
    if (!sl.isEmpty()) {
        QString file = sl.first();
        QFile f(file);
        if (f.open(QFile::ReadOnly)) {
            QTextStream ts(&f);
            QString s = ts.readLine();
            int i = 0;
            while (!s.startsWith("Exec=") && !ts.atEnd() && i < 50) {
                i++;
                s = ts.readLine();
            }
            if (!ts.atEnd() && i < 50) {
                QString temp = s;
                temp.remove("Exec=");
                return temp;
            }
        }
    }
#endif
#ifdef Q_WS_WIN
    if(!sl.isEmpty()) {
        if(sl.first().endsWith("WormKit.exe")) {
            looki::iswormkitgame=1; //wormkit closes the QProcess
            return QString("\"")+sl.first()+"\" wa.exe";
        }
        else {
            looki::iswormkitgame=0;
            return QString("\"")+sl.first()+"\"";
        }
    }
#endif
    return "";
}
//*****************************join your own host end**********************
void netcoupler::sendinfotochan(const QString &chan, const QString &msg) {
    QString command = QString("PRIVMSG %1 :\001ACTION %2 \001").arg(chan).arg(
            msg);
    sendrawcommand(command);
    command = QString("\001ACTION %1 \001").arg(msg);
    getmsg(nick, chan, command);
}
void netcoupler::usesettingswindow(const QString &s) {
    if (s == "sbwhorepead" || s == "") {
        int i = singleton<settingswindow>().from_map("sbwhorepead").value<int> ();
        timer.disconnect();
        connect(&timer, SIGNAL(timeout()),this, SLOT(getwholist()));
        timer.start(i);
    } else if (s == "cbsetawaywhilegaming" || s == "")
        setawayingame
                = singleton<settingswindow>().from_map("cbsetawaywhilegaming").value<bool> ();
    else if (s == "leawaystring" || s == "")
        awaystringwhilehosting = singleton<settingswindow>().from_map("leawaystring").value<
                                 QString> ();
    else if (s == "sbhosttimeout" || s == "")
        hosttimeoutdelay = singleton<settingswindow>().from_map("sbhosttimeout").value<int> ();

    singleton<sound_handler>().init();
}
void netcoupler::settingswindowemitfunktion() { //signals are protected?!
    emit sigsettingswindowchanged();
}
void netcoupler::setaway() {
    if (setawayingame) {
        if (!isaway){
            isaway = 1;
            wasaway=0;
        }
        else
            wasaway = 1;
        awaymessage = awaystringwhilehosting;
        oldawaystring = awaymessage;
    }
    emit sigawaystringchanged();
}
void netcoupler::refreshhostlist() {
    http->refreshhostlist();
}
void netcoupler::startprocess(const QString &s){    
    if(singleton<settingswindow>().from_map("chbhidechannelwindowsongame").toBool()){
        foreach(window *w,mainwindow::windowlist)
        {
            w->minimize();
        }
    } if(singleton<settingswindow>().from_map("chbdisconnectongame").toBool()){
        emit sigdisconnect();
    }
    p->start(s);
}
