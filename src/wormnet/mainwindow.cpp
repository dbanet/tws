#include "mainwindow.h"
#include "window.h"
#include "netcoupler.h"
#include "buddylist.h"
#include "ctcphandler.h"
#include "chathandler.h"
#include "awaybox.h"
#include "inihandlerclass.h"
#include "chatwindow.h"
#include "about.h"
#include "settings.h"
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
#include "codecselectdia.h"
#include "clantowebpagemapper.h"
#include "leagueserverhandler.h"
#include "picturehandler.h"
#include "quithandler.h"
#include "qobjectwrapper.h"
#include "awayhandler.h"
#include "usermessage.h"

#ifdef WITH_GAMESURGE_SUPPORT
#include "src/irc/irc_netcoupler.h"
#include "src/irc/irc_window.h"
#include "src/irc/ircjoindia.h"
#endif

#include <QTextCodec>
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include <QPicture>
#include <QDialogButtonBox>
#include <QTime>
#include <QFileDialog>
#include <QDesktopServices>

inihandlerclass inihandler;
extern volumeslider *volume;
bool fontOrColorHasChanged = 0;
extern QStringList querylist;
QString GamesurgeChannelName="#worms";
mainwindow::mainwindow() {
    whichUIType = 1;
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

    getBaseStyleSheet();
    initMenus();
    connect(singleton<balloonHandler>().tray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason)),this, SLOT(trayActivation(QSystemTrayIcon::ActivationReason)));
    singleton<pictureHandler>().fillFlags(ui.flag);
    singleton<pictureHandler>().fillRanks(ui.rank);
    snpSetContains("chbminimized");
    snpSetContains("nickname");
    snpSetContains("tus_password");
    snpSetContains("tus_login");
    snpSetContains("enablesecurelogging");
    snpSetContains("whichuitype");
    snpSetContains("countrycode");
    snpSetContains("information");
    snpSetContains("clan");
    snpSetContains("wormnetserverlist");
    snpSetContains("rank");
    snpSetContains("leagueservers");
    snpSetContains("showinformation");

    ui.cbremember->setChecked(S_S.getbool("chbremember"));
    if (ui.cbremember->isChecked())
        chooseClicked();
    setWindowTitle(tr("Wheat Snoopers root window."));
    joinOnStartUp = 0;
    connect(ui.start, SIGNAL(clicked()),this, SLOT(chooseClicked\()));
    connect(ui.tabWidget, SIGNAL(currentChanged ( int )),this, SLOT(currentTabChanged(int)));

    connect(&singleton<netcoupler>(), SIGNAL(sigSettingsWindowChanged()),this, SLOT(useSettingsWindow()));
    connect(&singleton<netcoupler>(), SIGNAL(sigGotChanList(const QStringList &)),this, SLOT(getChannelList(const QStringList &)));
    connect(&qobjectwrapper<awayhandler>::ref(), SIGNAL(sigawaystringchanged()),this, SLOT(awayMessageChanged()));
    connect(&singleton<netcoupler>(), SIGNAL(sigConnected()),this,SLOT(connected()));
    connect(&singleton<netcoupler>(), SIGNAL(sigDisconnected()),this,SLOT(disconnected()));
    connect(&singleton<netcoupler>(), SIGNAL(sigGotUserMessage(const usermessage)),this,SLOT(gotUserMessage(const usermessage)));

    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginfailed()),this,SLOT(leagueServerConnectionFailed()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginsuccess()),this,SLOT(leagueServerConnectionSuccess()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigprofile(QString)),this,SLOT(leagueServerProfilePage(QString)));

    QRegExp regexp;
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`){1,15}");
    validator = new QRegExpValidator(regexp, 0);
    ui.lenick->setValidator(validator);
    ui.clan->setValidator(validator);

    //    S_S.set("isarrangedbyonetab", false);
    //    S_S.set("isarrangedbytwotabs", false);
    debugMsg.clear();

    QVariantList windowstates = S_S.getlist("MainWindowGeometry");
    if (!windowstates.isEmpty())
        restoreGeometry(windowstates.takeFirst().toByteArray());
    if(height()<530)
        resize(width(),530);
    setWindowFlags (Qt::Dialog);
}
mainwindow::~mainwindow() {
}
void mainwindow::currentTabChanged(int i){
    if(i!=0)
        return;
    foreach(channelWindow *w, windowList){
        singleton<netcoupler>().partChannel(w->currentChannel);
        w->close();
    }
    singleton<netcoupler>().stop();
}
void mainwindow::fillsnpsettings(){
    S_S.set("chbremember", ui.cbremember->isChecked());
    S_S.set("nickname", ui.lenick->text());
    S_S.set("chbminimized", ui.chbminimized->isChecked());
    S_S.set("chbautojoin", ui.chbautojoin->isChecked());
    S_S.set("whichuitype", whichUIType);
    S_S.set("countrycode", ui.flag->currentText());
    S_S.set("rank", ui.rank->currentText());
    if (fontOrColorHasChanged == 1) {
        S_S.set("textscheme", "lastedited.textscheme");
        singleton<chatFormatSettings>().safe();
    }
}
void mainwindow::getBaseStyleSheet(){
    QFile f(QApplication::applicationDirPath() + "/qss/Skin_Base");
    if(!f.open(QFile::ReadOnly))
        QMessageBox::warning(0,QObject::tr("Warning"),QObject::tr("Cant read the Skinfile:\nSkin_Base"));
    baseStyleSheet = QLatin1String(f.readAll());
}
void mainwindow::connectToNetwork(){
    ui.tabWidget->setCurrentIndex(1);
    if(S_S.getbool("enablesecurelogging"))
        singleton<netcoupler>().start(singleton<leagueserverhandler>().nick);
    else
        singleton<netcoupler>().start(ui.lenick->text());    
    ui.tabWidget->setTabEnabled(1, 1);
}
void mainwindow::chooseClicked() {
    if(ui.tabWidget->currentIndex()!=0)
        return;
    if (ui.lenick->text().isEmpty() && !ui.cbenabletus->isChecked()){
        QMessageBox::warning(this,tr("Nickname field is empty"),tr("Please choose a nickname."),QMessageBox::Ok);
        return;
    }
    S_S.transaction();
    S_S.set("clan", ui.clan->text());
    if (ui.clan->text().isEmpty())
        S_S.set("clan", "UserName");
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
    S_S.set("leagueservers"+makeValidColumnName(ui.cbleagueservers->currentText()), QStringList()<<ui.letuslogin->text()<<ui.letuspassword->text());

    if(S_S.getbool("enablesecurelogging")){
        S_S.set("spectateleagueserver", true);
        setLeague();
        singleton<leagueserverhandler>().login(ui.letuslogin->text(),ui.letuspassword->text());
    } else {        
        S_S.set("spectateleagueserver", false);
        connectToNetwork ();
    }
    ui.start->setEnabled(false);
    ui.start->setText("");
    S_S.commit();
}
void mainwindow::setLeague(){
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

void mainwindow::leagueServerConnectionFailed(){
    ui.start->setText(tr("Apply"));
    ui.start->setEnabled(true);
}
void mainwindow::leagueServerConnectionSuccess(){
    connectToNetwork();
}
void mainwindow::reopenChatWindowsAndChannelWindows(){
    foreach(QString s,lastOpenedChatWindows)
        openChatWindowRaised(s);
    lastOpenedChatWindows.clear();
    foreach(QString s,lastOpenedWindows)
        join(s);
    lastOpenedWindows.clear();
}
void mainwindow::getChannelList(const QStringList &sl) {
    ui.cbchannels->clear();
    foreach(QString s,sl)
        ui.cbchannels->addItem(s);
    channelList = sl;
    ui.pbjoin->setEnabled(1);
    joinMenu->clear();
    foreach(QString s,channelList) {
        joinMenu->addAction(s);
    }
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesurgeChannelName);
#endif
    if (joinOnStartUp == 0) {
        snpSetContains("joinonstartup");
        joinOnStartUp = 1;
    }
}
void mainwindow::join(const QString channel){
#ifdef WITH_GAMESURGE_SUPPORT
    if(channel==GamesurgeChannelName){
        joinGameSurge();
        return;
    }
#endif
    if(joinedChannels.contains(channel))
        return;
    joinedChannels << channel;
    Q_ASSERT_X(whichUIType!=1||whichUIType!=2||whichUIType!=3,"void mainWindow::join(const QString channel)","whichUIType is not equal to 1, 2 or 3");
    windowList.push_back(new channelWindow(channel,whichUIType));
    windowList.last()->setObjectName("channelwindow");
    singleton<netcoupler>().joinChannel(channel);
    if(windowList.isEmpty())
        return;
    windowList.last()->show();
    windowList.last()->raise();
    if (qobjectwrapper<awayhandler>::ref().away()) {
        windowList.last()->windowAwayTitle = qobjectwrapper<awayhandler>::ref().message();
        windowList.last()->updateWindowTitle();
    }
    connect(windowList.last(), SIGNAL(sigJoinChannel(const QString&)),this, SLOT(join(const QString&)));
    connect(windowList.last(), SIGNAL(sigOpenChatWindow(const QString &)),this, SLOT(openChatWindowRaised(const QString &)));
    connect(this, SIGNAL(sigOpenChatWindow(const QString &)),this, SLOT(openChatWindowRaised(const QString &)));
    connect(windowList.last(), SIGNAL(sigChangeLeagueState()),this, SLOT(reconnect()));
    connect(windowList.last(), SIGNAL(sigClosed()),this, SLOT(windowClosed()));
}
void mainwindow::windowClosed(){
    channelWindow *w=qobject_cast<channelWindow*> (sender());
    Q_CHECK_PTR(w);
    w->hiddenChannelWindowsHelper.removeAll(w);
    joinedChannels.removeAll(w->currentChannel);
    windowList.removeAll(w);
}
void mainwindow::quit() {
    foreach(chatwindow *w,channelWindow::chatWindowsList) {
        Q_ASSERT(w!=0);
        w->close();        
    }
    foreach(channelWindow *w, windowList) {
        Q_ASSERT(w!=0);
        w->close();
    }
    fillsnpsettings();
}
void mainwindow::closeEvent(QCloseEvent * e) {
    QVariantList windowstates;
    windowstates << saveGeometry();
    S_S.set("MainWindowGeometry", windowstates);
    e->ignore ();
    hide();
}
void mainwindow::returnToLoginTab() {
    joinOnStartUp = 0;
    S_S.set("chbautojoin", ui.chbautojoin->isChecked());
    joinMenu->clear();
    joinedChannels.clear();
    ui.cbchannels->clear();
    ui.pbjoin->setEnabled(0);
    ui.tabWidget->setTabEnabled(1, 0);
    ui.pbjoin->setEnabled(0);
    foreach(chatwindow *w,channelWindow::chatWindowsList) {
        Q_ASSERT(w);
        w->close();
    }
    channelWindow::chatWindowsList.clear();
    foreach(channelWindow *w,windowList)
        w->close();
    ui.tabWidget->setTabEnabled(1, 0);
}
void mainwindow::on_pbrememberjoin_clicked() {
    S_S.set("joinonstartup", ui.cbchannels->currentText());
    ui.pbrememberjoin->setText(tr("Autojoin:") + "\n" + S_S.getString("joinonstartup"));
}
void mainwindow::snpSetContains(const QString &s) {
    if (s == "chbautojoin" && S_S.contains(s))
        ui.chbautojoin->setChecked(S_S.getbool("chbautojoin"));
    else if (s == "qss_file") {
        QFile f(QApplication::applicationDirPath() + "/qss/" + S_S.getString("qss_file"));
        if(!f.open(QFile::ReadOnly))
            QMessageBox::warning(this,QObject::tr("Warning"),tr("Cant read the Skinfile:\n")+S_S.getString("qss_file"));
        QString stylesheet = QLatin1String(f.readAll());
        qApp->setStyleSheet(baseStyleSheet+stylesheet);
    } else if (s == "joinonstartup" && S_S.contains(s) && S_S.contains("chbautojoin")) {
        ui.pbrememberjoin->setText(ui.pbrememberjoin->text().split("\n").first() + "\n" + S_S.getString("joinonstartup"));
        if (S_S.getbool("chbautojoin"))
            join(S_S.getString("joinonstartup"));
    } else if (s == "chbminimized")
        ui.chbminimized->setChecked(S_S.getbool("chbminimized"));
    else if (s == "nickname" && S_S.contains(s))
        ui.lenick->setText(S_S.getString("nickname"));
    else if (s == "tus_password" && S_S.contains("tus_password"))
        ui.letuspassword->setText(S_S.getString("tus_password"));
    else if (s == "tus_login" && S_S.contains("tus_login"))
        ui.letuslogin->setText(S_S.getString("tus_login"));
    else if (s == "countrycode"){
        QString language=QLocale::system().name().left(2); int i;
        if(-1==(i=ui->flag->findText(S_S.getString("countrycode"))))
            if(-1==(i=ui->flag->findText(language)))
                ui->flag->setCurrentIndex(ui->flag->findText("q"));
            else
                ui->flag->setCurrentIndex(i);
        else
            ui->flag->setCurrentIndex(i);
    }else if (s == "rank"){
        ui.rank->setCurrentIndex(S_S.getint("rank"));
    }
    else if (s == "information" && S_S.contains(s))
        ui.client->setText(S_S.getString(s));
    else if (s == "clan" && S_S.contains(s)){
        QString clanstring=S_S.getString(s);
        if(clanstring.toLower()=="username")
            ui.clan->setText("");
        else
            ui.clan->setText(clanstring);
    }
    else if (s == "whichuitype" && S_S.contains(s))
        whichUIType = S_S.getint("whichuitype");
    else if (s == "wormnetserverlist")
        ui.cbServerList->addItems(S_S.getStringList("wormnetserverlist"));
    else if (s == "leagueservers"){
        ui.cbleagueservers->addItems(S_S.getStringList("leagueservers"));
    }
    else if(s=="enablesecurelogging"){
        bool b=S_S.getbool("enablesecurelogging");
        ui.cbenabletus->setChecked(b);
        on_cbenabletus_toggled(b);
    } else if(s=="showinformation"){
        bool b=S_S.getbool("showinformation");
        ui.cbshowinformation->setChecked(b);
    }
}
void mainwindow::useSettingsWindow(const QString&) {
}
void mainwindow::chatWindowClosed() {
    chatwindow *w = qobject_cast<chatwindow*> (sender());
    Q_CHECK_PTR(w);
    w->disconnect();
    hiddenChatWindowsHelper.removeAll(w);
    window::chatWindowsStringList.removeAll(w->chatpartner);
    window::chatWindowsList.removeAll(w);
}
void mainwindow::appendDebugMessage(const QString &msg) {
    debugMsg.append(msg);
    ui.labeltraydescription->insertPlainText(debugMsg);
    if(!S_S.cbservermessageinchannelwindows)
        return;
    foreach( channelWindow *w,windowList)
        w->gotdebugmsg(debugMsg);
    debugMsg.clear();
}
void mainwindow::setLanguage(const QString &langfile) {
    S_S.set("language_file", langfile);
    myDebug() << S_S.getString("language_file");
    QMessageBox::StandardButton button = QMessageBox::question(this, tr(
                                                                   "Restart the application?"), tr(
                                                                   "Changing the translation requires a program restart.\n"
                                                                   "To restart now click Ok, otherwise cancel."), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
    if (button == QMessageBox::Ok) {
        returnToLoginTab();
        singleton<netcoupler>().stop();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList(), QApplication::applicationDirPath());
        foreach(channelWindow *w,windowList)
            w->close();
        singleton<balloonHandler>().tray->hide();
        singleton<quithandler>().inducequit();
    }
}
void mainwindow::awayBoxOk() {
    foreach(channelWindow *w,windowList) {
        w->windowAwayTitle = " " + tr("<away>:") +" "+ qobjectwrapper<awayhandler>::ref().message();
        w->updateWindowTitle();
    }
}
void mainwindow::awayMessageChanged() {
    if (qobjectwrapper<awayhandler>::ref().away()) {
        foreach(channelWindow *w,windowList) {
            w->windowAwayTitle = " " + tr("<away>:") + " "+qobjectwrapper<awayhandler>::ref().message();
            w->updateWindowTitle();
        }
    } else {
        foreach(channelWindow *w,windowList) {
            w->windowAwayTitle = "";
            w->updateWindowTitle();
        }
    }
}
void mainwindow::setTextScheme(const QString &file) {
    myDebug() << tr("trying to apply new textscheme: ") + file;
    S_S.set("textscheme", file);
    singleton<chatFormatSettings>().load();
    chatHandler::initialFormatStarter();
}
void mainwindow::openChatWindow(QString user){
    window::chatWindowsStringList << user;
    window::chatWindowsList.push_back(new chatwindow(user));
    connect(window::chatWindowsList.last(), SIGNAL(sigClosed()),this, SLOT(chatWindowClosed()));
}
void mainwindow::openChatWindowRaised(const QString &user) {
    openChatWindow(user);
    window::chatWindowsList.last()->show();
    window::chatWindowsList.last()->raise();
}
void mainwindow::openChatWindowHidden(const QString &user) {
    openChatWindow(user);
    hiddenChatWindowsHelper << window::chatWindowsList.last();
}
void mainwindow::gotScriptMessage(const usermessage u){
    QString msg=u.msg();    
    if(startswithCI(msg, "away")){
        msg.remove(0,5);
        qobjectwrapper<awayhandler>::ref().setaway(msg);
        foreach(channelWindow *w,windowList) {
            w->windowAwayTitle = " " + tr("<away>:") +" "+ qobjectwrapper<awayhandler>::ref().message();
            w->updateWindowTitle();
        }
    } else if(startswithCI(msg, "back")) {
        qobjectwrapper<awayhandler>::ref().back();
        awayMessageChanged();
    } else if(startswithCI(msg, "nick")) {
        msg.remove(0,5);
        if(ui.cbenabletus->isChecked()){
            myDebug()<<tr("Its not possible to change the nick while using secure logging. Go to the profile page at your secure logging server to change the nick.");
            return;
        }
        int pos;
        int i=ui.lenick->validator()->validate(msg,pos);
        if(i != QValidator::Acceptable){
            myDebug()<<tr("This nickname is not allowed!");
            return;
        }
        ui.lenick->setText(msg);
        reconnect();
    } else if(startswithCI(msg, "help")) {
        myDebug()<< tr("    Supported commands are:")<<
                    tr("    $away to change the away message")<<
                    tr("    $back to remove the away state")<<
                    tr("    $nick to change the nick")<<
                    tr("    $codec to open the codec dialog")<<
                    tr("    $codec to set a specific codec, (wa, prl and prc and wa2 are shortcuts for the specific codecs).")<<
                    tr("    wa=W:A 3.6.31 codepage, prc=Prosnooper cyrillic- prl= Prosnooper latin codepage, wa2=W:A 3.6.31 specific changes merged with the Localized codec.")<<

                    tr("    $quit to quit with this message")<<
                    tr("    $help to see this help");
    } else if(startswithCI(msg, "quit")) {        
        msg.remove(0,5);
        singleton<quithandler>().inducequit(msg);
    } else if(startswithCI(msg, "codec")) {
        msg.remove(0,6).simplified ();
        if(CodecSelectDia::contains (msg))
            CodecSelectDia::setcodec (msg);
        else if(msg=="prl")
            CodecSelectDia::setcodec ("windows-1252");
        else if(msg=="prc")
            CodecSelectDia::setcodec ("windows-1251");
        else {
            myDebug ()<<tr("This codec doesnt exists: ")+msg;
            CodecSelectDia().exec();
            return;
        }
        myDebug ()<<tr("Applying codec: ")+QString(CodecSelectDia::codec->name());
    } else
        myDebug()<<tr("Unknown command, write $help to get a list of available commands");
}
void mainwindow::gotNotice(usermessage u) {
    const QString &user=u.user();
    if(containsCI(S_S.ignorelist, user))
        return;
    singleton<balloonHandler>().gotPrivmsg(u);
    foreach(channelWindow *w, windowList)
        w->getUserMessage(u);
    foreach(chatwindow *w, channelWindow::chatWindowsList)
        w->getusermessage(u.add_type(e_CHANNELMSGTOCHAT));
}
void mainwindow::gotUserMessage(const usermessage u){
    const QString &user=u.user();
    if(u.has_type(e_SCRIPTCOMMAND)) {
        gotScriptMessage(u);
        return;
    }

    if(u.has_type(e_CTCP) && singleton<ctcphandler>().getctcp(u))
        return;

    if(u.has_type(e_NOTICE))
        return gotNotice(u);

    if(u.receiver() == singleton<netcoupler>().nick) {
        if(!containsCI(channelWindow::chatWindowsStringList,user))
            appendtoquerylist(user);
        qobjectwrapper<awayhandler>::ref().sendaway(user);
    }

    if (containsCI(window::chatWindowsStringList, user)) {
        foreach(chatwindow *w,channelWindow::chatWindowsList)
            w->getusermessage(u);
        if(u.receiver() == singleton<netcoupler>().nick)
            return;
    } else if (!containsCI(S_S.ignorelist, user) && u.receiver()==singleton<netcoupler>().nick
               && (windowList.isEmpty() || S_S.getbool("chballwaysopenchatwindows"))) {
        if (S_S.getbool("chbstartchatsminimized"))
            openChatWindowHidden(user);
        else
            openChatWindowRaised(user);
        window::chatWindowsList.last()->getusermessage(u);
        return;
    } else if(u.receiver() == singleton<netcoupler>().nick){
        if(containsCI(S_S.buddylist, user)) {
            if (S_S.getbool("chbstartchatsminimized"))
                openChatWindowHidden(user);
            else openChatWindowRaised(user);
            window::chatWindowsList.last()->getusermessage(u);
            singleton<balloonHandler>().gotPrivmsg(u);
#ifdef PHONON
            singleton<soundHandler>().play_buddymsgsound(user);
#endif
            return;
        }
        singleton<balloonHandler>().gotPrivmsg(u);
#ifdef PHONON
        singleton<soundHandler>().play_normalmsgsound(user);
#endif
        foreach(channelWindow *w,windowList)
            w->getUserMessage(u);
        return;
    }
    if(!u.has_type(e_GARBAGEQUIT)){
        foreach(channelWindow *w,windowList)
            if(w->currentChannel.toLower()==u.receiver().toLower())
                w->getUserMessage(u);
    }
    else
        foreach(QString channelTheQuittedUserWasBeingOn,u.receiver().split(','))
            foreach(channelWindow *w,windowList)
                if(w->currentChannel.toLower()==channelTheQuittedUserWasBeingOn.toLower())
                    w->getUserMessage(u);
}
void mainwindow::connected(){
    ui.start->setEnabled(false);
    ui.start->setText("");
    ui.connectlabel->setText(tr("Connected"));
    reopenChatWindowsAndChannelWindows();
}
void mainwindow::disconnected(){
    ui.start->setText(tr("Apply"));
    ui.start->setEnabled(true);   
    ui.connectlabel->setText(tr("Disconnected"));
    returnToLoginTab();
    emit sigDisconnected ();
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
void mainwindow::initMenus(){
    trayMenu = new QMenu;
    connect(trayMenu,SIGNAL(triggered(QAction*)),this,SLOT(trayMenuTriggered(QAction*)));
    ui.pbtraymenu->setMenu(trayMenu);
    singleton<balloonHandler>().tray->setContextMenu(trayMenu);
    QAction *a0;
    stuffMenu = trayMenu->addMenu(tr("Stuff"));
    stuffMenu->setIcon(QIcon("snppictures/stufficon.png"));
    stuffMenu->addAction(tr("About"));
    stuffMenu->addAction(tr("Settings"))->setIcon(QIcon("snppictures/settingsicon.png"));
    stuffMenu->addAction(tr("Volume"));
    stuffMenu->addAction(tr("Open Log Browser"));
    stuffMenu->addAction(usermodel::tr("Buddylist"));
    stuffMenu->addAction(tr("Scheme maker"));
    stuffMenu->addAction(tr("Select another Textcodec"));
#ifdef Q_OS_WIN32
    stuffMenu->addAction(tr("Playername"));
#endif
    styleMenu = trayMenu->addMenu(tr("Skins"));
    textSchemeMenu = trayMenu->addMenu(tr("Text Schemes"));
    {
        QDir directory(QApplication::applicationDirPath() + "/textschemes/");
        foreach(QString s,directory.entryList(QStringList("*.textscheme"),QDir::Files)) {
            textSchemeMenu->addAction(s);
        }
    }
    textSchemeMenu->addSeparator();
    textSchemeMenu->addAction(tr("Save current textscheme to file"));
    textSchemeMenu->addSeparator();
    textSchemeMenu->setIcon(QIcon("snppictures/skinicon.png"));
    styleMenu->setIcon(QIcon("snppictures/skinicon.png"));
    QDir directory(QApplication::applicationDirPath() + "/qss/");
    foreach(QString s,directory.entryList(QStringList("*.qss"),QDir::Files)) {
        styleMenu->addAction(s);
    }
    snpSetContains("qss_file");
    snpSetContains("chbautojoin");
    layoutManu = trayMenu->addMenu(tr("Layouts"));
    layoutManu->setIcon(QIcon("snppictures/arrangementicon.png"));
    layoutManu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout1.png"), "1");
    layoutManu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout2.png"), "2");
    layoutManu->addAction(QIcon(QApplication::applicationDirPath()
                                + QDir::separator() + "snppictures" + QDir::separator()
                                + "layout3.png"), "3");
    languageMenu = trayMenu->addMenu(tr("Language"));
    languageMenu->setIcon(QIcon("snppictures/languageicon.png"));
    QDir dir1(QApplication::applicationDirPath() + QDir::separator()
              + "translations" + QDir::separator());
    foreach(QString s,dir1.entryList(QStringList("*.qm"),QDir::Files)) {
        languageMenu->addAction(s);
    }
    joinMenu = trayMenu->addMenu(tr("Join"));
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesurgeChannelName);
#endif
    joinMenu->setIcon(QIcon("snppictures/joinicon.png"));
    QAction *a = new QAction(this);
    a->setText(tr("Away mode"));
    a->setIcon(QIcon("snppictures/awayicon.png"));
    trayMenu->addAction(a);
    trayMenu->addSeparator();
    a0 = trayMenu->addAction(tr("Reconnect"));
    a0->setIcon(QIcon("snppictures/reconnect.png"));
    a0 = trayMenu->addAction(QDialogButtonBox::tr("&Close"));
    a0->setIcon(QIcon("snppictures/closeicon.png"));
}

void mainwindow::trayActivation(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (!channelWindow::hiddenChannelWindowsHelper.isEmpty()) {
            channelWindow *w = channelWindow::hiddenChannelWindowsHelper.takeLast();
            w->show();
            w->raise ();
        } else if (! hiddenChatWindowsHelper.isEmpty()) {
            chatwindow *w = hiddenChatWindowsHelper.takeLast();
            w->show();
            w->raise ();
            querylist.removeAll(w->chatpartner);
        } else {
            if ( isHidden()) {
                show();
                activateWindow ();
                raise();
            } else if ( isVisible())
                close();
        }
    }
}
void mainwindow::trayMenuTriggered(QAction *a) {
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
        if(a->text()==S_S.getString("qss_file"))
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
        whichUIType = 1;
        S_S.set("whichuitype", whichUIType);
        QMessageBox::information(0,tr("Information"),tr("You must reopen the channelwindows to change the layout."));
        return;
    } else if (a->text() == "2") {
        whichUIType = 2;
        S_S.set("whichuitype", whichUIType);
        QMessageBox::information(0,tr("Information"),tr("You must reopen the channelwindows to change the layout."));
        return;
    } else if (a->text() == "3") {
        whichUIType = 3;
        S_S.set("whichuitype", whichUIType);
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
        setLanguage(a->text());
    } else if (a->text() == tr("Open Log Browser")) {
        logbrowser *l = new logbrowser;
        l->show();
        connect(l, SIGNAL(sigOpenChatWindow(const QString&)),this, SLOT(openChatWindowRaised(const QString&)));
    } else if (a->text().contains(".textscheme", Qt::CaseInsensitive)) {
        setTextScheme(a->text());
    } else if (a->text() == tr("Save current textscheme to file")) {
        QString file = QFileDialog::getSaveFileName(this, tr(
                                                        "Save textscheme to."), "textschemes/", "*.textscheme");
        if (file != "") {
            QFileInfo f(file);
            QString file_name=f.fileName();
            if(!file_name.endsWith(".textscheme"))
                file_name+=".textscheme";
            S_S.set("textscheme", file_name);
            chatHandler::initialformatsaver();
            textSchemeMenu->addAction(file_name);
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
            awayMessageChanged();
        }
    } else {
        if (!awaybox::ison) {
            awaybox *away = new awaybox;
            away->show();
            connect(away, SIGNAL(sigok()),this, SLOT(awayBoxOk()));
        }
    }
}
void mainwindow::reconnect(){
    lastOpenedWindows.clear ();
    lastOpenedChatWindows.clear ();
    foreach(channelWindow *w, windowList)
        lastOpenedWindows<<w->currentChannel;
    foreach(chatwindow *w,channelWindow::chatWindowsList)
        lastOpenedChatWindows<<w->chatpartner;
    if(singleton<netcoupler>().ircState()==QAbstractSocket::ConnectedState)
        connect(this,SIGNAL(sigDisconnected()),this,SLOT(reconnect2()));
    else reconnect2 ();
    returnToLoginTab();
    singleton<netcoupler>().stop();
}
void mainwindow::reconnect2(){
    disconnect(this,SIGNAL(sigDisconnected()),this,SLOT(reconnect2()));
    chooseClicked();
}
void mainwindow::on_pbsettings_clicked(){
    singleton<settingswindow>().show();
}
void mainwindow::on_pbabout_clicked(){
    about *ab = new about;
    ab->show();
}
void mainwindow::joinGameSurge(){
#ifdef WITH_GAMESURGE_SUPPORT
    //    ircJoinDia dia;
    //    if(!dia.exec())
    //        return;
    //    static irc_netcoupler *net=new irc_netcoupler(ui.lenick->text(), this, dia.getChannel());
    //    static irc_window *window=new irc_window(net,GamesurgeChannelName);
    //    window->show();
    //    window->raise();
#endif
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
    setLeague();
    if(ui.letuslogin->text().isEmpty()){
        QMessageBox::information(0,QObject::tr("Information"),tr("Before you can view your profile, you must fill in a loginname."));
        ui.letuslogin->setFocus(Qt::MouseFocusReason);
        return;
    }
    singleton<leagueserverhandler>().profile(ui.letuslogin->text());
}
void mainwindow::leagueServerProfilePage(QString url){
    QDesktopServices::openUrl(QUrl(url));
}
void mainwindow::on_cbleagueservers_activated(QString s){
    QStringList sl=S_S.getStringList("leagueservers"+s);
    if(sl.size()<2)
        return;
    ui.letuslogin->setText(sl.takeFirst());
    ui.letuspassword->setText(sl.takeFirst());
}

void mainwindow::on_chbautojoin_clicked(bool checked){
    if(checked)
        on_pbrememberjoin_clicked();
}
