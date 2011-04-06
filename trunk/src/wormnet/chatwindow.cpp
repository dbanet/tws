#include"chatwindow.h"
#include"netcoupler.h"
#include"settings.h"
#include"settingswindow.h"
#include"chathandlerprv.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"usermessage.h"
#include"balloon_handler.h"
#include"emoticonsdialog.h"

#include<QKeyEvent>
#include<QDir>
#include<QTime>
#include<QFileDialog>
#include<QStatusBar>
#include<QMessageBox>
#include<QDesktopWidget>

extern QStringList querylist;
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
    foreach(usermessage u,history()[chatpartner.toLower()])
        chat->append(u);
    setWindowTitle(tr("Chat with") + " " + s);

    if (containsCI(S_S.buddylist, chatpartner))
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/buddyicon.png"));
    else if (containsCI(S_S.ignorelist, chatpartner))
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/ignoreicon.png"));
    else
        setWindowIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/usericon.png"));

    connect(ui.send, SIGNAL(clicked()),ui.lineEdit, SIGNAL(returnPressed()));
    connect(ui.lineEdit, SIGNAL(returnPressed()),this, SLOT(sendmsg()));    
    ui.lineEdit->setFocus(Qt::MouseFocusReason);                

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
}
void chatwindow::showEvent(QShowEvent * event ){
    querylist.removeAll(chatpartner);
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
void chatwindow::getusermessage(usermessage u_arg){
    if(u_arg.receiver() == singleton<netcoupler>().nick && u_arg.user() == chatpartner){
        chat->append(u_arg);
        singleton<balloon_handler>().alert(u_arg.user(), this);
        if (singleton<netcoupler>().buddylistcontains(u_arg.user()))
            singleton<sound_handler>().play_buddymsgsound(u_arg.user());
        else
            singleton<sound_handler>().play_normalmsgsound(u_arg.user());
        if (statusbar->currentMessage() == tr("Was offline when this window opened."))
            statusbar->showMessage(QObject::tr("Online"));
    } else if(u_arg.user() == chatpartner && S_S.chbshowchannelchatinchatwindows){
        chat->append(u_arg.add_type(e_CHANNELMSGTOCHAT));
        return;
    }
}
void chatwindow::sendmsg() {
    QString s = ui.lineEdit->text();
    if (s.isEmpty())
        return;
    usermessage u=usermessage::create(s, chatpartner);
    chat->append(u);    
    singleton<netcoupler>().sendusermessage(u);

    ui.lineEdit->clear();
    chat->moveSliderToMaximum();
}
void chatwindow::closeEvent(QCloseEvent *) {
}
void chatwindow::getgamerwho(QString prefix) {
    int i = singleton<netcoupler>().users.users.indexOf(userstruct::whoami(chatpartner));
    if (i > -1) {
        userstruct u = singleton<netcoupler>().users.users[i];
        QString s=prefix+" GAMERWHO:\n"+ u.returnwho().join(" | ");
        chat->append(usermessage(s, e_PRIVMSG, chatpartner));
    }
}
void chatwindow::pbmuteclicked() {
    if (containsCI(S_S.getstringlist("mutedusers"), chatpartner)) {
        S_S.remove("mutedusers",chatpartner);
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
        S_S.append("buddylist", chatpartner);
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
    singleton<netcoupler>().sendusermessage(usermessage("whois " + chatpartner, e_RAWCOMMAND, chatpartner));
}
void chatwindow::gotidletime(const QString &user, int i) {
    QTime time(0, 0, 0);
    time = time.addSecs(i);
    if (compareCI(user,  chatpartner))
        chat->appenddebug(tr("<This users idle time:")+" " +time.toString("hh:mm:ss"));    
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
        statusbar->showMessage(QString(msg).remove(0, 5));
    } else {
        statusbar->showMessage(QObject::tr("Online"));
    }
}
void chatwindow::filtergarbage() {
    ui.chat->clear();
    foreach(usermessage u,history(chatpartner.toLower())) {
        if (!u.has_type(e_GARBAGEJOIN) && !u.has_type(e_GARBAGEPART) && !u.has_type(e_GARBAGEQUIT))
            chat->append(u);
    }
}
chatwindow::~chatwindow() {
    chat->disconnect();
    chat->deleteLater();
    emit
            sigclosed();
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

void chatwindow::on_pbsmiley_clicked(){
    emoticonsdialog *dia = new emoticonsdialog;
    dia->move(QCursor::pos()-QPoint(dia->width(),dia->height()));
    dia->show();
    dia->raise();
    dia->setFocus();
    connect(dia,SIGNAL(sigemotchoosed(QString)),this,SLOT(insertemot(QString)));
}
void chatwindow::insertemot(QString s){
    QString ss=ui.lineEdit->text();
    if(ss.endsWith(" ") || s.isEmpty())
        ui.lineEdit->insert(s + " ");
    else
        ui.lineEdit->insert(" " + s + " ");
    ui.lineEdit->setFocus ();
}
