#include "chatwindow.h"
#include"netcoupler.h"
#include "snpsettings.h"
#include"settingswindow.h"
#include"chathandlerprv.h"
#include"ctcphandler.h"
#include"mainwindow.h"
#include "sound_handler.h"
#include"global_functions.h"
#include<QKeyEvent>
#include<QDir>
#include<QTime>
#include<QFileDialog>
#include<QStatusBar>
#include<QMessageBox>
#include<QDebug>
extern QStringList querylist;
extern QMap<QString, QStringList> usergarbagemap;
chatwindow::chatwindow(netcoupler *n, const QString &s, QWidget *parent) :
	QWidget(parent), chatpartner(s), net(n) {
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setObjectName("chatwindow");
    ui.setupUi(this);    
    ui.pbbuddy->setObjectName("chatwindowbutton");
    ui.pbctcp->setObjectName("chatwindowbutton");
    ui.pbidle->setObjectName("chatwindowbutton");
    ui.pblog->setObjectName("chatwindowbutton");
    ui.pbmute->setObjectName("chatwindowbutton");
    ui.pbfilter->setObjectName("chatwindowbutton");
    ui.lineEdit->installEventFilter(this);
    ui.chatwindowbuttonscrollArea->installEventFilter(this);
    this->update();
    chat = new chathandlerprv(this, ui.chat, chatpartner);
    foreach(QString s,usergarbagemap[chatpartner.toLower()]) {
        chat->appendgarbage(s);
    }
    this->setWindowTitle(tr("Chat with") + " " + s);

    if (containsCI(singleton<snpsettings>().map["buddylist"].value<QStringList> (), chatpartner))
        this->setWindowIcon(QIcon(QApplication::applicationDirPath()
                                  + QDir::separator() + "snppictures" + QDir::separator()
                                  + "buddyicon.png"));
    else if (containsCI(singleton<snpsettings>().map["ignorelist"].value<QStringList> (),
			chatpartner))
        this->setWindowIcon(QIcon(QApplication::applicationDirPath()
                                  + QDir::separator() + "snppictures" + QDir::separator()
                                  + "ignoreicon.png"));
    else
        this->setWindowIcon(QIcon(QApplication::applicationDirPath()
                                  + QDir::separator() + "snppictures" + QDir::separator()
                                  + "usericon.png"));

    connect(ui.send, SIGNAL(clicked()),ui.lineEdit, SIGNAL(returnPressed()));
    connect(ui.lineEdit, SIGNAL(returnPressed()),this, SLOT(sendmsg()));
    connect(net, SIGNAL(sigusergarbage(const QString&,const QString&)),this, SLOT(garbagemapchanged(const QString&,const QString&)));    
    ui.lineEdit->setFocus(Qt::MouseFocusReason);

    if (containsCI(querylist, chatpartner)) { //best place to clean the querylist
        querylist.removeAll(chatpartner);
    }
    usesettingswindow();
    connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));

    singleton<sound_handler>().play_chatwindowopensound();    
    if (net->mutedusers.contains(this->chatpartner, Qt::CaseInsensitive))
        ui.pbmute->setIcon(QIcon("snppictures/buttons/nomutebutton.png"));
    else
        ui.pbmute->setIcon(QIcon("snppictures/buttons/mutebutton.png"));
    if (singleton<snpsettings>().map["buddylist"].value<QStringList> ().contains(
            this->chatpartner, Qt::CaseInsensitive))
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
    else
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/buddy.png"));

    ui.pblog->setIcon(QIcon("snppictures/buttons/log.png"));
    ui.pbidle->setIcon(QIcon("snppictures/buttons/idle.png"));
    ui.pbctcp->setIcon(QIcon("snppictures/buttons/ctcp.png"));
    ui.pbfilter->setIcon(QIcon("snppictures/buttons/filter.png"));
    ui.pbsmileys->setIcon(QIcon("snppictures/buttons/smileys.png"));
    ui.buttonlayout->setAlignment(Qt::AlignLeft);
    connect(ui.pbmute, SIGNAL(clicked()),this, SLOT(pbmuteclicked()));
    connect(ui.pbbuddy, SIGNAL(clicked()),this, SLOT(pbbuddyclicked()));
    connect(ui.pblog, SIGNAL(clicked()),this, SLOT(pblogclicked()));
    connect(ui.pbidle, SIGNAL(clicked()),this, SLOT(pbidleclicked()));
    connect(ui.pbctcp, SIGNAL(clicked()),this, SLOT(pbctcpclicked()));
    connect(ui.pbfilter, SIGNAL(clicked()),this, SLOT(filtergarbage()));
    connect(ui.pbsmileys, SIGNAL(clicked()),chat->emot, SLOT(show()));

    connect(net, SIGNAL(siggotidletime(const QString&, int)),this, SLOT(gotidletime(const QString&,int)));
    connect(net, SIGNAL(signosuchnick(const QString&)),this, SLOT(gotnosuchnick(const QString&)));
    this->statusbar = new QStatusBar(this);
    ui.verticalLayout_2->addWidget(statusbar);
    statusbar->setMaximumHeight(20);
    if (net->users.users.contains(userstruct::whoami(chatpartner))){
        statusbar->showMessage(tr("Online"));
        userisoffline=0;
    }
    else{
        statusbar->showMessage(tr("Was offline when this window opened."));
        userisoffline=1;
    }
    connect(net, SIGNAL(siggotmsg(const QString&,const QString&,const QString&)),this, SLOT(channelmsg(const QString&,const QString&,const QString&)));
}
bool chatwindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui.lineEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);            
            if (keyEvent->key() == Qt::Key_Up)
                qApp->postEvent(ui.lineEdit,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ControlModifier));
            else if (keyEvent->key() == Qt::Key_Down)
                qApp->postEvent(ui.lineEdit,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ShiftModifier | Qt::ControlModifier));
        }
        return false;
    }
    if (qobject_cast<QScrollArea*> (obj) != 0 && qobject_cast<QScrollArea*> (
            obj)->objectName() == "chatwindowbuttonscrollArea") {
        if (event->type() == QEvent::Enter) {
            if(ui.chatwindowbuttonscrollArea->width()<305){
                ui.chatwindowbuttonscrollArea->setMaximumHeight(70);
                ui.chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            }
        } else if (event->type() == QEvent::Leave) {
            ui.chatwindowbuttonscrollArea->setMaximumHeight(43);
            ui.chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    return QObject::eventFilter(obj, event);
}
void chatwindow::garbagemapchanged(const QString &user, const QString &s) {

    if (compareCI(user, chatpartner)) {
        bool b = usergarbagemap.contains(user.toLower());
        Q_UNUSED(b);
        Q_ASSERT(b==1);
        chat->appendgarbage(s);
    }
}
void chatwindow::gotmsg(const QString &user, const QString &receiver,
                        const QString &msg) {
    if (statusbar->currentMessage() == tr(
            "Was offline when this window opened."))
        statusbar->showMessage(tr("Online"));
    if (compareCI(user, chatpartner)) {
        chat->append(user, receiver, msg);
        QApplication::alert(this);emit
                sigalert(this);
        if (net->users.usermap[usermodel::tr("Buddylist")].count(
                userstruct::whoami(user))) {
            singleton<sound_handler>().play_buddymsgsound(user);

        } else {
            singleton<sound_handler>().play_normalmsgsound(user);
        }
    }
}
void chatwindow::sendmsg(QString str) {
    QString s;
    if (str == "")
        s = ui.lineEdit->text();
    else
        s = str;
    if (s != "") {
        if (s.startsWith(">!")) {
            net->sendrawcommand(QString("PRIVMSG ") + chatpartner + " :\001"
                                + s.remove(0, 2) + "\001");
            chat->append(net->nick, chatpartner, "\001" + s + "\001");
        } else if (s.startsWith("/")) {
            chat->append(net->nick, chatpartner, s);
            net->sendrawcommand(s.remove(0, 1));
        } else if (s.startsWith(">>>")) {
            sendnoticeaction();
            chat->append(net->nick, chatpartner, s);
        } else if (s.startsWith(">>")) {
            sendnotice();
            chat->append(net->nick, chatpartner, s);
        } else if (s.startsWith(">")) {
            net->sendrawcommand(QString("PRIVMSG ") + chatpartner
                                + " :\001ACTION " + s.remove(0, 1).remove("") + " \001\n");
            chat->append(net->nick, chatpartner, "\001ACTION " + s + " \001");
        } else {
            net->sendmessage(chatpartner, s);
            chat->append(net->nick, chatpartner, s);
        }
        usergarbagemap[chatpartner.toLower()] << QDate::currentDate().toString(
                "dd.MM") + " " + QTime::currentTime().toString("hh:mm") + " "
                + net->nick + ">" + s;
        ui.lineEdit->clear();
    }
}
void chatwindow::sendnotice() {

    const QString s = ui.lineEdit->text().remove(0, 2).remove("\n");
    if (s != "") {
        net->sendnotice(chatpartner, s);
        chat->appendnotice(net->nick, chatpartner, s);
        ui.lineEdit->clear();
    }
}
void chatwindow::sendnoticeaction() {

    const QString s = ui.lineEdit->text().remove(0, 3).remove("\n");
    if (s != "") {
        net->sendrawcommand(QString("NOTICE ") + chatpartner + " :\001ACTION "
                            + s + " \001");
        chat->appendnotice(net->nick, chatpartner, "<" + s + ">");
        ui.lineEdit->clear();
    }
}
void chatwindow::closeEvent(QCloseEvent *) {
}
void chatwindow::getgamerwho() {

    int i = net->users.users.indexOf(userstruct::whoami(chatpartner));
    if (i > -1) {
        userstruct u = net->users.users[i];
        chat->append(chatpartner, net->nick, " GAMERWHO:\n"
                     + u.returnwho().join(" | "));
    }
}
void chatwindow::usesettingswindow(const QString&) {
}
void chatwindow::pbmuteclicked() {
    if (net->mutedusers.contains(this->chatpartner, Qt::CaseInsensitive)) {
        foreach(QString s,net->mutedusers) {
            if (compareCI(s, this->chatpartner))
                net->mutedusers.removeAt(net->mutedusers.indexOf(s));
        }
        ui.pbmute->setIcon(QIcon("snppictures/buttons/mutebutton.png"));
    } else {
        net->mutedusers << this->chatpartner;
        ui.pbmute->setIcon(QIcon("snppictures/buttons/nomutebutton.png"));
    }
    singleton<snpsettings>().map["mutedusers"] = net->mutedusers;
}
void chatwindow::pbbuddyclicked() {
    if (singleton<snpsettings>().map["buddylist"].value<QStringList> ().contains(
            this->chatpartner, Qt::CaseInsensitive)) {
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/buddy.png"));
        QStringList sl = singleton<snpsettings>().map["buddylist"].value<QStringList> ();
        singleton<snpsettings>().map["buddylist"] = removeCI(sl, this->chatpartner);
    } else if (singleton<snpsettings>().map["ignorelist"].value<QStringList> ().contains(
            this->chatpartner)) {
        QStringList sl = singleton<snpsettings>().map["ignorelist"].value<QStringList> ();
        singleton<snpsettings>().map["ignorelist"] = removeCI(sl, this->chatpartner);
        singleton<snpsettings>().map["buddylist"] = singleton<snpsettings>().map["buddylist"].value<QStringList> ()
                                                    << this->chatpartner;
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
    } else {
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
        singleton<snpsettings>().map["buddylist"] = singleton<snpsettings>().map["buddylist"].value<QStringList> ()
                                                    << this->chatpartner;
    }
}
void chatwindow::pblogclicked() {
    QString s = ui.chat->toPlainText();
    QString file = QFileDialog::getSaveFileName(this, tr("Choose a file."),
                                                "*.log");
    QFile f(file);
    if (file != "" && f.open(QFile::WriteOnly)) {
        QTextStream ts(&f);
        ts << s;
    }
}
void chatwindow::pbidleclicked() {
    net->sendrawcommand("whois " + this->chatpartner);
}
void chatwindow::gotidletime(const QString &user, int i) {
    QTime time(0, 0, 0);
    time = time.addSecs(i);
    if (compareCI(user, this->chatpartner)) {
        chat->appenddebug(tr("<This users idle time: %1>").arg(time.toString(
                "hh:mm:ss")));
    }
}
void chatwindow::gotnosuchnick(const QString &s){
    if(compareCI(this->chatpartner,s)){
        chat->appenddebug(tr("This user is currently offline"));
        userisoffline=1;
    }
}
void chatwindow::pbctcpclicked() {
    singleton<ctctphandlerwidget>().show();
}
void chatwindow::setaway(bool b, const QString &msg) {
    if (b) {
        statusbar->showMessage(QString(msg).remove(0, 5).remove("\001"));
    } else {
        statusbar->showMessage(tr("Online"));
    }
}
void chatwindow::channelmsg(const QString &user, const QString &receiver,
                            const QString &msg) {
    if (user == this->chatpartner) {
        usergarbagemap[user.toLower()]
                << QDate::currentDate().toString("dd.MM") + " "
                + QTime::currentTime().toString("hh:mm") + " " + user
                + " to " + receiver + ">"
                + QString(msg).remove("\n").remove("\r");
        if (singleton<settingswindow>().from_map("chbshowchannelchatinchatwindows").toBool()) {
            chat->appendgarbage(QDate::currentDate().toString("dd.MM") + " "
                                + QTime::currentTime().toString("hh:mm") + " " + user
                                + " to " + receiver + ">"
                                + QString(msg).remove("\n").remove("\r"));
        }
    }
}
void chatwindow::filtergarbage() {
    ui.chat->clear();
    foreach(QString s,usergarbagemap[chatpartner.toLower()]) {
        if (!s.contains("JOIN") && !s.contains("PART") && !s.contains(
                "QUIT"))
            chat->appendgarbage(s);
    }
}
chatwindow::~chatwindow() {
    chat->disconnect();
    chat->deleteLater();
    emit
            closed();
}
