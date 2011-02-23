#include"chatwindow.h"
#include"netcoupler.h"
#include"settings.h"
#include"settingswindow.h"
#include"chathandlerprv.h"
#include"ctcphandler.h"
#include"sound_handler.h"
#include"global_functions.h"

#include<QKeyEvent>
#include<QDir>
#include<QTime>
#include<QFileDialog>
#include<QStatusBar>
#include<QMessageBox>
#include<QDesktopWidget>

extern QStringList querylist;
extern QHash<QString, QStringList> usergarbagemap;
chatwindow::chatwindow(const QString &s, QWidget *parent) :
        QWidget(parent), chatpartner(s){
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("chatwindow");
    ui.setupUi(this);    
    ui.pbbuddy->setObjectName("chatwindowbutton");
    ui.pbctcp->setObjectName("chatwindowbutton");
    ui.pbidle->setObjectName("chatwindowbutton");
    ui.pblog->setObjectName("chatwindowbutton");
    ui.pbmute->setObjectName("chatwindowbutton");
    ui.pbfilter->setObjectName("chatwindowbutton");    
    ui.chatwindowbuttonscrollArea->installEventFilter(this);
    update();    
    chat = new chathandlerprv(this, ui.chat, chatpartner);
    ui.lineEdit->installEventFilter(chat);
    if(S_S.cbsafequerys){
        foreach(QString s,usergarbagemap[chatpartner.toLower()]) {
            chat->appendgarbage(s);
        }
    }
    setWindowTitle(tr("Chat with") + " " + s);

    if (containsCI(S_S.buddylist, chatpartner))
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/buddyicon.png"));
    else if (containsCI(S_S.ignorelist, chatpartner))
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/ignoreicon.png"));
    else
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/usericon.png"));

    connect(ui.send, SIGNAL(clicked()),ui.lineEdit, SIGNAL(returnPressed()));
    connect(ui.lineEdit, SIGNAL(returnPressed()),this, SLOT(sendmsg()));
    connect(&singleton<netcoupler>(), SIGNAL(sigusergarbage(const QString&,const QString&)),this, SLOT(garbagemapchanged(const QString&,const QString&)));
    ui.lineEdit->setFocus(Qt::MouseFocusReason);

    if (containsCI(querylist, chatpartner)) { //best place to clean the querylist
        querylist.removeAll(chatpartner);
    }
    usesettingswindow();
    connect(&singleton<netcoupler>(), SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));

    singleton<sound_handler>().play_chatwindowopensound();    
    if (S_S.getstringlist("mutedusers").contains( chatpartner, Qt::CaseInsensitive))
        ui.pbmute->setIcon(QIcon("snppictures/buttons/nomutebutton.png"));
    else
        ui.pbmute->setIcon(QIcon("snppictures/buttons/mutebutton.png"));
    if (containsCI(S_S.buddylist, chatpartner))
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
    else
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/buddy.png"));

    ui.pblog->setIcon(QIcon("snppictures/buttons/log.png"));
    ui.pbidle->setIcon(QIcon("snppictures/buttons/idle.png"));
    ui.pbctcp->setIcon(QIcon("snppictures/buttons/ctcp.png"));
    ui.pbfilter->setIcon(QIcon("snppictures/buttons/filter.png"));
    ui.pbsmileys->setIcon(QIcon("snppictures/buttons/smileys.png"));
    ui.pbresize->setIcon(QIcon("snppictures/buttons/resize.png"));
    ui.buttonlayout->setAlignment(Qt::AlignLeft);
    connect(ui.pbmute, SIGNAL(clicked()),this, SLOT(pbmuteclicked()));
    connect(ui.pbbuddy, SIGNAL(clicked()),this, SLOT(pbbuddyclicked()));
    connect(ui.pblog, SIGNAL(clicked()),this, SLOT(pblogclicked()));
    connect(ui.pbidle, SIGNAL(clicked()),this, SLOT(pbidleclicked()));
    connect(ui.pbctcp, SIGNAL(clicked()),this, SLOT(pbctcpclicked()));
    connect(ui.pbfilter, SIGNAL(clicked()),this, SLOT(filtergarbage()));
    connect(ui.pbsmileys, SIGNAL(clicked()),chat->emot, SLOT(show()));

    connect(&singleton<netcoupler>(), SIGNAL(siggotidletime(const QString&, int)),this, SLOT(gotidletime(const QString&,int)));
    connect(&singleton<netcoupler>(), SIGNAL(signosuchnick(const QString&)),this, SLOT(gotnosuchnick(const QString&)));
    statusbar = new QStatusBar(this);
    ui.verticalLayout_2->addWidget(statusbar);
    statusbar->setMaximumHeight(20);
    if (singleton<netcoupler>().users.users.contains(userstruct::whoami(chatpartner))){
        statusbar->showMessage(QObject::tr("Online"));
        userisoffline=0;
    }
    else{
        statusbar->showMessage(tr("Was offline when this window opened."));
        userisoffline=1;
    }
    connect(&singleton<netcoupler>(), SIGNAL(siggotmsg(const QString&,const QString&,const QString&)),this, SLOT(channelmsg(const QString&,const QString&,const QString&)));
}
bool chatwindow::eventFilter(QObject *obj, QEvent *event) {
    if (qobject_cast<QScrollArea*> (obj) == ui.chatwindowbuttonscrollArea) {
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
    return false;
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
    if (compareCI(user, chatpartner)) {
        chat->append(user, receiver, msg);
        QApplication::alert(this);
        emit sigalert(this);
        if (singleton<netcoupler>().users.usermap[usermodel::tr("Buddylist")].count(userstruct::whoami(user)))
            singleton<sound_handler>().play_buddymsgsound(user);
        else
            singleton<sound_handler>().play_normalmsgsound(user);        
        if (statusbar->currentMessage() == tr("Was offline when this window opened."))
            statusbar->showMessage(QObject::tr("Online"));
    }
}
void chatwindow::sendmsg() {
    QString s = ui.lineEdit->text();
    if (s == "")
        return;
    if (s.startsWith(">!")) {
        s.remove(0, 2);
        chat->append(singleton<netcoupler>().nick, chatpartner, "\001" + s);
        singleton<netcoupler>().sendrawcommand(QString("PRIVMSG ") + chatpartner + " :\001" + s + "\001");
    } else if (s.startsWith("/")) {
        chat->append(singleton<netcoupler>().nick, chatpartner, s);
        singleton<netcoupler>().sendrawcommand(s.remove(0, 1));
    } else if (s.startsWith(">>>"))
        sendnoticeaction();
    else if (s.startsWith(">>"))
        sendnotice();
    else if (s.startsWith(">")) {
        s.remove(0, 1).remove("");
        chat->append(singleton<netcoupler>().nick, chatpartner, "\001ACTION " + s + " \001");
        singleton<netcoupler>().sendrawcommand(QString("PRIVMSG ") + chatpartner + " :\001ACTION " + s + " \001\n");
    } else {
        chat->append(singleton<netcoupler>().nick, chatpartner, s);
        singleton<netcoupler>().sendmessage(chatpartner, s);
    }
    usergarbagemap[chatpartner.toLower()] << QDate::currentDate().toString(
            "dd.MM") + " " + QTime::currentTime().toString("hh:mm") + " "
            + singleton<netcoupler>().nick + ">" + s;
    ui.lineEdit->clear();
    chat->moveSliderToMaximum();
}
void chatwindow::sendnotice() {

    const QString s = ui.lineEdit->text().remove(0, 2).remove("\n");
    if (s == "")
        return;
    singleton<netcoupler>().sendnotice(chatpartner, s);
    chat->appendnotice(singleton<netcoupler>().nick, chatpartner, s);
    ui.lineEdit->clear();
}
void chatwindow::sendnoticeaction() {

    const QString s = ui.lineEdit->text().remove(0, 3).remove("\n");
    if (s == "")
        return;
    singleton<netcoupler>().sendrawcommand(QString("NOTICE ") + chatpartner + " :\001ACTION " + s + " \001");
    chat->appendnotice(singleton<netcoupler>().nick, chatpartner, "<" + s + ">");
    ui.lineEdit->clear();
}
void chatwindow::closeEvent(QCloseEvent *) {
}
void chatwindow::getgamerwho(QString prefix) {

    int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(chatpartner));
    if (i > -1) {
        userstruct u = singleton<netcoupler>().users.users[i];
        chat->append(chatpartner, singleton<netcoupler>().nick, prefix+" GAMERWHO:\n"
                     + u.returnwho().join(" | "));
    }
}
void chatwindow::usesettingswindow(const QString&) {
}
void chatwindow::pbmuteclicked() {
    if (S_S.getstringlist("mutedusers").contains(chatpartner, Qt::CaseInsensitive)) {
        foreach(QString s,S_S.getstringlist("mutedusers")) {
            if (compareCI(s, chatpartner))
                S_S.remove("mutedusers",s);
        }
        ui.pbmute->setIcon(QIcon("snppictures/buttons/mutebutton.png"));
    } else {
        S_S.append("mutedusers", chatpartner);
        ui.pbmute->setIcon(QIcon("snppictures/buttons/nomutebutton.png"));
    }    
}
void chatwindow::pbbuddyclicked() {
    if (containsCI(S_S.buddylist, chatpartner)) {
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/buddy.png"));
        QStringList sl = S_S.buddylist;
        S_S.set("buddylist", removeCI(sl, chatpartner));
    } else if (containsCI(S_S.ignorelist, chatpartner)) {
        QStringList sl = S_S.ignorelist;
        S_S.set("ignorelist", removeCI(sl, chatpartner));
        S_S.set("buddylist", S_S.buddylist << chatpartner);
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
    } else {
        ui.pbbuddy->setIcon(QIcon("snppictures/buttons/normal.png"));
        S_S.set("buddylist", S_S.buddylist << chatpartner);
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
    singleton<netcoupler>().sendrawcommand("whois " +  chatpartner);
}
void chatwindow::gotidletime(const QString &user, int i) {
    QTime time(0, 0, 0);
    time = time.addSecs(i);
    if (compareCI(user,  chatpartner)) {
        chat->appenddebug(tr("<This users idle time:")+" " +time.toString("hh:mm:ss"));
    }
}
void chatwindow::gotnosuchnick(const QString &s){
    if(compareCI( chatpartner,s)){
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
        statusbar->showMessage(QObject::tr("Online"));
    }
}
void chatwindow::channelmsg(const QString &user, const QString &receiver,
                            const QString &msg) {
    if (user ==  chatpartner) {
        usergarbagemap[user.toLower()]
                << QDate::currentDate().toString("dd.MM") + " "
                + QTime::currentTime().toString("hh:mm") + " " + user
                + " to " + receiver + ">"
                + QString(msg).remove("\n").remove("\r");
        if (S_S.getbool("chbshowchannelchatinchatwindows")) {
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

void chatwindow::on_pbresize_clicked()
{
    static int clickCounter=0;
    static int oldwidth;
    static int screenCount=qApp->desktop()->screenCount();
    static int desktopWidth=qApp->desktop()->size().width();

    if(clickCounter==0)
        oldwidth=size().width();

    clickCounter=(clickCounter+1)%3;
    switch(clickCounter){
    case 0:
        resize(oldwidth,height());
        break;
    case 1:
        resize((desktopWidth/2)/screenCount,height());
        break;
    case 2:
        resize((3*desktopWidth/4)/screenCount,height());
        break;
    }
}
