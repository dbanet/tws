#include "mainwindow.h"
#include "window.h"
#include "netcoupler.h"
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QPicture>
#include<QDialogButtonBox>
#include "buddylist.h"
#include <QTime>
#include "ctcphandler.h"
#include "chathandler.h"
#include <QFileDialog>
#include "buttonlayout.h"
#include "awaybox.h"
#include "inihandlerclass.h"
#include "chatwindow.h"
#include "about.h"
#include "snpsettings.h"
#include "charformatsettings.h"
#include "ui_window.h"
#include "ui_window2.h"
#include "ui_window3.h"
#include "settingswindow.h"
#include "volumeslider.h"
#include "logbrowser.h"
#include "textschemehandler.h"
#include "playername.h"
#include "singleton.h"
#include "sound_handler.h"
#include "global_functions.h"
#include "balloon_handler.h"
#include"src/irc/irc_netcoupler.h"
#include"src/irc/irc_window.h"
#include "src/irc/ircjoindia.h"
#include"codecselectdia.h"
#include"clantowebpagemapper.h"
#include"leagueserverhandler.h"
#include "picturehandler.h"
#include"quithandler.h"

inihandlerclass inihandler;
QPointer<netcoupler> net;
extern volumeslider *volume;
bool fontorcolorchanged = 0;
extern QStringList querylist;
QString GamesourgeChannelName="#GameSourgeWorms";
mainwindow::mainwindow() {
    net = NULL;
    whichuitype = 1;
    ui.setupUi(this);        
    bool b=singleton<snpsettings>().map["righttoleftwriting"].toBool();
    if(b)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
    ui.pbrememberjoin->setText(tr("Autojoin:"));
    ui.pbabout->setText(tr("About"));
    ui.tabWidget->setTabEnabled(1, 0);

    get_baseStyleSheet();
    init_menus();    
    connect(singleton<balloon_handler>().tray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason)),this, SLOT(trayactivation(QSystemTrayIcon::ActivationReason)));            
    singleton<picturehandler>().fillflags(ui.flag);
    singleton<picturehandler>().fillranks(ui.rank);
    snpsetcontains("chbminimized");
    snpsetcontains("nickname");
    snpsetcontains("tus_password");
    snpsetcontains("tus_login");
    snpsetcontains("tusloginenable");
    snpsetcontains("whichuitype");
    snpsetcontains("countrycode");    
    snpsetcontains("client");
    snpsetcontains("clan");
    snpsetcontains("wormnetserverlist");    
    snpsetcontains("rank");
    snpsetcontains("leagueservers");

    ui.cbremember->setChecked(singleton<snpsettings>().map["chbremember"].value<bool> ());
    if (ui.cbremember->isChecked()) {
        chooseclicked();        
    }
    this->setWindowTitle(tr("Wheat Snoopers root window."));
    joinonstartup = 0;
    connect(ui.tabWidget, SIGNAL(currentChanged (int) ),this, SLOT(returntotabsettings(int)));
    connect(ui.start, SIGNAL(clicked()),this, SLOT(chooseclicked()));
    connect(ui.pbrememberjoin, SIGNAL(clicked()),this, SLOT(pbrememberjoinclicked()));
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),&singleton<quithandler>(), SLOT(beforequit()));
    connect(ui.tabWidget, SIGNAL(currentChanged ( int )),this, SLOT(returntotabsettings(int)));    

    QRegExp regexp;
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`){15}");
    validator = new QRegExpValidator(regexp, 0);
    ui.lenick->setValidator(validator);
    ui.clan->setValidator(validator);
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`|[ ]|=|){25}");
    validatorclient = new QRegExpValidator(regexp, 0);
    //ui.client->setValidator(validatorclient);   

    singleton<snpsettings>().map["isarrangedbyonetab"] = false;
    singleton<snpsettings>().map["isarrangedbytwotabs"] = false;

    debugmsg.clear();

    connect(&singleton<ctcphandler>(), SIGNAL(sigctcpcommand(const QString&,const QString&)),this, SLOT(gotctcpsignal(const QString&,const QString&)));

    QVariantList windowstates = singleton<snpsettings>().map["MainWindowGeometry"].toList();
    if (!windowstates.isEmpty())
        restoreGeometry(windowstates.takeFirst().toByteArray());        
}
mainwindow::~mainwindow() {
}
void mainwindow::fillsnpsettings(){
    singleton<snpsettings>().map["chbremember"] = ui.cbremember->isChecked();
    singleton<snpsettings>().map["nickname"] = ui.lenick->text();
    singleton<snpsettings>().map["chbminimized"] = ui.chbminimized->isChecked();
    singleton<snpsettings>().map["chbautojoin"] = ui.chbautojoin->isChecked();
    singleton<snpsettings>().map["whichuitype"] = whichuitype;
    singleton<snpsettings>().map["countrycode"]=ui.flag->currentText();
    singleton<snpsettings>().map["rank"]=ui.rank->currentText();
    if (fontorcolorchanged == 1) {
        singleton<snpsettings>().map["charformatfile"] = "lastedited.textscheme";
        singleton<charformatsettings>().safe();
    }
}
void mainwindow::get_baseStyleSheet(){
    QFile f(QApplication::applicationDirPath() + "/qss/Skin_Base");
    if(!f.open(QFile::ReadOnly))
        QMessageBox::warning(0,QObject::tr("Warning"),QObject::tr("Cant read the Skinfile:\nSkin_Base"));
    baseStyleSheet = QLatin1String(f.readAll());
}
void mainwindow::connectToNetwork(){    
    ui.tabWidget->setCurrentIndex(1);
    if(singleton<snpsettings>().map["tusloginenable"].toBool())
        net = new netcoupler(singleton<leagueserverhandler>().nick, this);
    else
        net = new netcoupler(ui.lenick->text(), this);
    connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(net,SIGNAL(sigreconnect()),this,SLOT(reconnect()));
    connect(net, SIGNAL(siggotchanellist(const QStringList &)),this, SLOT(getchannellist(const QStringList &)));
    connect(net, SIGNAL(sigawaystringchanged()),this, SLOT(awaymessagechanged()));
    connect(net, SIGNAL(siggotprivmsg(const QString&,const QString&,const QString&)),this,
            SLOT(gotprvmsg(const QString&,const QString&,const QString&)));
    connect(net,SIGNAL(sigconnected()),this,SLOT(connected()));
    connect(net,SIGNAL(sigdisconnected()),this,SLOT(disconnected()));
    connect(net,SIGNAL(sigdisconnect()),this,SLOT(disconnect_netcoupler()));

    QTimer::singleShot(7000,this,SLOT(reopenChatWindowsAndChannelWindows()));
    ui.tabWidget->setTabEnabled(1, 1);
}
void mainwindow::chooseclicked() {
    if(ui.tabWidget->currentIndex()!=0)
        return;
    if (ui.lenick->text().isEmpty()){
        QMessageBox::warning(this,tr("Nickname field is empty"),tr("Please choose a nickname."),QMessageBox::Ok);
        return;
    }
    if (ui.clan->text().isEmpty())
        singleton<snpsettings>().map["clan"]="Username";
    else
        singleton<snpsettings>().map["clan"] = ui.clan->text();
    singleton<snpsettings>().map["rank"] = ui.rank->currentText();
    singleton<snpsettings>().map["nickname"] = ui.lenick->text();
    singleton<snpsettings>().map["tus_password"] = ui.letuspassword->text();
    singleton<snpsettings>().map["tus_login"] = ui.letuslogin->text();
    singleton<snpsettings>().map["countrycode"] = ui.flag->currentText();
    singleton<snpsettings>().map["rank"] = ui.rank->currentText();
    singleton<snpsettings>().map["client"] = ui.client->text();
    singleton<snpsettings>().map["tusloginenable"] = ui.cbenabletus->isChecked();

    singleton<snpsettings>().map["wormnetserverlist"] = refreshcombobox(ui.cbServerList);
    singleton<snpsettings>().map["leagueservers"]= refreshcombobox(ui.cbleagueservers);

    singleton<snpsettings>().safe();
    if(singleton<snpsettings>().map["tusloginenable"].toBool()){           
        connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginfailed()),this,SLOT(leagueserverconnectionfailed()));
        connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginsuccess()),this,SLOT(leagueserverconnectionsuccess()));
        QString item=ui.cbleagueservers->currentText();
        if(!item.endsWith("/"))
            item+="/";
        if(!item.startsWith("http://"))
            item="http://"+item;
        QString servicename=item;
        servicename=servicename.remove("http://");
        int i=servicename.indexOf("/");
        if(i!=-1)
            servicename=servicename.left(i);
        singleton<leagueserverhandler>().setleague(servicename,item);

        singleton<leagueserverhandler>().login(ui.letuslogin->text(),ui.letuspassword->text());
    } else
        connectToNetwork();
}
void mainwindow::leagueserverconnectionfailed(){
}
void mainwindow::leagueserverconnectionsuccess(){
    connectToNetwork();
}
void mainwindow::reopenChatWindowsAndChannelWindows(){
    foreach(QString s,lastOpenedChatWindows)
        openchatwindow(s);
    lastOpenedChatWindows.clear();
    foreach(QString s,lastOpenedWindows)
        join(s);
    lastOpenedWindows.clear();
}

void mainwindow::getchannellist(const QStringList &sl) {
    ui.cbchannels->clear();
    foreach(QString s,sl) {
        ui.cbchannels->addItem(s.split(" ").first());
    }
    channellist = sl;
    ui.pbjoin->setEnabled(1);
    joinmenu->clear();
    foreach(QString s,channellist) {
        joinmenu->addAction(s);
    }
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesourgeChannelName);
#endif
    if (joinonstartup == 0) {
        snpsetcontains("joinonstartup");
        joinonstartup = 1;
    }
}
void mainwindow::join(const QString channel){

    if(channel==GamesourgeChannelName){
        joinGameSourge();
        return;
    }   
    if(currentchannellist.contains(channel))
        return;
    currentchannellist << channel;
    net->joinchannel(channel);    
    if (whichuitype == 1) {
        windowlist.push_back(new ::window(net, channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else if (whichuitype == 2) {
        windowlist.push_back(new ::window(net, channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else if (whichuitype == 3) {
        windowlist.push_back(new ::window(net, channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else
        myDebug() << "joinclicked in mainwindow assert";
    windowlist.last()->show();
    windowlist.last()->raise();
    if (net->isaway) {
        windowlist.last()->windowtitleaway = net->awaymessage;
        windowlist.last()->mysetwindowtitle();
    }
    connect(windowlist.last(), SIGNAL(sigjoinchannel(const QString&)),this, SLOT(join(const QString&)));
    connect(windowlist.last(), SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
    connect(this, SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
    connect(windowlist.last(), SIGNAL(sigwindowclosed(const QString&)),this, SLOT(windowremoved(const QString&)));
    singleton<snpsettings>().safe();
}
void mainwindow::onquit() {        
    foreach(chatwindow *w,::window::chatwindows) {
        Q_ASSERT(w!=0);
        w->close();
        w->deleteLater();
    }
    foreach(::window *w,this->windowlist) {
        w->close();
        w->deleteLater();
    }
    singleton<mainwindow>().fillsnpsettings();
}
void mainwindow::closeEvent(QCloseEvent *) {
    QVariantList windowstates;
    windowstates << saveGeometry();
    singleton<snpsettings>().map["MainWindowGeometry"]=windowstates;
    singleton<snpsettings>().safe();
}
void mainwindow::disconnect_netcoupler(){
    returntotabsettings(0);
}

void mainwindow::returntotabsettings(int i) {
    if (i != 0)
        return;    
    singleton<leagueserverhandler>().reset();
    joinonstartup = 0;
    singleton<snpsettings>().map["chbautojoin"] = ui.chbautojoin->isChecked();
    singleton<snpsettings>().safe();
    joinmenu->clear();
    currentchannellist.clear();
    ui.cbchannels->clear();
    ui.pbjoin->setEnabled(0);
    ui.tabWidget->setTabEnabled(1, 0);
    ui.pbjoin->setEnabled(0);
    foreach(chatwindow *w,::window::chatwindows) {
        Q_ASSERT(w!=0);
        w->deleteLater();
    }
    qDeleteAll(::window::chatwindows.begin(), ::window::chatwindows.end());
    ::window::chatwindows.clear();
    foreach(::window *w,this->windowlist) {
        Q_ASSERT(w!=0);
        w->deleteLater();
    }
    windowlist.clear();
    net->deleteLater();
    qApp->processEvents();
    ui.tabWidget->setTabEnabled(1, 0);
}
void mainwindow::changeEvent(QEvent * event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (this->isMinimized()) {
            QTimer::singleShot(200, this, SLOT(close()));
            return;
        }
    }
    QWidget::changeEvent(event);
}
void mainwindow::pbrememberjoinclicked() {
    singleton<snpsettings>().map["joinonstartup"] = ui.cbchannels->currentText();
    ui.pbrememberjoin->setText(tr("Autojoin:")
                               + "\n" + singleton<snpsettings>().map["joinonstartup"].value<QString> ());
    singleton<snpsettings>().safe();
}
void mainwindow::snpsetcontains(const QString &s) {
    if (s == "chbautojoin" && singleton<snpsettings>().map.contains(s))
        ui.chbautojoin->setChecked(singleton<snpsettings>().map["chbautojoin"].value<bool> ());    
    else if (s == "qss file") {        
        QFile f(QApplication::applicationDirPath() + "/qss/" + singleton<snpsettings>().map["qss file"].toString());
        if(!f.open(QFile::ReadOnly))
            QMessageBox::warning(this,QObject::tr("Warning"),tr("Cant read the Skinfile:\n")+singleton<snpsettings>().map["qss file"].toString());
        QString stylesheet = QLatin1String(f.readAll());
        qApp->setStyleSheet(baseStyleSheet+stylesheet);
    } else if (s == "joinonstartup" && singleton<snpsettings>().map.contains(s)
        && singleton<snpsettings>().map.contains("chbautojoin")) {
        ui.pbrememberjoin->setText(
                ui.pbrememberjoin->text().split("\n").first() + "\n"
                + singleton<snpsettings>().map["joinonstartup"].value<QString> ());
        if (singleton<snpsettings>().map["chbautojoin"].value<bool> ()) {
            join(singleton<snpsettings>().map["joinonstartup"].value<QString> ());
        }
    } else if (s == "chbminimized")
        ui.chbminimized->setChecked(singleton<snpsettings>().map["chbminimized"].value<bool> ());   
    else if (s == "nickname" && singleton<snpsettings>().map.contains(s))
        ui.lenick->setText(singleton<snpsettings>().map["nickname"].value<QString> ());
    else if (s == "tus_password" && singleton<snpsettings>().map.contains("tus_password"))
        ui.letuspassword->setText(singleton<snpsettings>().map["tus_password"].value<QString> ());
    else if (s == "tus_login" && singleton<snpsettings>().map.contains("tus_login"))
        ui.letuslogin->setText(singleton<snpsettings>().map["tus_login"].value<QString> ());
    else if (s == "countrycode"){
        QString language=QLocale::system().name().left(2);
        int i=ui.flag->findText(singleton<snpsettings>().map["countrycode"].toString());
        if(i==-1)
            i=ui.flag->findText(language);
        ui.flag->setCurrentIndex(i);
    }else if (s == "rank"){
        ui.rank->setCurrentIndex(singleton<snpsettings>().map["rank"].toString().toInt());
    }
    else if (s == "client" && singleton<snpsettings>().map.contains(s))
        ui.client->setText(singleton<snpsettings>().map[s].value<QString> ());
    else if (s == "clan" && singleton<snpsettings>().map.contains(s)){
        QString clanstring=singleton<snpsettings>().map[s].value<QString> ();
        if(clanstring.toLower()=="username")
            ui.clan->setText("");
        else
            ui.clan->setText(clanstring);
    }
    else if (s == "whichuitype" && singleton<snpsettings>().map.contains(s))
        whichuitype = singleton<snpsettings>().map["whichuitype"].value<int> ();
    else if (s == "wormnetserverlist"){        
        ui.cbServerList->addItems(singleton<snpsettings>().map["wormnetserverlist"].value<QStringList> ());
    }
    else if (s == "leagueservers"){
        ui.cbleagueservers->addItems(singleton<snpsettings>().map["leagueservers"].value<QStringList>());
    }
    else if(s=="tusloginenable"){
        bool b=singleton<snpsettings>().map["tusloginenable"].toBool();
        ui.cbenabletus->setChecked(b);
        on_cbenabletus_toggled(b);
    }
}
void mainwindow::windowremoved(const QString &s) {
    this->currentchannellist.removeAll(s);
    windowlist.removeAll(qobject_cast<channelwindow*> (sender()));
}
void mainwindow::usesettingswindow(const QString&) {
}
void mainwindow::chatwinowclosed() {
    chatwindow *w = qobject_cast<chatwindow*> (sender());
    Q_CHECK_PTR(w);
    w->disconnect();
    int i = window::chatwindowstringlist.removeAll(w->chatpartner);
    Q_ASSERT(i!=0);
    i = window::chatwindows.removeAll(w);
    Q_ASSERT(i!=0);
}
void mainwindow::appenddebugmessage(const QString &msg) {
    debugmsg.append(msg);
    ui.labeltraydescription->insertPlainText(debugmsg);
    foreach( ::window *w,windowlist) {
        w->gotdebugmsg(debugmsg);
    }
    debugmsg.clear();
}
void mainwindow::setlanguage(const QString &langfile) {
    singleton<snpsettings>().map["language file"] = langfile;
    myDebug() << singleton<snpsettings>().map["language file"].value<QString> ();
    singleton<snpsettings>().safe();
    QMessageBox::StandardButton button = QMessageBox::question(this, tr(
            "Restart the application?"), tr(
                    "Changing the translation requires a program restart.\n"
                    "To restart now click Ok, otherwise cancel."), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
    if (button == QMessageBox::Ok) {
        int i = 0;
        returntotabsettings(i);
        QProcess::startDetached(qApp->applicationFilePath(), QStringList(),
                                QApplication::applicationDirPath());
        foreach(::window *w,this->windowlist) {
            w->close();
        }
        singleton<balloon_handler>().tray->hide();
        QTimer::singleShot(100, qApp,SLOT(quit()));
    }
}
void mainwindow::quitslot() {
    //qApp->exit();
}
void mainwindow::awayboxok() {
    singleton<balloon_handler>().set_away_tray_icon();
    foreach(::window *w,this->windowlist) {
        w->windowtitleaway = " " + tr("<away>:") +" "+ net->awaymessage;
        w->mysetwindowtitle();
    }
    net->wasaway = 1;
    net->isaway = 1;
}
void mainwindow::awaymessagechanged() {
    if (net->isaway) {
        foreach(::window *w,this->windowlist) {
            w->windowtitleaway = " " + tr("<away>:") + " "+net->awaymessage;
            w->mysetwindowtitle();
        }
    } else {
        foreach(::window *w,this->windowlist) {
            w->windowtitleaway = "";
            w->mysetwindowtitle();
        }
    }
}
void mainwindow::gotctcpsignal(const QString& command, const QString &user) {
    foreach(chatwindow *w,channelwindow::chatwindows) {
        if (compareCI(w->chatpartner, user)) {
            if (command.startsWith("away"))
                w->setaway(1, command);
            if (command == "back")
                w->setaway(0);
        }
    }
}
void mainwindow::settextscheme(const QString &file) {
    myDebug() << tr("trying to apply new textscheme: ") + file;
    singleton<snpsettings>().map["charformatfile"] = file;
    singleton<charformatsettings>().load();
    chathandler::initialformatstarter();
}
void mainwindow::openchatwindow(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(net, s));
    window::chatwindows.last()->show();
    connect(window::chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
void mainwindow::openchatwindowhidden(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(net, s));
    mainwindow::hiddenchatwindowshelper << window::chatwindows.last();
    connect(window::chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
void mainwindow::gotprvmsg(const QString &user, const QString &receiver,
                           const QString &msg) {
    bool ctcp_wants_it = 0;
    if (!msg.startsWith("\001ACTION", Qt::CaseInsensitive) && msg.startsWith(
            "\001")) {
        ctcp_wants_it = singleton<ctcphandler>().getctcp(user, msg);
    }
    if (ctcp_wants_it)
        return;
    usergarbagemap[user.toLower()]
            << QDate::currentDate().toString("dd.MM") + " "
            + QTime::currentTime().toString("hh:mm") + " " + user
            + ">" + QString(msg).remove("\n").remove("\r");
    if (net->isaway && !ctcp_wants_it) {
        if (mainwindow::rememberwhogotaway[user] != net->awaymessage) {
            net->sendmessage(user, net->awaymessage);
            int i = net->users.users.indexOf(userstruct::whoami(user));
            if (i != -1 && net->users.users[i].clan != "Username")
                net->sendrawcommand("PRIVMSG " + user + " :\001away "
                                    + net->awaymessage + "\001");
        }
        mainwindow::rememberwhogotaway[user] = net->awaymessage;
    }
    if (!ctcp_wants_it) {
        if (!net->users.usermap[usermodel::tr("Ignorelist")].count(
                userstruct::whoami(user))
            && !::window::chatwindowstringlist.contains(user,
                                                        Qt::CaseInsensitive)) {
            singleton<balloon_handler>().got_privmsg(user, msg);
        }
        if (!containsCI(window::chatwindowstringlist, user)) {
            if ((net->users.usermap[usermodel::tr("Buddylist")].count(
                    userstruct::whoami(user)) || mainwindow::windowlist.isEmpty()
                || singleton<settingswindow>().from_map("chballwaysopenchatwindows").toBool())
                && !net->users.usermap[usermodel::tr("Ignorelist")].count(
                        userstruct::whoami(user))) {
                Q_ASSERT(user!="");
                if (singleton<settingswindow>().from_map("chbstartchatsminimized").toBool()) {
                    if (mainwindow::windowlist.isEmpty()) {
                        singleton<sound_handler>().play_normalmsgsound(user);
                    } else {
                        foreach(::window *w,mainwindow::windowlist) {
                            w->gotprvmsg(user, receiver, msg);
                        }
                    }
                    openchatwindowhidden(user);
                    window::chatwindows.last()->gotmsg(user, receiver, msg);
                    if (!containsCI(querylist, user))
                        querylist << user;
                } else {
                    openchatwindow(user);
                    window::chatwindows.last()->gotmsg(user, receiver, msg);
                }
            }
        } else {
            foreach(chatwindow *w,::window::chatwindows) {
                w->gotmsg(user, receiver, msg);
            }
            return;
        }
        foreach(::window *w,mainwindow::windowlist) {
            w->gotprvmsg(user, receiver, msg);
        }
    }
}
void mainwindow::connected(){
    ui.connectlabel->setText(tr("Connected"));
}
void mainwindow::disconnected(){
    ui.connectlabel->setText(tr("Disconnected"));
}
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//***********************                                     ************************************************************
//***********************             Menu                    ************************************************************
//***********************                                     ************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
void mainwindow::init_menus(){
    traymenu = new QMenu(this);
    connect(traymenu,SIGNAL(triggered(QAction*)),this,SLOT(traymenutriggered(QAction*)));
    singleton<balloon_handler>().tray->setContextMenu(traymenu);
    QAction *a0;
    stuffmenu = traymenu->addMenu(tr("Stuff"));
    stuffmenu->setIcon(QIcon("snppictures/stufficon.png"));
    stuffmenu->addAction(tr("About"));
    stuffmenu->addAction(tr("Settings"))->setIcon(QIcon("snppictures/settingsicon.png"));
    stuffmenu->addAction(tr("Volume"));
    stuffmenu->addAction(tr("Open Log Browser"));
    stuffmenu->addAction(usermodel::tr("Buddylist"));
    stuffmenu->addAction(tr("Scheme maker"));
    stuffmenu->addAction(tr("Select another Textcodec"));
#ifdef Q_WS_WIN
    stuffmenu->addAction(tr("Playername"));
#endif
    stylemenu = traymenu->addMenu(tr("Skins"));
    textschememenu = traymenu->addMenu(tr("Text Schemes"));
    {
        QDir directory(QApplication::applicationDirPath() + "/textschemes/");
        foreach(QString s,directory.entryList(QStringList("*.textscheme"),QDir::Files)) {
            textschememenu->addAction(s);
        }
    }
    textschememenu->addSeparator();
    textschememenu->addAction(tr("Save current textscheme to file"));
    textschememenu->addSeparator();
    textschememenu->setIcon(QIcon("snppictures/skinicon.png"));
    stylemenu->setIcon(QIcon("snppictures/skinicon.png"));
    QDir directory(QApplication::applicationDirPath() + "/qss/");
    foreach(QString s,directory.entryList(QStringList("*.qss"),QDir::Files)) {
        stylemenu->addAction(s);
    }
    snpsetcontains("qss file");
    snpsetcontains("chbautojoin");
    layoutmenu = traymenu->addMenu(tr("Layouts"));
    layoutmenu->setIcon(QIcon("snppictures/arrangementicon.png"));
    layoutmenu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout1.png"), "1");
    layoutmenu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout2.png"), "2");
    layoutmenu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout3.png"), "3");
    languagemenu = traymenu->addMenu(tr("Language"));
    languagemenu->setIcon(QIcon("snppictures/languageicon.png"));
    QDir dir1(QApplication::applicationDirPath() + QDir::separator()
              + "translations" + QDir::separator());
    foreach(QString s,dir1.entryList(QStringList("*.qm"),QDir::Files)) {
        languagemenu->addAction(s);
    }
    joinmenu = traymenu->addMenu(tr("Join"));
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesourgeChannelName);
#endif
    joinmenu->setIcon(QIcon("snppictures/joinicon.png"));
    QAction *a = new QAction(this);
    a->setText(tr("Away mode"));
    a->setIcon(QIcon("snppictures/awayicon.png"));
    traymenu->addAction(a);
    traymenu->addSeparator();
    a0 = traymenu->addAction(tr("Reconnect"));
    a0->setIcon(QIcon("snppictures/reconnect.png"));
    a0 = traymenu->addAction(QDialogButtonBox::tr("&Close"));
    a0->setIcon(QIcon("snppictures/closeicon.png"));        
}

void mainwindow::trayactivation(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (!::window::hiddenchannelwindowshelper.isEmpty()) {
            ::window *w = ::window::hiddenchannelwindowshelper.takeLast();
            w->show();
        } else if (!this->hiddenchatwindowshelper.isEmpty()) {
            chatwindow *w = hiddenchatwindowshelper.takeLast();
            w->show();
            querylist.removeAll(w->chatpartner);
        } else {
            if (this->isHidden()) {
                this->show();
                this->activateWindow();
                this->raise();
            } else if (this->isVisible())
                this->close();
        }
    }
}
void mainwindow::traymenutriggered(QAction *a) {
    if (a->text() == QDialogButtonBox::tr("&Close")) {        
        singleton<quithandler>().inducequit();
        return;    
    } else if (a->text() ==  tr("Select another Textcodec")){
        CodecSelectDia().exec();
    } else if (a->text().startsWith("#")) {
        join(a->text().split(" ").first());
        return;
    } else if (a->text().contains(".qss")) {
        QFile f(QApplication::applicationDirPath() + "/qss/" + a->text());
        QString stylesheet;
        if(a->text()==singleton<snpsettings>().map["qss file"].toString())
            return;
        if (f.open(QFile::ReadOnly)) {
            stylesheet = f.readAll();
            singleton<snpsettings>().map["qss file"] = a->text();
            singleton<snpsettings>().safe();
            safeusergarbage();
            safequerylist();
            try{
                QMessageBox::StandardButton button=QMessageBox::warning(this,QObject::tr("Warning"),
                                                                        tr("Changing the skin crashes sometimes, but The Wheat Snooper\n" \
                                                                           "will keep the settings for the next Start.\nDo you want to proceed?"),
                                                                        QMessageBox::Ok | QMessageBox::Cancel);
                if(button==QMessageBox::Ok)
                    qApp->setStyleSheet(baseStyleSheet+stylesheet);
            }
            catch(...){
                myDebug()<<tr("Skinchanging failed, please try again.");
            }
        }
        return;
    } else if (a->text() == "1") {
        whichuitype = 1;
        singleton<snpsettings>().map["whichuitype"] = whichuitype;
        return;
    } else if (a->text() == "2") {
        whichuitype = 2;
        singleton<snpsettings>().map["whichuitype"] = whichuitype;
        return;
    } else if (a->text() == "3") {
        whichuitype = 3;
        singleton<snpsettings>().map["whichuitype"] = whichuitype;
        return;
    } else if (a->text() == tr("Away mode")) {
        if (net != 0) {
            if (net->isaway) {
                if (!awaybox::ison) {
                    net->isaway = 0;
                    net->wasaway = 0;
                    singleton<balloon_handler>().set_normal_tray_icon();
                    foreach(QString s,rememberwhogotaway.keys()) {
                        if (net->users.users.indexOf(userstruct::whoami(s))
                            != -1
                                    && !compareCI(
                                            net->users.users[net->users.users.indexOf(
                                                    userstruct::whoami(s))].clan,
                                            "username"))
                            net->sendrawcommand("PRIVMSG " + s
                                                + " :\001back\001");
                        if (singleton<settingswindow>().from_map("chbbacktonormals").toBool()
                            && !singleton<snpsettings>().map["ignorelist"].toStringList().contains(
                                    s, Qt::CaseInsensitive))
                            net->sendnotice(
                                    s,
                                    singleton<settingswindow>().from_map("lebackmessage").toString());
                        else if (singleton<settingswindow>().from_map("chbbacktobuddys").toBool()) {
                            if (singleton<snpsettings>().map["buddylist"].toStringList().contains(
                                    s, Qt::CaseInsensitive))
                                net->sendnotice(
                                        s,
                                        singleton<settingswindow>().from_map("lebackmessage").toString());
                        }
                    }
                    rememberwhogotaway.clear();
                    this->awaymessagechanged();
                } else {

                }
            } else {
                if (!awaybox::ison) {
                    awaybox *away = new awaybox;
                    away->show();
                    connect(away, SIGNAL(sigok()),this, SLOT(awayboxok()));
                }
            }
        }
        return;

    } else if (a->text() == tr("About")) {
        about *ab = new about;
        ab->show();
        return;
    } else if (a->text() == tr("Settings")) {
        singleton<settingswindow>().show();
        return;
    } else if (a->text() == tr("Reconnect")) {
        reconnect();
        return;
    } else if (a->text() == usermodel::tr("Buddylist")) {
        buddylist *buddy = new buddylist;
        buddy->show();
    }
    if (a->text() == tr("Volume")) {
        volume->show();
        volume->move(QCursor::pos());
    } else if (a->text().contains(".qm", Qt::CaseInsensitive)) {
        setlanguage(a->text());
    } else if (a->text() == tr("Open Log Browser")) {
        logbrowser *l = new logbrowser;
        l->show();
        connect(l, SIGNAL(sigopenchatwindow(const QString&)),this, SLOT(openchatwindow(const QString&)));
    } else if (a->text().contains(".textscheme", Qt::CaseInsensitive)) {
        settextscheme(a->text());
    } else if (a->text() == tr("Save current textscheme to file")) {
        QString file = QFileDialog::getSaveFileName(this, tr(
                "Save textscheme to."), "textschemes/", "*.textscheme");
        if (file != "") {
            QFileInfo f(file);
            QString file_name=f.fileName();
            if(!file_name.endsWith(".textscheme"))
                file_name+=".textscheme";
            singleton<snpsettings>().map["charformatfile"] = file_name;
            chathandler::initialformatsaver();
            textschememenu->addAction(file_name);
        }
    } else if (a->text() == tr("Scheme maker")) {
        textschemehandler *h = new textschemehandler;
        h->show();
    } else if (a->text() == tr("Playername")) {
        playername *pl = new playername;
        pl->show();
    }
    singleton<snpsettings>().safe();
}
void mainwindow::reconnect(){
    foreach(::window *w,this->windowlist) {
        lastOpenedWindows<<w->currentchannel;
    }
    foreach(chatwindow *w,::window::chatwindows) {
        lastOpenedChatWindows<<w->chatpartner;
    }
    if (net != 0)
        net->isaway = 0;
    singleton<balloon_handler>().set_normal_tray_icon();
    returntotabsettings(0);
    QTimer::singleShot(2000, this, SLOT(chooseclicked()));
}

void mainwindow::on_pbsettings_clicked()
{
    singleton<settingswindow>().show();
}

void mainwindow::on_pbabout_clicked()
{
    about *ab = new about;
    ab->show();
}
void mainwindow::joinGameSourge(){
    //    ircJoinDia dia;
    //    if(!dia.exec())
    //        return;
    //    static irc_netcoupler *net=new irc_netcoupler(ui.lenick->text(), this, dia.getChannel());
    //    static irc_window *window=new irc_window(net,GamesourgeChannelName);
    //    window->show();
    //    window->raise();
}

void mainwindow::on_pbjoin_clicked()
{
    join(ui.cbchannels->currentText());
}

void mainwindow::on_cbenabletus_toggled(bool checked)
{
    if(checked){
        ui.lenick->setEnabled(false);
        ui.rank->setEnabled(false);
        ui.clan->setEnabled(false);
        int i=ui.cbServerList->findText("http://wormnet1.team17.com");
        if(i==-1)
            ui.cbServerList->addItem("http://wormnet1.team17.com");
        i=ui.cbServerList->findText("http://wormnet1.team17.com");
        ui.cbServerList->setCurrentIndex(i);
        ui.cbServerList->setEnabled(false);
        ui.flag->setEnabled(false);
        ui.letuspassword->setEnabled(true);
        ui.letuslogin->setEnabled(true);
        ui.cbleagueservers->setEnabled(true);
    } else{
        ui.flag->setEnabled(true);
        ui.lenick->setEnabled(true);
        ui.rank->setEnabled(true);
        ui.clan->setEnabled(true);
        ui.cbServerList->setEnabled(true);;
        ui.letuspassword->setEnabled(false);
        ui.letuslogin->setEnabled(false);
        ui.cbleagueservers->setEnabled(false);
    }
}
