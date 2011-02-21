#include"mainwindow.h"
#include"window.h"
#include"netcoupler.h"
#include"buddylist.h"
#include"ctcphandler.h"
#include"chathandler.h"
#include"buttonlayout.h"
#include"awaybox.h"
#include"inihandlerclass.h"
#include"chatwindow.h"
#include"about.h"
#include"settings.h"
#include"charformatsettings.h"
#include"ui_window.h"
#include"ui_window2.h"
#include"ui_window3.h"
#include"settingswindow.h"
#include"volumeslider.h"
#include"logbrowser.h"
#include"textschemehandler.h"
#include"playername.h"
#include"singleton.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"balloon_handler.h"
#include"codecselectdia.h"
#include"clantowebpagemapper.h"
#include"leagueserverhandler.h"
#include"picturehandler.h"
#include"quithandler.h"
#include"qobjectwrapper.h"
#include"awayhandler.h"

#ifdef WITH_GAMESURGE_SUPPORT
#include"src/irc/irc_netcoupler.h"
#include"src/irc/irc_window.h"
#include"src/irc/ircjoindia.h"
#endif

#include<QDir>
#include<QStringList>
#include<QSystemTrayIcon>
#include<QMenu>
#include<QMessageBox>
#include<QPicture>
#include<QDialogButtonBox>
#include<QTime>
#include<QFileDialog>
#include<QDesktopServices>

inihandlerclass inihandler;
extern volumeslider *volume;
bool fontorcolorchanged = 0;
extern QStringList querylist;
QString GamesourgeChannelName="#GameSourgeWorms";
mainwindow::mainwindow() {
    whichuitype = 1;
    ui.setupUi(this);
    bool b=S_S.getbool("righttoleftwriting");
    if(b)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
    ui.pbrememberjoin->setText(tr("Autojoin:"));
    ui.pbabout->setText(tr("About"));
    ui.start->setText(tr("Apply"));
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
    snpsetcontains("enablesecurelogging");
    snpsetcontains("whichuitype");
    snpsetcontains("countrycode");
    snpsetcontains("information");
    snpsetcontains("clan");
    snpsetcontains("wormnetserverlist");
    snpsetcontains("rank");
    snpsetcontains("leagueservers");
    snpsetcontains("showinformation");

    ui.cbremember->setChecked(S_S.getbool("chbremember"));
    if (ui.cbremember->isChecked())
        chooseclicked();    
    setWindowTitle(tr("Wheat Snoopers root window."));
    joinonstartup = 0;
    connect(ui.start, SIGNAL(clicked()),this, SLOT(chooseclicked()));
    connect(ui.pbrememberjoin, SIGNAL(clicked()),this, SLOT(pbrememberjoinclicked()));
    connect(ui.tabWidget, SIGNAL(currentChanged ( int )),this, SLOT(currenttabchanged(int)));

    connect(&singleton<netcoupler>(), SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(const QStringList &)),this, SLOT(getchannellist(const QStringList &)));
    connect(&qobjectwrapper<awayhandler>::ref(), SIGNAL(sigawaystringchanged()),this, SLOT(awaymessagechanged()));
    connect(&singleton<netcoupler>(), SIGNAL(siggotprivmsg(const QString&,const QString&,const QString&)),this,
            SLOT(gotprvmsg(const QString&,const QString&,const QString&)));
    connect(&singleton<netcoupler>(), SIGNAL(sigconnected()),this,SLOT(connected()));
    connect(&singleton<netcoupler>(), SIGNAL(sigdisconnected()),this,SLOT(disconnected()));

    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginfailed()),this,SLOT(leagueserverconnectionfailed()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginsuccess()),this,SLOT(leagueserverconnectionsuccess()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigprofile(QString)),this,SLOT(leagueserverprofilepage(QString)));

    QRegExp regexp;
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`){,15}");
    validator = new QRegExpValidator(regexp, 0);
    ui.lenick->setValidator(validator);
    ui.clan->setValidator(validator);

    //    S_S.set("isarrangedbyonetab", false);
    //    S_S.set("isarrangedbytwotabs", false);
    debugmsg.clear();

    connect(&singleton<ctcphandler>(), SIGNAL(sigctcpcommand(const QString&,const QString&)),this, SLOT(gotctcpsignal(const QString&,const QString&)));

    QVariantList windowstates = S_S.getlist("MainWindowGeometry");
    if (!windowstates.isEmpty())
        restoreGeometry(windowstates.takeFirst().toByteArray());
    if(height()<530)
        resize(width(),530);

    qApp->installEventFilter(this);
}
mainwindow::~mainwindow() {
}
void mainwindow::currenttabchanged(int i){
    if(i!=0)
        return;
    singleton<netcoupler>().stop();
}
void mainwindow::fillsnpsettings(){
    S_S.set("chbremember", ui.cbremember->isChecked());
    S_S.set("nickname", ui.lenick->text());
    S_S.set("chbminimized", ui.chbminimized->isChecked());
    S_S.set("chbautojoin", ui.chbautojoin->isChecked());
    S_S.set("whichuitype", whichuitype);
    S_S.set("countrycode", ui.flag->currentText());
    S_S.set("rank", ui.rank->currentText());
    if (fontorcolorchanged == 1) {
        S_S.set("charformatfile", "lastedited.textscheme");
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
    if(S_S.enablesecurelogging)
        singleton<netcoupler>().start(singleton<leagueserverhandler>().nick);
    else
        singleton<netcoupler>().start(ui.lenick->text());
    QTimer::singleShot(1000,this,SLOT(reopenChatWindowsAndChannelWindows()));
    ui.tabWidget->setTabEnabled(1, 1);
}
void mainwindow::chooseclicked() {
    if(ui.tabWidget->currentIndex()!=0)
        return;
    if (ui.lenick->text().isEmpty() && !ui.cbenabletus->isChecked()){
        QMessageBox::warning(this,tr("Nickname field is empty"),tr("Please choose a nickname."),QMessageBox::Ok);
        return;
    }
    S_S.set("clan", ui.clan->text());
    if (ui.clan->text().isEmpty())
        S_S.set("clan", "Username");
    S_S.set("rank", ui.rank->currentText());
    S_S.set("nickname", ui.lenick->text());
    S_S.set("tus_password", ui.letuspassword->text());
    S_S.set("tus_login", ui.letuslogin->text());
    S_S.set("countrycode", ui.flag->currentText());
    S_S.set("rank", ui.rank->currentText());
    S_S.set("information", ui.client->text());
    S_S.set("enablesecurelogging", ui.cbenabletus->isChecked());    

    S_S.set("wormnetserverlist", refreshcombobox(ui.cbServerList));
    S_S.set("leagueservers", refreshcombobox(ui.cbleagueservers));
    S_S.set("showinformation", ui.cbshowinformation->isChecked());
    S_S.set("leagueservers"+ui.cbleagueservers->currentText(), QStringList()<<ui.letuslogin->text()<<ui.letuspassword->text());

    if(S_S.enablesecurelogging){
        S_S.set("spectateleagueserver", true);
        setleague();
        singleton<leagueserverhandler>().login(ui.letuslogin->text(),ui.letuspassword->text());
    } else {
        if(S_S.getbool("spectatingneversettedoff")){
            S_S.set("spectateleagueserver", true);
            singleton<leagueserverhandler>().setleague("http://www.tus-wa.com/","http://www.tus-wa.com/");
            singleton<leagueserverhandler>().startrefresh();
        } else
            S_S.set("spectateleagueserver", false);
        connectToNetwork();
    }
}
void mainwindow::setleague(){
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
    singleton<netcoupler>().joinchannel(channel);
    if (whichuitype == 1) {
        windowlist.push_back(new ::window(&singleton<netcoupler>(), channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else if (whichuitype == 2) {
        windowlist.push_back(new ::window(&singleton<netcoupler>(), channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else if (whichuitype == 3) {
        windowlist.push_back(new ::window(&singleton<netcoupler>(), channel, whichuitype));
        windowlist.last()->setObjectName("channelwindow");
    } else
        myDebug() << "joinclicked in mainwindow assert";
    if(windowlist.isEmpty())
        return;
    windowlist.last()->show();
    windowlist.last()->raise();
    if (qobjectwrapper<awayhandler>::ref().away()) {
        windowlist.last()->windowtitleaway = qobjectwrapper<awayhandler>::ref().message();
        windowlist.last()->mysetwindowtitle();
    }
    connect(windowlist.last(), SIGNAL(sigjoinchannel(const QString&)),this, SLOT(join(const QString&)));
    connect(windowlist.last(), SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
    connect(this, SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
    connect(windowlist.last(), SIGNAL(sigwindowclosed(const QString&)),this, SLOT(windowremoved(const QString&)));
}
void mainwindow::quit() {
    foreach(chatwindow *w,::window::chatwindows) {
        Q_ASSERT(w!=0);
        w->close();
        w->deleteLater();
    }
    foreach(::window *w, windowlist) {
        Q_ASSERT(w!=0);
        w->close();
        w->deleteLater();
    }
    fillsnpsettings();
}
void mainwindow::closeEvent(QCloseEvent *) {
    QVariantList windowstates;
    windowstates << saveGeometry();
    S_S.set("MainWindowGeometry", windowstates);
}
void mainwindow::returntologintab() {
    singleton<leagueserverhandler>().reset();
    joinonstartup = 0;
    S_S.set("chbautojoin", ui.chbautojoin->isChecked());
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
    ::window::chatwindows.clear();
    foreach(::window *w,windowlist) {
        Q_ASSERT(w!=0);
        w->deleteLater();
    }
    windowlist.clear();
    ui.tabWidget->setTabEnabled(1, 0);
}
void mainwindow::changeEvent(QEvent * event) {
    if (event->type() == QEvent::WindowStateChange) {
        if ( isMinimized()) {
            QTimer::singleShot(200, this, SLOT(close()));
            return;
        }
    }
    QWidget::changeEvent(event);
}
void mainwindow::pbrememberjoinclicked() {
    S_S.set("joinonstartup", ui.cbchannels->currentText());
    ui.pbrememberjoin->setText(tr("Autojoin:") + "\n" + S_S.joinonstartup);
}
void mainwindow::snpsetcontains(const QString &s) {
    if (s == "chbautojoin" && S_S.contains(s))
        ui.chbautojoin->setChecked(S_S.chbautojoin);
    else if (s == "qss_file") {
        QFile f(QApplication::applicationDirPath() + "/qss/" + S_S.qss_file);
        if(!f.open(QFile::ReadOnly))
            QMessageBox::warning(this,QObject::tr("Warning"),tr("Cant read the Skinfile:\n")+S_S.qss_file);
        QString stylesheet = QLatin1String(f.readAll());
        qApp->setStyleSheet(baseStyleSheet+stylesheet);
    } else if (s == "joinonstartup" && S_S.contains(s) && S_S.contains("chbautojoin")) {
        ui.pbrememberjoin->setText(ui.pbrememberjoin->text().split("\n").first() + "\n" + S_S.joinonstartup);
        if (S_S.chbautojoin) {
            join(S_S.joinonstartup);
        }
    } else if (s == "chbminimized")
        ui.chbminimized->setChecked(S_S.chbminimized);
    else if (s == "nickname" && S_S.contains(s))
        ui.lenick->setText(S_S.nickname);
    else if (s == "tus_password" && S_S.contains("tus_password"))
        ui.letuspassword->setText(S_S.tus_password);
    else if (s == "tus_login" && S_S.contains("tus_login"))
        ui.letuslogin->setText(S_S.tus_login);
    else if (s == "countrycode"){
        QString language=QLocale::system().name().left(2);
        int i=ui.flag->findText(S_S.countrycode);
        if(i==-1)
            i=ui.flag->findText(language);
        ui.flag->setCurrentIndex(i);
    }else if (s == "rank"){
        ui.rank->setCurrentIndex(S_S.rank);
    }
    else if (s == "information" && S_S.contains(s))
        ui.client->setText(S_S.getstring(s));
    else if (s == "clan" && S_S.contains(s)){
        QString clanstring=S_S.getstring(s);
        if(clanstring.toLower()=="username")
            ui.clan->setText("");
        else
            ui.clan->setText(clanstring);
    }
    else if (s == "whichuitype" && S_S.contains(s))
        whichuitype = S_S.whichuitype;
    else if (s == "wormnetserverlist"){
        ui.cbServerList->addItems(S_S.wormnetserverlist);
    }
    else if (s == "leagueservers"){
        ui.cbleagueservers->addItems(S_S.leagueservers);
    }
    else if(s=="enablesecurelogging"){
        bool b=S_S.enablesecurelogging;
        ui.cbenabletus->setChecked(b);
        on_cbenabletus_toggled(b);
    } else if(s=="showinformation"){
        bool b=S_S.showinformation;
        ui.cbshowinformation->setChecked(b);
    }
}
void mainwindow::windowremoved(const QString &s) {
    currentchannellist.removeAll(s);
    windowlist.removeAll(qobject_cast<channelwindow*> (sender()));
}
void mainwindow::usesettingswindow(const QString&) {
}
void mainwindow::chatwinowclosed() {
    chatwindow *w = qobject_cast<chatwindow*> (sender());
    Q_CHECK_PTR(w);
    w->disconnect();
    int i = window::chatwindowstringlist.removeAll(w->chatpartner);
    i = window::chatwindows.removeAll(w);
}
void mainwindow::appenddebugmessage(const QString &msg) {
    debugmsg.append(msg);
    ui.labeltraydescription->insertPlainText(debugmsg);
    foreach( ::window *w,windowlist)
        w->gotdebugmsg(debugmsg);    
    debugmsg.clear();
}
void mainwindow::setlanguage(const QString &langfile) {
    S_S.set("language_file", langfile);
    myDebug() << S_S.language_file;
    QMessageBox::StandardButton button = QMessageBox::question(this, tr(
            "Restart the application?"), tr(
                    "Changing the translation requires a program restart.\n"
                    "To restart now click Ok, otherwise cancel."), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
    if (button == QMessageBox::Ok) {
        returntologintab();
        singleton<netcoupler>().stop();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList(),
                                QApplication::applicationDirPath());
        foreach(::window *w,windowlist) {
            w->close();
        }
        singleton<balloon_handler>().tray->hide();
        singleton<quithandler>().inducequit();
    }
}
void mainwindow::awayboxok() {
    foreach(::window *w,windowlist) {
        w->windowtitleaway = " " + tr("<away>:") +" "+ qobjectwrapper<awayhandler>::ref().message();
        w->mysetwindowtitle();
    }
}
void mainwindow::awaymessagechanged() {
    if (qobjectwrapper<awayhandler>::ref().away()) {
        foreach(::window *w,windowlist) {
            w->windowtitleaway = " " + tr("<away>:") + " "+qobjectwrapper<awayhandler>::ref().message();
            w->mysetwindowtitle();
        }
    } else {
        foreach(::window *w,windowlist) {
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
    S_S.set("charformatfile", file);
    singleton<charformatsettings>().load();
    chathandler::initialformatstarter();
}
void mainwindow::openchatwindow(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(&singleton<netcoupler>(), s));
    window::chatwindows.last()->show();
    connect(window::chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
void mainwindow::openchatwindowhidden(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(&singleton<netcoupler>(), s));
    mainwindow::hiddenchatwindowshelper << window::chatwindows.last();
    connect(window::chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
void mainwindow::gotprvmsg(const QString &user, const QString &receiver, const QString &msg) {
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
    if (qobjectwrapper<awayhandler>::ref().away()) {
        qobjectwrapper<awayhandler>::ref().sendaway(user);
    }
    if (!singleton<netcoupler>().users.usermap[usermodel::tr("Ignorelist")].count(
            userstruct::whoami(user))
        && !::window::chatwindowstringlist.contains(user,
                                                    Qt::CaseInsensitive)) {
        singleton<balloon_handler>().got_privmsg(user, msg);
    }
    if (!containsCI(window::chatwindowstringlist, user)) {
        if ((singleton<netcoupler>().users.usermap[usermodel::tr("Buddylist")].count(
                userstruct::whoami(user)) || mainwindow::windowlist.isEmpty() || S_S.chballwaysopenchatwindows)
            && !singleton<netcoupler>().users.usermap[usermodel::tr("Ignorelist")].count(
                    userstruct::whoami(user))) {
            Q_ASSERT(user!="");
            if (S_S.chbstartchatsminimized) {
                if (mainwindow::windowlist.isEmpty())
                    singleton<sound_handler>().play_normalmsgsound(user);
                else
                    foreach(::window *w,mainwindow::windowlist)
                        w->gotprvmsg(user, receiver, msg);                                    
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
void mainwindow::connected(){
    ui.start->setEnabled(false);
    ui.start->setText("");
    ui.connectlabel->setText(tr("Connected"));
}
void mainwindow::disconnected(){
    ui.start->setText(tr("Apply"));
    ui.start->setEnabled(true);
    if(singleton<netcoupler>().connectstate==netcoupler::e_started)
        QTimer::singleShot(200 *1000, this, SLOT(reconnect()));
    ui.connectlabel->setText(tr("Disconnected"));
    singleton<leagueserverhandler>().logout();
    returntologintab();
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
    traymenu = new QMenu;
    connect(traymenu,SIGNAL(triggered(QAction*)),this,SLOT(traymenutriggered(QAction*)));
    ui.pbtraymenu->setMenu(traymenu);
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
    snpsetcontains("qss_file");
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
        } else if (! hiddenchatwindowshelper.isEmpty()) {
            chatwindow *w = hiddenchatwindowshelper.takeLast();
            w->show();
            querylist.removeAll(w->chatpartner);
        } else {
            if ( isHidden()) {
                show();
                activateWindow();
                raise();
            } else if ( isVisible())
                close();
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
        if(a->text()==S_S.qss_file)
            return;
        if (f.open(QFile::ReadOnly)) {
            stylesheet = f.readAll();
            S_S.set("qss_file", a->text());
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
        S_S.set("whichuitype", whichuitype);
        QMessageBox::information(0,tr("Information"),tr("You must reopen the channelwindows to change the layout."));
        return;
    } else if (a->text() == "2") {
        whichuitype = 2;
        S_S.set("whichuitype", whichuitype);
        QMessageBox::information(0,tr("Information"),tr("You must reopen the channelwindows to change the layout."));
        return;
    } else if (a->text() == "3") {
        whichuitype = 3;
        S_S.set("whichuitype", whichuitype);
        QMessageBox::information(0,tr("Information"),tr("You must reopen the channelwindows to change the layout."));
        return;
    } else if (a->text() == tr("Away mode")) {
        handleAwayBox();
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
            S_S.set("charformatfile", file_name);
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
}
void mainwindow::handleAwayBox(){
    if (qobjectwrapper<awayhandler>::ref().away()) {
        if (!awaybox::ison) {
            qobjectwrapper<awayhandler>::ref().back();
            awaymessagechanged();
        }
    } else {
        if (!awaybox::ison) {
            awaybox *away = new awaybox;
            away->show();
            connect(away, SIGNAL(sigok()),this, SLOT(awayboxok()));
        }
    }
}
void mainwindow::reconnect(){
    disconnect(this,SLOT(reconnect()));
    foreach(::window *w, windowlist) {
        lastOpenedWindows<<w->currentchannel;
    }
    foreach(chatwindow *w,::window::chatwindows) {
        lastOpenedChatWindows<<w->chatpartner;
    }    
    returntologintab();
    singleton<netcoupler>().stop();
    QTimer::singleShot(2000, this, SLOT(chooseclicked()));
}
void mainwindow::on_pbsettings_clicked(){
    singleton<settingswindow>().show();
}

void mainwindow::on_pbabout_clicked(){
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

void mainwindow::on_pbjoin_clicked(){
    join(ui.cbchannels->currentText());
}

void mainwindow::on_cbenabletus_toggled(bool checked){
    if(checked) {
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
    } else {
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

void mainwindow::on_pbeditleagueprofile_clicked(){
    setleague();
    if(ui.letuslogin->text().isEmpty()){
        QMessageBox::information(0,QObject::tr("Information"),tr("Before you can view your profile, you must fill in a loginname."));
        ui.letuslogin->setFocus(Qt::MouseFocusReason);
        return;
    }
    singleton<leagueserverhandler>().profile(ui.letuslogin->text());
}
void mainwindow::leagueserverprofilepage(QString url){
    QDesktopServices::openUrl(QUrl(url));
}
void mainwindow::on_cbleagueservers_activated(QString s){
    QStringList sl=S_S.getstringlist("leagueservers"+s);
    if(sl.size()<2)
        return;
    ui.letuslogin->setText(sl.takeFirst());
    ui.letuspassword->setText(sl.takeFirst());
}

void mainwindow::on_chbautojoin_clicked(bool checked){
    if(checked)
        pbrememberjoinclicked();
}
