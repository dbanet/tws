#include "mainwindow.h"
#include "channel/channeltab.h"
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
#include "ui_mainwindow.h"

#ifdef WITH_GAMESURGE_SUPPORT
#include "src/irc/irc_netcoupler.h"
#include "src/irc/irc_window.h"
#include "src/irc/ircjoindia.h"
#endif

#include <QColorDialog>
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
#include <QSignalMapper>

inihandlerclass inihandler;
extern volumeslider *volume;
bool fontOrColorHasChanged = 0;
extern QStringList querylist;
QString GamesourgeChannelName="#GameSourgeWorms";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    whichuitype = 1;
    ui->setupUi(this);
    bool b=S_S.getbool("righttoleftwriting");
    if(b)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
    ui->pbrememberjoin->setText(tr("Autojoin:"));
    ui->start->setText(tr("Apply"));
    ui->tabWidget->setTabEnabled(1, 0);

    init_menus();
    connect(singleton<balloonHandler>().tray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason)),this, SLOT(trayactivation(QSystemTrayIcon::ActivationReason)));
    singleton<pictureHandler>().fillFlags(ui->flag);
    singleton<pictureHandler>().fillRanks(ui->rank);
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

    ui->cbremember->setChecked(S_S.getbool("chbremember"));
    if (ui->cbremember->isChecked())
        chooseclicked();
    setWindowTitle(tr("The Wheat Snooper"));
    joinonstartup = 0;
    connect(ui->start, SIGNAL(clicked()),this, SLOT(chooseclicked()));
    connect(ui->pbrememberjoin, SIGNAL(clicked()),this, SLOT(pbrememberjoinclicked()));
    connect(ui->tabWidget, SIGNAL(currentChanged ( int )),this, SLOT(currenttabchanged(int)));

    connect(&singleton<netcoupler>(), SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(&singleton<netcoupler>(), SIGNAL(sigGotChanList(const QStringList &)),this, SLOT(getchannellist(const QStringList &)));
    connect(&qobjectwrapper<awayhandler>::ref(), SIGNAL(sigawaystringchanged()),this, SLOT(awaymessagechanged()));
    connect(&singleton<netcoupler>(), SIGNAL(sigconnected()),this,SLOT(connected()));
    connect(&singleton<netcoupler>(), SIGNAL(sigdisconnected()),this,SLOT(disconnected()));
    connect(&singleton<netcoupler>(), SIGNAL(siggotusermessage(const usermessage)),this,SLOT(gotusermsg(const usermessage)));

    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginfailed()),this,SLOT(leagueserverconnectionfailed()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigloginsuccess()),this,SLOT(leagueserverconnectionsuccess()));
    connect(&singleton<leagueserverhandler>(),SIGNAL(sigprofile(QString)),this,SLOT(leagueserverprofilepage(QString)));

    QRegExp regexp;
    regexp.setPattern("([A-Z]|[a-z]|[0-9]|-|`){1,15}");
    validator = new QRegExpValidator(regexp, 0);
    ui->lenick->setValidator(validator);
    ui->clan->setValidator(validator);

    //    S_S.set("isarrangedbyonetab", false);
    //    S_S.set("isarrangedbytwotabs", false);
    debugmsg.clear();

    QVariantList windowstates = S_S.getlist("MainWindowGeometry");
    if (!windowstates.isEmpty())
        restoreGeometry(windowstates.takeFirst().toByteArray());
    if(height()<530)
        resize(width(),530);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::currenttabchanged(int i){
    if(i!=0)
        return;
    foreach(channelwindow *w, windowlist){
        singleton<netcoupler>().partchannel(w->currentchannel);
        w->close();
    }
    singleton<netcoupler>().stop();
}
void MainWindow::fillsnpsettings(){
    S_S.set("chbremember", ui->cbremember->isChecked());
    S_S.set("nickname", ui->lenick->text());
    S_S.set("chbminimized", ui->chbminimized->isChecked());
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
    S_S.set("whichuitype", whichuitype);
    S_S.set("countrycode", ui->flag->currentText());
    S_S.set("rank", ui->rank->currentText());
    if (fontOrColorHasChanged == 1) {
        S_S.set("textscheme", "lastedited.textscheme");
        singleton<chatFormatSettings>().safe();
    }
}
void MainWindow::get_baseStyleSheet(){
    QFile f(QApplication::applicationDirPath() + "/qss/Skin_Base");
    if(!f.open(QFile::ReadOnly))
        QMessageBox::warning(0,QObject::tr("Warning"),QObject::tr("Cant read the Skinfile:\nSkin_Base"));
    baseStyleSheet = QLatin1String(f.readAll());
}
void MainWindow::connectToNetwork(){
    ui->tabWidget->setCurrentIndex(1);
    if(S_S.getbool("enablesecurelogging"))
        singleton<netcoupler>().start(singleton<leagueserverhandler>().nick);
    else
        singleton<netcoupler>().start(ui->lenick->text());
    ui->tabWidget->setTabEnabled(1, 1);
}
void MainWindow::chooseclicked() {
    if(ui->tabWidget->currentIndex()!=0)
        return;
    if (ui->lenick->text().isEmpty() && !ui->cbenabletus->isChecked()){
        QMessageBox::warning(this,tr("Nickname field is empty"),tr("Please choose a nickname."),QMessageBox::Ok);
        return;
    }
    S_S.transaction();
    S_S.set("clan", ui->clan->text());
    if (ui->clan->text().isEmpty())
        S_S.set("clan", "UserName");
    S_S.set("nickname", ui->lenick->text());
    S_S.set("tus_password", ui->letuspassword->text());
    S_S.set("tus_login", ui->letuslogin->text());
    S_S.set("countrycode", ui->flag->currentText());
    S_S.set("rank", ui->rank->currentText());
    S_S.set("information", ui->client->text());
    S_S.set("enablesecurelogging", ui->cbenabletus->isChecked());

    S_S.set("wormnetserverlist", refreshcombobox(ui->cbServerList));
    S_S.set("leagueservers", refreshcombobox(ui->cbleagueservers));
    S_S.set("showinformation", ui->cbshowinformation->isChecked());
    S_S.set("leagueservers"+makeValidColumnName(ui->cbleagueservers->currentText()), QStringList()<<ui->letuslogin->text()<<ui->letuspassword->text());

    if(S_S.getbool("enablesecurelogging")){
        S_S.set("spectateleagueserver", true);
        setleague();
        singleton<leagueserverhandler>().login(ui->letuslogin->text(),ui->letuspassword->text());
    } else {
        S_S.set("spectateleagueserver", false);
        connectToNetwork ();
    }
    ui->start->setEnabled(false);
    ui->start->setText("");
    S_S.commit();
}
void MainWindow::setleague(){
    QString item=ui->cbleagueservers->currentText();
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

void MainWindow::leagueserverconnectionfailed(){
    ui->start->setText(tr("Apply"));
    ui->start->setEnabled(true);
}
void MainWindow::leagueserverconnectionsuccess(){
    connectToNetwork();
}
void MainWindow::reopenChatWindowsAndChannelWindows(){
    foreach(QString s,lastOpenedChatWindows)
        openchatwindowraised(s);
    lastOpenedChatWindows.clear();
    foreach(QString s,lastOpenedWindows)
        join(s);
    lastOpenedWindows.clear();
}
void MainWindow::getchannellist(const QStringList &sl) {
    ui->cbchannels->clear();
    foreach(QString s,sl)
        ui->cbchannels->addItem(s);
    channellist = sl;
    ui->pbjoin->setEnabled(1);
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
channelTab* MainWindow::dockTab(channelTab *tab){
    if(0!=(tab=tab==0?(channelTab*)QObject::sender():tab)){
        ui->tabWidget->addTab(tab,tab->currentchannel);
        ui->tabWidget->setCurrentWidget(tab);

        QAction *undock=new QAction("Undock",tab);
        tab->addAction(undock);
        tab->setContextMenuPolicy(Qt::ActionsContextMenu);
        connect(undock,SIGNAL(triggered()),this,SLOT(actionUndockTabTriggered()));

        QMenu *menu=tab->createMenu();
        ui->menuChannel->addActions(menu->actions());
        ui->menuChannel->setEnabled(true);
    }
    return tab;
}
void MainWindow::actionUndockTabTriggered(){
    this->undockTab((channelTab*)(((QAction*)QObject::sender())->parent()));
}
channelTab* MainWindow::undockTab(channelTab *tab){
    if(0!=(tab=tab==0?(channelTab*)QObject::sender():tab)){
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tab));

        QMenu *tabMenu=new QMenu();
        tabMenu->addActions(ui->menuChannel->actions());
        tab->monopolizeMenu(tabMenu);

        tab->setParent(0);
        tab->show();
    }
    return tab;
}
void MainWindow::join(const QString channel){
//    if(channel==GamesourgeChannelName){
//        joinGameSourge();
//        return;
//    }
    if(currentchannellist.contains(channel))
        return;
    currentchannellist << channel;

    channelTab *chanTab=dockTab(new channelTab(channel,1));
    windowlist.push_back(chanTab);

    singleton<netcoupler>().joinchannel(channel);
    if(windowlist.isEmpty())
        return;
    if (qobjectwrapper<awayhandler>::ref().away()) {
        windowlist.last()->windowtitleaway = qobjectwrapper<awayhandler>::ref().message();
        windowlist.last()->mysetwindowtitle();
    }
    connect(windowlist.last(), SIGNAL(sigjoinchannel(const QString&)),this, SLOT(join(const QString&)));
    connect(windowlist.last(), SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindowraised(const QString &)));
    connect(this, SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindowraised(const QString &)));
    connect(windowlist.last(), SIGNAL(sigclosed()),this, SLOT(windowclosed()));
}
void MainWindow::windowclosed(){
    channelwindow *w=qobject_cast<channelwindow*> (sender());
    Q_CHECK_PTR(w);
    w->hiddenchannelwindowshelper.removeAll(w);
    currentchannellist.removeAll(w->currentchannel);
    windowlist.removeAll(w);
}
void MainWindow::quit(){
    foreach(chatwindow *w,channelwindow::chatwindows) {
        Q_ASSERT(w!=0);
        w->close();
    }
    foreach(channelwindow *w, windowlist) {
        Q_ASSERT(w!=0);
        w->close();
    }
    fillsnpsettings();
}
void MainWindow::closeEvent(QCloseEvent * e) {
    QVariantList windowstates;
    windowstates << saveGeometry();
    S_S.set("MainWindowGeometry", windowstates);
    e->ignore ();
    hide();
}
void MainWindow::returntologintab() {
    joinonstartup = 0;
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
    joinmenu->clear();
    currentchannellist.clear();
    ui->cbchannels->clear();
    ui->pbjoin->setEnabled(0);
    ui->tabWidget->setTabEnabled(1, 0);
    ui->pbjoin->setEnabled(0);
    foreach(chatwindow *w,channelwindow::chatwindows) {
        Q_ASSERT(w);
        w->close();
    }
    channelwindow::chatwindows.clear();
    foreach(channelwindow *w,windowlist)
        w->close();
    ui->tabWidget->setTabEnabled(1, 0);
}
void MainWindow::pbrememberjoinclicked() {
    S_S.set("joinonstartup", ui->cbchannels->currentText());
    ui->pbrememberjoin->setText(tr("Autojoin:") + "\n" + S_S.getString("joinonstartup"));
}
void MainWindow::snpsetcontains(const QString &s) {
    if (s == "chbautojoin" && S_S.contains(s))
        ui->chbautojoin->setChecked(S_S.getbool("chbautojoin"));
    else if (s == "qss_file") {
        QFile f(QApplication::applicationDirPath() + "/qss/" + S_S.getString("qss_file"));
        if(!f.open(QFile::ReadOnly))
            QMessageBox::warning(this,QObject::tr("Warning"),tr("Cant read the Skinfile:\n")+S_S.getString("qss_file"));
        QString stylesheet = QLatin1String(f.readAll());
    } else if (s == "joinonstartup" && S_S.contains(s) && S_S.contains("chbautojoin")) {
        ui->pbrememberjoin->setText(ui->pbrememberjoin->text().split("\n").first() + "\n" + S_S.getString("joinonstartup"));
        if (S_S.getbool("chbautojoin"))
            join(S_S.getString("joinonstartup"));
    } else if (s == "chbminimized")
        ui->chbminimized->setChecked(S_S.getbool("chbminimized"));
    else if (s == "nickname" && S_S.contains(s))
        ui->lenick->setText(S_S.getString("nickname"));
    else if (s == "tus_password" && S_S.contains("tus_password"))
        ui->letuspassword->setText(S_S.getString("tus_password"));
    else if (s == "tus_login" && S_S.contains("tus_login"))
        ui->letuslogin->setText(S_S.getString("tus_login"));
    else if (s == "countrycode"){
        QString language=QLocale::system().name().left(2);
        int i=ui->flag->findText(S_S.getString("countrycode"));
        if(i==-1)
            i=ui->flag->findText(language);
        ui->flag->setCurrentIndex(i);
    }else if (s == "rank"){
        ui->rank->setCurrentIndex(S_S.getint("rank"));
    }
    else if (s == "information" && S_S.contains(s))
        ui->client->setText(S_S.getString(s));
    else if (s == "clan" && S_S.contains(s)){
        QString clanstring=S_S.getString(s);
        if(clanstring.toLower()=="username")
            ui->clan->setText("");
        else
            ui->clan->setText(clanstring);
    }
    else if (s == "whichuitype" && S_S.contains(s))
        whichuitype = S_S.getint("whichuitype");
    else if (s == "wormnetserverlist")
        ui->cbServerList->addItems(S_S.getStringList("wormnetserverlist"));
    else if (s == "leagueservers"){
        ui->cbleagueservers->addItems(S_S.getStringList("leagueservers"));
    }
    else if(s=="enablesecurelogging"){
        bool b=S_S.getbool("enablesecurelogging");
        ui->cbenabletus->setChecked(b);
        on_cbenabletus_toggled(b);
    } else if(s=="showinformation"){
        bool b=S_S.getbool("showinformation");
        ui->cbshowinformation->setChecked(b);
    }
}
void MainWindow::usesettingswindow(const QString&) {
}
void MainWindow::chatwinowclosed() {
    chatwindow *w = qobject_cast<chatwindow*> (sender());
    Q_CHECK_PTR(w);
    w->disconnect();
    hiddenchatwindowshelper.removeAll(w);
    channelTab::chatwindowstringlist.removeAll(w->chatpartner);
    channelTab::chatwindows.removeAll(w);
}
void MainWindow::appenddebugmessage(const QString &msg) {
    debugmsg.append(msg);
    ui->labeltraydescription->insertPlainText(debugmsg);
    if(!S_S.cbservermessageinchannelwindows)
        return;
    foreach( channelwindow *w,windowlist)
        w->gotdebugmsg(debugmsg);
    debugmsg.clear();
}
void MainWindow::setlanguage(const QString &langfile) {
    S_S.set("language_file", langfile);
    myDebug() << S_S.getString("language_file");
    QMessageBox::StandardButton button = QMessageBox::question(this, tr(
                                                                   "Restart the application?"), tr(
                                                                   "Changing the translation requires a program restart.\n"
                                                                   "To restart now click Ok, otherwise cancel."), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
    if (button == QMessageBox::Ok) {
        returntologintab();
        singleton<netcoupler>().stop();
        QProcess::startDetached(qApp->applicationFilePath(), QStringList(), QApplication::applicationDirPath());
        foreach(channelwindow *w,windowlist)
            w->close();
        singleton<balloonHandler>().tray->hide();
        singleton<quithandler>().inducequit();
    }
}
void MainWindow::awayboxok() {
    foreach(channelwindow *w,windowlist) {
        w->windowtitleaway = " " + tr("<away>:") +" "+ qobjectwrapper<awayhandler>::ref().message();
        w->mysetwindowtitle();
    }
}
void MainWindow::awaymessagechanged() {
    if (qobjectwrapper<awayhandler>::ref().away()) {
        foreach(channelwindow *w,windowlist) {
            w->windowtitleaway = " " + tr("<away>:") + " "+qobjectwrapper<awayhandler>::ref().message();
            w->mysetwindowtitle();
        }
    } else {
        foreach(channelwindow *w,windowlist) {
            w->windowtitleaway = "";
            w->mysetwindowtitle();
        }
    }
}
void MainWindow::settextscheme(const QString &file) {
    myDebug() << tr("trying to apply new textscheme: ") + file;
    S_S.set("textscheme", file);
    singleton<chatFormatSettings>().load();
    chatHandler::initialFormatStarter();
}
void MainWindow::openchatwindow(QString user){
    channelTab::chatwindowstringlist << user;
    channelTab::chatwindows.push_back(new chatwindow(user));
    connect(channelTab::chatwindows.last(), SIGNAL(sigclosed()),this, SLOT(chatwinowclosed()));
}
void MainWindow::openchatwindowraised(const QString &user) {
    openchatwindow(user);
    channelTab::chatwindows.last()->show();
    channelTab::chatwindows.last()->raise();
}
void MainWindow::openchatwindowhidden(const QString &user) {
    openchatwindow(user);
    hiddenchatwindowshelper << channelTab::chatwindows.last();
}
void MainWindow::gotscriptmsg(const usermessage u){
    QString msg=u.msg();
    if(startswithCI(msg, "away")){
        msg.remove(0,5);
        qobjectwrapper<awayhandler>::ref().setaway(msg);
        foreach(channelwindow *w,windowlist) {
            w->windowtitleaway = " " + tr("<away>:") +" "+ qobjectwrapper<awayhandler>::ref().message();
            w->mysetwindowtitle();
        }
    } else if(startswithCI(msg, "back")) {
        qobjectwrapper<awayhandler>::ref().back();
        awaymessagechanged();
    } else if(startswithCI(msg, "nick")) {
        msg.remove(0,5);
        if(ui->cbenabletus->isChecked()){
            myDebug()<<tr("Its not possible to change the nick while using secure logging. Go to the profile page at your secure logging server to change the nick.");
            return;
        }
        int pos;
        int i=ui->lenick->validator()->validate(msg,pos);
        if(i != QValidator::Acceptable){
            myDebug()<<tr("This nickname is not allowed!");
            return;
        }
        ui->lenick->setText(msg);
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
void MainWindow::gotnotice(usermessage u) {
    const QString &user=u.user();
    if(containsCI(S_S.ignorelist, user))
        return;
    singleton<balloonHandler>().gotPrivmsg(u);
    foreach(channelwindow *w, windowlist)
        w->getusermessage(u);
    foreach(chatwindow *w, channelwindow::chatwindows)
        w->getusermessage(u.add_type(e_CHANNELMSGTOCHAT));
}
void MainWindow::gotusermsg(const usermessage u){
    const QString &user=u.user();
    if(u.has_type(e_SCRIPTCOMMAND)) {
        gotscriptmsg(u);
        return;
    }

    if(u.has_type(e_CTCP) && singleton<ctcphandler>().getctcp(u))
        return;

    if(u.has_type(e_NOTICE))
        return gotnotice(u);

    if(u.receiver() == singleton<netcoupler>().nick) {
        if(!containsCI(channelwindow::chatwindowstringlist,user))
            appendtoquerylist(user);
        qobjectwrapper<awayhandler>::ref().sendaway(user);
    }

    if (containsCI(channelTab::chatwindowstringlist, user)) {
        foreach(chatwindow *w,channelwindow::chatwindows)
            w->getusermessage(u);
        if(u.receiver() == singleton<netcoupler>().nick)
            return;
    } else if (!containsCI(S_S.ignorelist, user) && u.receiver()==singleton<netcoupler>().nick
               && (windowlist.isEmpty() || S_S.getbool("chballwaysopenchatwindows"))) {
        if (S_S.getbool("chbstartchatsminimized"))
            openchatwindowhidden(user);
        else
            openchatwindowraised(user);
        channelTab::chatwindows.last()->getusermessage(u);
        return;
    } else if(u.receiver() == singleton<netcoupler>().nick){
        if(containsCI(S_S.buddylist, user)) {
            if (S_S.getbool("chbstartchatsminimized"))
                openchatwindowhidden(user);
            else openchatwindowraised(user);
            channelTab::chatwindows.last()->getusermessage(u);
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
        foreach(channelwindow *w,windowlist)
            w->getusermessage(u);
        return;
    }
    if(!u.has_type(e_GARBAGEQUIT)){
        foreach(channelwindow *w,windowlist)
            if(w->currentchannel.toLower()==u.receiver().toLower())
                w->getusermessage(u);
    }
    else
        foreach(QString channelTheQuittedUserWasBeingOn,u.receiver().split(','))
            foreach(channelwindow *w,windowlist)
                if(w->currentchannel.toLower()==channelTheQuittedUserWasBeingOn.toLower())
                    w->getusermessage(u);
}
void MainWindow::connected(){
    ui->start->setEnabled(false);
    ui->start->setText("");
    reopenChatWindowsAndChannelWindows();
}
void MainWindow::disconnected(){
    ui->start->setText(tr("Apply"));
    ui->start->setEnabled(true);
    returntologintab();
    emit sigdisconnected ();
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
void MainWindow::init_menus(){
    traymenu = new QMenu;
    connect(traymenu,SIGNAL(triggered(QAction*)),this,SLOT(traymenutriggered(QAction*)));
    ui->pbtraymenu->setMenu(traymenu);
    singleton<balloonHandler>().tray->setContextMenu(traymenu);
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
#ifdef Q_OS_WIN32
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

void MainWindow::trayactivation(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (!channelwindow::hiddenchannelwindowshelper.isEmpty()) {
            channelwindow *w = channelwindow::hiddenchannelwindowshelper.takeLast();
            w->show();
            w->raise ();
        } else if (! hiddenchatwindowshelper.isEmpty()) {
            chatwindow *w = hiddenchatwindowshelper.takeLast();
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
void MainWindow::traymenutriggered(QAction *a) {
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
            /* removed skin support temprarily *//*try{
                QMessageBox::StandardButton button=QMessageBox::warning(this,QObject::tr("Warning"),
                                                                        tr("Changing the skin crashes sometimes, but The Wheat Snooper\n" \
                                                                           "will keep the settings for the next Start.\nDo you want to proceed?"),
                                                                        QMessageBox::Ok | QMessageBox::Cancel);
            }
            catch(...){
                myDebug()<<tr("Skinchanging failed, please try again.");
            }*/
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
        About *ab = new About;
        ab->show();
        return;
    } else if (a->text() == tr("Settings")) {
        singleton<settingswindow>().show();
        return;
    } else if (a->text() == tr("Reconnect")) {
        reconnect();
        return;
    } else if (a->text() == usermodel::tr("Buddylist")) {
        BuddyList *buddy = new BuddyList;
        buddy->show();
    }
    if (a->text() == tr("Volume")) {
        volume->show();
        volume->move(QCursor::pos());
    } else if (a->text().contains(".qm", Qt::CaseInsensitive)) {
        setlanguage(a->text());
    } else if (a->text() == tr("Open Log Browser")) {
        LogBrowser *l = new LogBrowser;
        l->show();
        connect(l, SIGNAL(sigopenchatwindow(const QString&)),this, SLOT(openchatwindowraised(const QString&)));
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
            S_S.set("textscheme", file_name);
            chatHandler::initialformatsaver();
            textschememenu->addAction(file_name);
        }
    } else if (a->text() == tr("Scheme maker")) {
        TextSchemeHandler *h = new TextSchemeHandler;
        h->show();
    } else if (a->text() == tr("Playername")) {
        PlayerName *pl = new PlayerName;
        pl->show();
    }
}
void MainWindow::handleAwayBox(){
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
void MainWindow::reconnect(){
    lastOpenedWindows.clear ();
    lastOpenedChatWindows.clear ();
    foreach(channelwindow *w, windowlist)
        lastOpenedWindows<<w->currentchannel;
    foreach(chatwindow *w,channelwindow::chatwindows)
        lastOpenedChatWindows<<w->chatpartner;
    if(singleton<netcoupler>().ircstate()==QAbstractSocket::ConnectedState)
        connect(this,SIGNAL(sigdisconnected()),this,SLOT(reconnect2()));
    else reconnect2 ();
    returntologintab();
    singleton<netcoupler>().stop();
}
void MainWindow::reconnect2(){
    disconnect(this,SIGNAL(sigdisconnected()),this,SLOT(reconnect2()));
    chooseclicked();
}
void MainWindow::on_pbsettings_clicked(){
    singleton<settingswindow>().show();
}
void MainWindow::on_pbabout_clicked(){
    About *ab = new About;
    ab->show();
}
void MainWindow::joinGameSourge(){
    //    ircJoinDia dia;
    //    if(!dia.exec())
    //        return;
    //    static irc_netcoupler *net=new irc_netcoupler(ui->lenick->text(), this, dia.getChannel());
    //    static irc_window *window=new irc_window(net,GamesourgeChannelName);
    //    window->show();
    //    window->raise();
}

void MainWindow::on_pbjoin_clicked(){
    join(ui->cbchannels->currentText());
}

void MainWindow::on_cbenabletus_toggled(bool checked){
    if(checked) {
        ui->lenick->setEnabled(false);
        ui->rank->setEnabled(false);
        ui->clan->setEnabled(false);
        int i=ui->cbServerList->findText("http://wormnet1.team17.com");
        if(i==-1)
            ui->cbServerList->addItem("http://wormnet1.team17.com");
        i=ui->cbServerList->findText("http://wormnet1.team17.com");
        ui->cbServerList->setCurrentIndex(i);
        ui->cbServerList->setEnabled(false);
        ui->flag->setEnabled(false);
        ui->letuspassword->setEnabled(true);
        ui->letuslogin->setEnabled(true);
        ui->cbleagueservers->setEnabled(true);
    } else {
        ui->flag->setEnabled(true);
        ui->lenick->setEnabled(true);
        ui->rank->setEnabled(true);
        ui->clan->setEnabled(true);
        ui->cbServerList->setEnabled(true);;
        ui->letuspassword->setEnabled(false);
        ui->letuslogin->setEnabled(false);
        ui->cbleagueservers->setEnabled(false);
    }
}

void MainWindow::on_pbeditleagueprofile_clicked(){
    setleague();
    if(ui->letuslogin->text().isEmpty()){
        QMessageBox::information(0,QObject::tr("Information"),tr("Before you can view your profile, you must fill in a loginname."));
        ui->letuslogin->setFocus(Qt::MouseFocusReason);
        return;
    }
    singleton<leagueserverhandler>().profile(ui->letuslogin->text());
}
void MainWindow::leagueserverprofilepage(QString url){
    QDesktopServices::openUrl(QUrl(url));
}
void MainWindow::on_cbleagueservers_activated(QString s){
    QStringList sl=S_S.getStringList("leagueservers"+s);
    if(sl.size()<2)
        return;
    ui->letuslogin->setText(sl.takeFirst());
    ui->letuspassword->setText(sl.takeFirst());
}
void MainWindow::on_chbautojoin_clicked(bool checked){
    if(checked)
        pbrememberjoinclicked();
}
void MainWindow::on_actionReconnect_triggered(){
    this->reconnect();
}
void MainWindow::on_actionClose_triggered(){
    singleton<quithandler>().inducequit();
}
void MainWindow::on_actionLock_the_UI_triggered(bool checked){
    for(int i=0;i<windowlist.length();++i)
        if(checked)
            windowlist[i]->lockUI();
        else
            windowlist[i]->unlockUI();
}
void MainWindow::on_actionSet_away_triggered(){
    handleAwayBox();
}
void MainWindow::on_actionBuddy_list_triggered(){
    BuddyList *buddyList=new BuddyList();
    buddyList->show();
}
void MainWindow::on_actionLog_browser_triggered(){
    LogBrowser *logBrowser=new LogBrowser();
    logBrowser->show();
}
void MainWindow::on_actionChange_ingame_nick_triggered(){
    PlayerName *playerName=new PlayerName();
    playerName->show();
}
void MainWindow::on_actionSelect_encoding_triggered(){
    CodecSelectDia().exec();
}
void MainWindow::on_actionVolume_triggered(){
    volume->show();
    volume->move(QCursor::pos());
}
void MainWindow::on_actionText_scheme_maker_triggered(){
    TextSchemeHandler *textSchemeHandler=new TextSchemeHandler();
    textSchemeHandler->show();
}
void MainWindow::on_actionSettings_triggered(){
    singleton<settingswindow>().show();
}
void MainWindow::on_actionAbout_triggered(){
    About *about=new About();
    about->show();
}
void MainWindow::on_actionDisconnect_triggered(){
    returntologintab();
}
void MainWindow::on_tabWidget_currentChanged(int index){
    QWidget *currentTab=ui->tabWidget->currentWidget();
    if(index<0)
        qFatal("No tabs in the tabBar left.");
    else if(currentTab->objectName()=="loginTab"){
        /* disabling the "channels" and "server" menus (neither is active) */
        ui->menuChannel->setEnabled(false);
        ui->menuServer->setEnabled(false);
    }
    else if(ui->tabWidget->currentWidget()->objectName()=="joinChannelsTab"){
        /* disabling the "channels" menu (no channel tab is active) */
        ui->menuChannel->setEnabled(false);

        /* enabling the "server" menu (if we're at this tab, then we've successfully connected */
        ui->menuServer->setEnabled(true);
    }
    else if(ui->tabWidget->currentWidget()->objectName()=="channelwindow"){
        /* enabling the "channels" menu */
        ui->menuChannel->setEnabled(true);
    }
}
void MainWindow::on_actionStalk_words_triggered(bool checked){
    singleton<settingswindow>().set("cbcostumword",checked);
}
void MainWindow::on_actionSound_triggered(bool checked){
    singleton<settingswindow>().set("cbdontplaysound",!checked);
}
void MainWindow::on_actionTray_notifications_triggered(bool checked){
    singleton<settingswindow>().set("cbshowballoons",checked);
}
void MainWindow::on_actionSort_userlist_triggered(bool checked){
    singleton<sqlsettings>().set("cbdontsortinchannels",!checked);
}
void MainWindow::on_menuSpectate_triggered(QAction *serverMenuItem){
    if(serverMenuItem==NULL)
        return;

    if(serverMenuItem->objectName()=="actionSpectateOff"){ /* turning spectation off */
        singleton<leagueserverhandler>().stoprefresh();
        S_S.set("spectateleagueserver",false);
    }
    else{
        QString server=serverMenuItem->text();
        S_S.set("spectateleagueserver",true);
        singleton<leagueserverhandler>().setleague(server,server);
        singleton<leagueserverhandler>().startrefresh();
    }
}
void MainWindow::on_menuLeagueColor_triggered(QAction *colorMenuItem){
    if(colorMenuItem==NULL)
        return;

    if(colorMenuItem->objectName()=="actionLeagueColorOff"){
        if(!S_S.getbool("leaguestatecoloron"))
            return;
        S_S.set("leaguestatecoloron",false);
        reconnect();
    }
    else{
        QColor *color;
        QString colorName=colorMenuItem->objectName();

        if(colorName=="actionLeagueColorReady")
            color=new QColor(Qt::green); else

        if(colorName=="actionLeagueColorAFK")
            color=new QColor(Qt::yellow); else

        if(colorName=="actionLeagueColorDND")
            color=new QColor(Qt::red); else

        if(colorName=="actionLeagueColorCustom"){
            QColorDialog dia;
            dia.setOption(QColorDialog::ShowAlphaChannel);
            color=new QColor(dia.getColor());
            if(!color->isValid()) return;
        }
        else qFatal((const char*)(QString("void MainWindow::on_menuLeagueColor_triggered(QAction *colorMenuItem) has got strange item with the following objectName(): ")+colorName).toLatin1().data());

        if(color->name()==S_S.getString("leaguestatecolorname") && S_S.getbool("leaguestatecoloron"))
            return;

        S_S.set("leaguestatecolorname",color->name());
        S_S.set("leaguestatecoloron",true);
        reconnect();
    }
}
