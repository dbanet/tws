#include "mainwindow.h"
#include"window.h"
#include"netcoupler.h"
#include<QDir>
#include<QStringList>
#include<QSystemTrayIcon>
#include<QMenu>
#include<QDebug>
#include<QMessageBox>
#include<QPicture>
#include"buddylist.h"
#include<QTime>
#include"ctcphandler.h"
#include"chathandler.h"
#include<QFileDialog>
#include"buttonlayout.h"
#include"awaybox.h"
#include"inihandlerclass.h"
#include"chatwindow.h"
#include"about.h"
#include"snpsettings.h"
#include"charformatsettings.h"
#include "ui_window.h"
#include "ui_window2.h"
#include "ui_window3.h"
#include"settingswindow.h"
#include"volumeslider.h"
#include "logbrowser.h"
#include "textschemehandler.h"
#include"playername.h"
#include "singleton.h"
#include "sound_handler.h"
#include"global_functions.h"
#include "balloon_handler.h"
inihandlerclass inihandler;
QPointer<netcoupler> net;
extern volumeslider *volume;
extern QList<QPixmap*> flaglist;
extern QList<QPixmap*> ranklist;
QString mainwindow::debugmsg;
QMap<QString, QString> mainwindow::rememberwhogotaway;
bool fontorcolorchanged = 0;
extern QStringList querylist;
QList< ::window *> mainwindow::windowlist;
QList< ::chatwindow*> mainwindow::hiddenchatwindowshelper;
mainwindow::mainwindow(QWidget *parent) :
        QWidget(parent), editor(new initextbrowser) {
    net = NULL;
    whichuitype = 1;
    ui.setupUi(this);    
    ui.pbabout->setText(tr("About"));
    ui.tabWidget->setTabEnabled(1, 0);

    init_menus();

    connect(singleton<balloon_handler>().tray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason)),this, SLOT(trayactivation(QSystemTrayIcon::ActivationReason)));
    ui.label->setPixmap(QApplication::applicationDirPath() + QDir::separator()
                        + "snppictures" + QDir::separator() + "mainwindowlabel.png");
    QDir dir(QApplication::applicationDirPath() + QDir::separator() + "snpini");
    QStringList sl = dir.entryList(QStringList() << "*.net", QDir::Files);
    ui.cbini->addItems(sl);
    if (ui.cbini->findText(singleton<snpsettings>().map["netinifile"].toString()) != -1)
        ui.cbini->setCurrentIndex(ui.cbini->findText(
                singleton<snpsettings>().map["netinifile"].toString()));
    for (int i = 0; i < flaglist.size(); ++i)
        ui.flag->addItem(*(flaglist[i]), QString::number(i));
    for (int i = 0; i < ranklist.size(); ++i)
        ui.rank->addItem(*(ranklist[i]), QString::number(i));
    snpsetcontains("chbminimized");
    snpsetcontains("inifilename");
    snpsetcontains("nickname");
    snpsetcontains("password");
    snpsetcontains("whichuitype");
    snpsetcontains("flag");
    snpsetcontains("rank");
    snpsetcontains("client");
    snpsetcontains("clan");

    ui.cbremember->setChecked(singleton<snpsettings>().map["chbremember"].value<bool> ());
    if (ui.cbremember->isChecked() && ui.lenick->text() != "") {
        chooseclicked();
        ui.tabWidget->setCurrentIndex(1);
    }
    this->setWindowTitle(tr("Wheat Snoopers root window."));
    joinonstartup = 0;
    connect(ui.tabWidget, SIGNAL(currentChanged (int) ),this, SLOT(returntotabsettings(int)));
    connect(ui.start, SIGNAL(clicked()),this, SLOT(chooseclicked()));
    connect(ui.pbeditor, SIGNAL(clicked()),this, SLOT(pbeditorclicked()));
    connect(ui.pbrememberjoin, SIGNAL(clicked()),this, SLOT(pbrememberjoinclicked()));
    connect(QApplication::instance(), SIGNAL(aboutToQuit()),this, SLOT(onquit()));
    connect(ui.tabWidget, SIGNAL(currentChanged ( int )),this, SLOT(returntotabsettings(int)));

    connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));

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

    QFont f;
    f.setWeight(QFont::Bold);
    ui.labeltraydescription1->setFont(f);
    ui.labeltraydescription1->insertPlainText(
            tr(
                    "<To hide or show this window,"
                    "you can leftclick the tray icon. "
                    "A right click to the tray opens a set of menues. "
                    "Use this menues to join channels, to change the skin "
                    "or to change the layout of the Wormnet channels. "
                    "Changing the layout will not be applied to existing channelwindows. "
                    "Under stuff you can find some handy settings for sound, alerts and others.>\n\n"));
    f.setWeight(QFont::Normal);
    ui.labeltraydescription->setFont(f);
    ui.labeltraydescription->insertPlainText(QString("\n\n") + debugmsg);
    debugmsg.clear();

    connect(&singleton<ctcphandler>(), SIGNAL(sigctcpcommand(const QString&,const QString&)),this, SLOT(gotctcpsignal(const QString&,const QString&)));
}
void mainwindow::chooseclicked() {
    if (ui.lenick->text().isEmpty())
        ui.lenick->setText("tempnick");
    if (ui.clan->text().isEmpty())
        ui.clan->setText("Username");
    singleton<snpsettings>().map["nickname"] = ui.lenick->text();
    singleton<snpsettings>().map["password"] = ui.lepassword->text();
    singleton<snpsettings>().map["inifilename"] = ui.cbini->currentText();
    singleton<snpsettings>().map["flag"] = ui.flag->currentText();
    singleton<snpsettings>().map["rank"] = ui.rank->currentText();
    singleton<snpsettings>().map["client"] = ui.client->text();
    singleton<snpsettings>().map["clan"] = ui.clan->text();
    singleton<snpsettings>().safe();
    if (editor->isHidden()) {
        ui.tabWidget->setCurrentIndex(1);
        inihandler = inihandlerclass(ui.cbini->currentText());
        net = new netcoupler(ui.lenick->text(), this);    
        connect(net, SIGNAL(siggotchanellist(const QStringList &)),this, SLOT(getchannellist(const QStringList &)));
        connect(net, SIGNAL(sigawaystringchanged()),this, SLOT(awaymessagechanged()));
        connect(net, SIGNAL(siggotprivmsg(const QString&,const QString&,const QString&)),this,
                SLOT(gotprvmsg(const QString&,const QString&,const QString&)));
        connect(ui.pbjoin, SIGNAL(clicked()),this, SLOT(joinclicked()));
        connect(net,SIGNAL(sigconnected()),this,SLOT(connected()));
        connect(net,SIGNAL(sigdisconnected()),this,SLOT(disconnected()));
    } else
        QMessageBox::warning(
                this,
                tr("warning"),
                tr("you are still editing the ini file. please close it bevor joining wormnet."),
                QMessageBox::Ok);
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
    if (joinonstartup == 0) {
        snpsetcontains("joinonstartup");
        joinonstartup = 1;
    }
}
void mainwindow::joinclicked() {
    QString s = ui.cbchannels->currentText();
    if (ui.pbjoin->text() != "" && !currentchannellist.contains(s)) {
        net->joinchannel(s);
        currentchannellist << s;
        if (whichuitype == 1) {
            windowlist.push_back(new ::window(net, s, whichuitype));
            windowlist.last()->setObjectName("channelwindow");
        } else if (whichuitype == 2) {
            windowlist.push_back(new ::window(net, s, whichuitype));
            windowlist.last()->setObjectName("channelwindow");
        } else if (whichuitype == 3) {
            windowlist.push_back(new ::window(net, s, whichuitype));
            windowlist.last()->setObjectName("channelwindow");
        } else
            qDebug() << "joinclicked in mainwindow assert";
        /*if(singleton<snpsettings>().map["isarrangedbyonetab"].value<bool>() == 1){
                 this->tabbox.addwidget(windowlist.last());
                 tabbox.show();
                 }else if(singleton<snpsettings>().map["isarrangedbytwotabs"].value<bool>() == 1){
                 this->tabwindowbox.addwidget(windowlist.last());
                 tabwindowbox.show();*/
        windowlist.last()->show();
        if (net->isaway) {
            windowlist.last()->windowtitleaway = net->awaymessage;
            windowlist.last()->mysetwindowtitle();
        }
        connect(windowlist.last(), SIGNAL(sigjoinchannel(const QString&)),this, SLOT(joinslot(const QString&)));
        connect(windowlist.last(), SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
        connect(this, SIGNAL(sigopenchatwindow(const QString &)),this, SLOT(openchatwindow(const QString &)));
        connect(windowlist.last(), SIGNAL(sigwindowclosed(const QString&)),this, SLOT(windowremoved(const QString&)));
        singleton<snpsettings>().safe();
    }
}
void mainwindow::joinslot(const QString &s) {
    int i = ui.cbchannels->findText(s);
    if (i != -1) {
        ui.cbchannels->setCurrentIndex(i);
        this->joinclicked();
    }
}
void mainwindow::onquit() {
    singleton<snpsettings>().map["inifilename"] = ui.cbini->currentText();
    singleton<snpsettings>().map["chbremember"] = ui.cbremember->isChecked();
    singleton<snpsettings>().map["nickname"] = ui.lenick->text();
    singleton<snpsettings>().map["chbminimized"] = ui.chbminimized->isChecked();
    singleton<snpsettings>().map["chbautojoin"] = ui.chbautojoin->isChecked();
    singleton<snpsettings>().map["whichuitype"] = whichuitype;
    singleton<snpsettings>().map["netinifile"] = ui.cbini->currentText();
    if (fontorcolorchanged == 1) {
        singleton<snpsettings>().map["charformatfile"] = "lastedited.textscheme";
        singleton<charformatsettings>().safe();
    }
    singleton<snpsettings>().safeonquit();
    singleton<snpsettings>().safe();
    bool b = singleton<settingswindow>().from_map("cbsafequerys").value<bool> (); //no need for usesettingswindow here
    if (b) {
        safeusergarbage();
        safequerylist();
    }
}
void mainwindow::closeEvent(QCloseEvent *) {
}
void mainwindow::returntotabsettings(int i) {
    if (i == 0) {
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
        QApplication::processEvents();
        windowlist.clear();
        net->deleteLater();
    }
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
    ui.pbrememberjoin->setText(ui.pbrememberjoin->text().split("\n").first()
                               + "\n" + singleton<snpsettings>().map["joinonstartup"].value<QString> ());
    singleton<snpsettings>().safe();
}
void mainwindow::snpsetcontains(const QString &s) {
    if (s == "chbautojoin" && singleton<snpsettings>().map.contains(s))
        ui.chbautojoin->setChecked(singleton<snpsettings>().map["chbautojoin"].value<bool> ());

    else if (s == "qss file" && singleton<snpsettings>().map.contains(s)) {
        QFile f(QApplication::applicationDirPath() + QDir::separator() + "qss"
                + QDir::separator() + singleton<snpsettings>().map["qss file"].value<QString> ());
        f.open(QFile::ReadOnly);
        QString stylesheet = QLatin1String(f.readAll());
        qApp->setStyleSheet(stylesheet);
    } else if (s == "joinonstartup" && singleton<snpsettings>().map.contains(s)
        && singleton<snpsettings>().map.contains("chbautojoin")) {
        ui.pbrememberjoin->setText(
                ui.pbrememberjoin->text().split("\n").first() + "\n"
                + singleton<snpsettings>().map["joinonstartup"].value<QString> ());
        if (singleton<snpsettings>().map["chbautojoin"].value<bool> ()) {
            ui.cbchannels->setCurrentIndex(ui.cbchannels->findText(
                    singleton<snpsettings>().map["joinonstartup"].value<QString> ()));
            joinclicked();
        }

    } else if (s == "chbminimized" && singleton<snpsettings>().map.contains(s))
        ui.chbminimized->setChecked(singleton<snpsettings>().map["chbminimized"].value<bool> ());

    else if (s == "inifilename" && singleton<snpsettings>().map.contains(s)
        && singleton<snpsettings>().map["chbiniischecked"].value<bool> ())
        ui.cbini->setCurrentIndex(ui.cbini->findText(
                singleton<snpsettings>().map["inifilename"].value<QString> ()));
    else if (s == "nickname" && singleton<snpsettings>().map.contains(s))
        ui.lenick->setText(singleton<snpsettings>().map["nickname"].value<QString> ());
    else if (s == "password" && singleton<snpsettings>().map.contains("password"))
        ui.lepassword->setText(singleton<snpsettings>().map["password"].value<QString> ());
    else if (s == "flag" && singleton<snpsettings>().map.contains(s))
        ui.flag->setCurrentIndex(ui.flag->findText(
                singleton<snpsettings>().map[s].value<QString> ()));
    else if (s == "rank" && singleton<snpsettings>().map.contains(s))
        ui.rank->setCurrentIndex(ui.rank->findText(
                singleton<snpsettings>().map[s].value<QString> ()));
    else if (s == "client" && singleton<snpsettings>().map.contains(s))
        ui.client->setText(singleton<snpsettings>().map[s].value<QString> ());
    else if (s == "clan" && singleton<snpsettings>().map.contains(s))
        ui.clan->setText(singleton<snpsettings>().map[s].value<QString> ());
    else if (s == "whichuitype" && singleton<snpsettings>().map.contains(s))
        whichuitype = singleton<snpsettings>().map["whichuitype"].value<int> ();
}
void mainwindow::pbeditorclicked() {
    editor->show();
    editor->start(ui.cbini->currentText());
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
    qDebug() << singleton<snpsettings>().map["language file"].value<QString> ();
    singleton<snpsettings>().safe();
    QMessageBox::StandardButton button = QMessageBox::question(this, tr(
            "Restart the application?"), tr(
                    "Changing the translation requires a program restart.\n"
                    "To restart now click Ok, otherwise cancel."), QMessageBox::Ok
                                                               | QMessageBox::Cancel);
    if (button == QMessageBox::Ok) {
        int i = 0;
        this->returntotabsettings(i);
        QProcess::startDetached(qApp->applicationFilePath(), QStringList(),
                                QApplication::applicationDirPath());
        foreach(::window *w,this->windowlist) {
            w->close();
        }
        singleton<balloon_handler>().tray->hide();
        //this->deleteLater();
        QTimer::singleShot(100, qApp,SLOT(quit()));
    }
}
void mainwindow::quitslot() {
    //qApp->exit();
}
void mainwindow::awayboxok() {
    singleton<balloon_handler>().set_away_tray_icon();
    foreach(::window *w,this->windowlist) {
        w->windowtitleaway = " " + tr("<away>:") + net->awaymessage;
        w->mysetwindowtitle();
    }
    net->wasaway = 1;
    net->isaway = 1;
}
void mainwindow::awaymessagechanged() {
    if (net->isaway) {
        foreach(::window *w,this->windowlist) {
            w->windowtitleaway = " " + tr("<away>:") + net->awaymessage;
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
    qDebug() << tr("trying to apply new textscheme: ") + file;
    singleton<snpsettings>().map["charformatfile"] = file;
    singleton<charformatsettings>().load();
    chathandler::initialformatstarter();
}
mainwindow::~mainwindow() {
}
void mainwindow::openchatwindow(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(net, s));
    /*if (singleton<snpsettings>().map["isarrangedbyonetab"].value<bool> () == true) {
         tabbox.addwidget(window::chatwindows.last());
         } else if (singleton<snpsettings>().map["isarrangedbytwotabs"].value<bool> () == true) {
         tabchatwindowbox.addwidget(window::chatwindows.last());
         if (tabchatwindowbox.isHidden()) {
         tabchatwindowbox.show();
         };
         } else*/
    window::chatwindows.last()->show();
    connect(window::chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
void mainwindow::openchatwindowhidden(const QString &s) {
    window::chatwindowstringlist << s;
    window::chatwindows.push_back(new chatwindow(net, s));
    /*if (singleton<snpsettings>().map["isarrangedbyonetab"].value<bool> () == true) {
         tabbox.addwidget(window::chatwindows.last());
         } else if (singleton<snpsettings>().map["isarrangedbytwotabs"].value<bool> () == true) {
         tabchatwindowbox.addwidget(window::chatwindows.last());
         if (tabchatwindowbox.isHidden()) {
         tabchatwindowbox.show();
         };
         } else*/
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
            if (i != -1 && net->users.users[i].nickfromclient != "Username")
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
    qDebug()<<"connected";
}
void mainwindow::disconnected(){
    ui.connectlabel->setText(tr("Disconnected"));
    qDebug()<<"disconnected";
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
    QAction *a0;
    stuffmenu = traymenu->addMenu(tr("Stuff"));
    stuffmenu->setIcon(QIcon("snppictures/stufficon.png"));
    stuffmenu->addAction(tr("About"));
    stuffmenu->addAction(tr("Settings"))->setIcon(QIcon("snppictures/settingsicon.png"));
    stuffmenu->addAction(tr("Volume"));
    stuffmenu->addAction(tr("Open Log Browser"));
    stuffmenu->addAction(usermodel::tr("Buddylist"));
    stuffmenu->addAction(tr("Scheme maker"));
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
    joinmenu->setIcon(QIcon("snppictures/joinicon.png"));
    QAction *a = new QAction(this);
    a->setText(tr("Away mode"));
    a->setIcon(QIcon("snppictures/awayicon.png"));
    traymenu->addAction(a);
    traymenu->addSeparator();
    a0 = traymenu->addAction(tr("Reconnect"));
    a0->setIcon(QIcon("snppictures/reconnect.png"));
    a0 = traymenu->addAction(tr("Close"));
    a0->setIcon(QIcon("snppictures/closeicon.png"));
    ui.labelButton->setIcon(QIcon(QApplication::applicationDirPath()
                                  + "/snppictures/trayiconpicture.png"));
    ui.labelButton->setMenu(traymenu);
    ui.labelButton->setFlat(1);
    singleton<balloon_handler>().tray->setContextMenu(traymenu);
    //connect(stylemenu, SIGNAL(triggered ( QAction *)),this, SLOT(traymenutriggered(QAction *)));
    connect(traymenu, SIGNAL(triggered ( QAction *)),this, SLOT(traymenutriggered(QAction *)));
    //connect(joinmenu, SIGNAL(triggered ( QAction *)),this, SLOT(traymenutriggered(QAction *)));
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
                this->raise();
            } else if (this->isVisible())
                this->close();
        }
    }
}
void mainwindow::traymenutriggered(QAction *a) {
    if (a->text() == tr("Close")) {        
        foreach(chatwindow *w,::window::chatwindows) {
            Q_ASSERT(w!=0);
            w->close();
            w->deleteLater();
        }
        foreach(::window *w,this->windowlist) {
            w->close();
            w->deleteLater();
        }
        singleton<balloon_handler>().hide_tray();
        qApp->quit();
        return;
    } else if (a->text().startsWith("#")) {
        ui.cbchannels->setCurrentIndex(ui.cbchannels->findText(a->text().split(
                " ").first()));
        this->joinclicked();
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
            try{
                QMessageBox::StandardButton button=QMessageBox::warning(this,tr("Warning"),
                                                                        tr("Changing the skin crashes sometimes, but The Wheat Snooper\n" \
                                                                           "will keep the settings for the next Start.\nDo you want to proceed?"),
                                                                        QMessageBox::Ok | QMessageBox::Cancel);
                if(button==QMessageBox::Ok)
                    qApp->setStyleSheet(stylesheet);
            }
            catch(...){
                qDebug()<<"Skinchanging failed, please try again"<<endl;
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
                                                    userstruct::whoami(s))].nickfromclient,
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
        if (net != 0)
            net->isaway = 0;
        singleton<balloon_handler>().set_normal_tray_icon();
        int i = 0;
        this->returntotabsettings(i);
        QTimer::singleShot(2000, this, SLOT(chooseclicked()));
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

void mainwindow::on_pbsettings_clicked()
{
    singleton<settingswindow>().show();
}

void mainwindow::on_pbabout_clicked()
{
    about *ab = new about;
    ab->show();
}
