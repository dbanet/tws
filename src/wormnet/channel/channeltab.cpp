#include <QHBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QProcess>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QMessageBox>
#include <QDate>
#include <QScrollBar>
#include <QDesktopServices>
#include <QListView>

#include "channeltab.h"
#include "ui_channeltab.h"
#include "ui_channelmenu.h"
#include "../chatwindow.h"
#include "../settings.h"
#include "../mainwindow.h"
#include "../chathandler.h"
#include "../hostbox.h"
#include "../settingswindow.h"
#ifdef PHONON
#include "../sound_handler.h"
#endif
#include "../global_functions.h"
#include "../clantowebpagemapper.h"
#include "../leagueserverhandler.h"
#include "../usermessage.h"
#include "../balloon_handler.h"
#include "../emoticonsdialog.h"

QList<chatwindow*> channelTab::chatwindows;
QStringList channelTab::chatwindowstringlist;
QList< ::channelTab*> channelTab::hiddenchannelwindowshelper;
extern QStringList querylist;
channelTab::channelTab(QString currentChannel,QMenu *serverMenu,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::channelTab)
{
    qDebug()<<"channelTab::window(QString s,int i)///";
    this->currentChannel=QString(currentChannel);
    this->chaticon=QIcon("snppictures/Chat_Icon.png");
    setAttribute(Qt::WA_DeleteOnClose);
    setObjectName("channelTab");
    ui->setupUi(this);

    qDebug()<<"ui->users->setAlternatingRowColors(1)";

    /***********************/
    /* setting up hostview */
    /***********************/
    this->hosts=new QList<hoststruct>();                                 // something to initialize the model with...
    this->hostModel=new HostModel(currentChannel,hosts);                 // model itself (blank for now)
    ui->hosts->setModel(this->hostModel);                                // setting it
    ui->hosts->setEnabled(true);
    ui->hosts->setSortingEnabled(true);
    ui->hosts->setColumnWidth(0,22);                                     // flag icons width
    ui->hosts->setColumnWidth(1,16);                                     // locked/unlocked icon width
    ui->hosts->setColumnWidth(2,15);                                     // buddy/ignore/plain-host icon
    ui->hosts->setRootIsDecorated(false);                                // that small dots; we don't need them
    ui->hosts->header()->setStretchLastSection(false);                   // the 'gamename' section will be stretched
    ui->hosts->header()->setResizeMode(0,QHeaderView::Fixed);            // the user shouldn't be able to resize neither flag,
    ui->hosts->header()->setResizeMode(1,QHeaderView::Fixed);            // nor locked/unlocked,
    ui->hosts->header()->setResizeMode(2,QHeaderView::Fixed);            // nor buddy/ignore/plain-host icons columns
    ui->hosts->header()->setResizeMode(3,QHeaderView::Stretch);          // the gamename section is set to be stretched (the biggest)
    ui->hosts->header()->setResizeMode(4,QHeaderView::ResizeToContents); // the hoster's nick is always visible fully (it's never too big)
    //ui->hosts->header()->setResizeMode(5,QHeaderView::Interactive);    // and the default for the IP. Many users won't like it occupying too much space
    ui->hosts->header()->setResizeMode(5,QHeaderView::ResizeToContents); // but l8r I thought there are lot of space... also I like to see it...
    ui->hosts->header()->setSortIndicatorShown(true);                    // why not?

    ui->users->setAlternatingRowColors(1);
    ui->users->installEventFilter(this);
    connect(&singleton<netcoupler>().users, SIGNAL(sigselectitem(const QModelIndex&,const QWidget*)),this, SLOT(setselection(const QModelIndex&,const QWidget*)));
    chat = new chatHandler(this, ui->chat, currentChannel);
    ui->msg->installEventFilter(chat);
    connect(chat, SIGNAL(sigopenchatwindow(const QString&)),this, SLOT(openchatwindow(const QString&)));
    connect(ui->send, SIGNAL(clicked()),ui->msg, SIGNAL(returnPressed()));
    ui->users->setModel(&singleton<netcoupler>().users);
    connect(ui->users->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection&,const QItemSelection&)),this, SLOT(userselectionchanged(const QItemSelection&,const QItemSelection&)));
    ui->users->setEnabled(1);
    ui->users->header()->swapSections(0, 1);
    ui->users->header()->swapSections(1, 2);
    ui->users->setColumnWidth(0, 180);
    ui->users->setColumnWidth(3, 48);
    ui->users->setColumnWidth(4, 120);

    qDebug()<<"ui->users->setSortingEnabled(1)";
    ui->users->setSortingEnabled(1);
    ui->users->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui->users->header()->setSortIndicator(1, Qt::AscendingOrder);
    ui->users->header()->setSortIndicator(2, Qt::AscendingOrder);
    ui->users->header()->setSortIndicator(3, Qt::AscendingOrder);
    ui->users->header()->setSortIndicatorShown(1);
    connect(ui->users->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),&singleton<netcoupler>().users, SLOT(sortslot(int, Qt::SortOrder)));

    qDebug()<<"joinmenu2.setTitle(tr(\"Join\"))";
    joinmenu2.setTitle(tr("Join"));
    hostmenu.addAction(tr("Host a game in ") + currentChannel);
    joinmenu.addMenu(&joinmenu2);
    usermenu.addAction(tr("Add this user to Buddylist."));
    usermenu.addAction(tr("Add this user to Ignorelist."));
    usermenu.addSeparator();
    usermenu.addAction(tr("Show info about this user."))->setIcon(chaticon);
    customlistmenu.addAction(tr("Remove this user from the list."));
    customlistmenu.addAction(tr("Show info about this user."))->setIcon(chaticon);

    qDebug()<<"connect(ui->users, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(useritemdblclicked(const QModelIndex&)))";
    connect(ui->users, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(useritemdblclicked(const QModelIndex&)));
    connect(ui->users, SIGNAL(pressed(const QModelIndex&)),this, SLOT(useritempressed(const QModelIndex&)));
    connect(ui->hosts, SIGNAL(pressed(const QModelIndex&)),this, SLOT(hostitempressed(const QModelIndex&)));
    connect(ui->hosts, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(hostitemdblclicked(const QModelIndex&)));
    //connect(ui->pbsmiley,SIGNAL(clicked()),this,SLOT(pbemotclicked()));
    connect(ui->actionHost,SIGNAL(triggered()),this,SLOT(openHostBox()));
    connect(ui->actionRefresh,SIGNAL(triggered()),&singleton<netcoupler>(),SLOT(refreshhostlist()));
    connect(&singleton<netcoupler>(),SIGNAL(sigJoinedChannel(QString,int)),this,SLOT(setupWindowTitleOnJoin(QString,int)));
    connect(&singleton<netcoupler>(),SIGNAL(sigUpdatedAmountOfUsers(QString,int)),this,SLOT(setupWindowTitleOnChangeOfUserAmount(QString,int)));
    connect(ui->msg, SIGNAL(returnPressed()),this, SLOT(sendmsg()));

    qDebug()<<"ui->msg->setFocus(Qt::MouseFocusReason)";
    ui->msg->setFocus(Qt::MouseFocusReason);

    ui->users->setColumnWidth(1, 22);
    ui->users->setColumnWidth(2, 48);
    ui->users->header()->setResizeMode(1, QHeaderView::Fixed);
    ui->users->header()->setResizeMode(2, QHeaderView::Fixed);

    qDebug()<<"Setting up the top right corner to belong to the right docking widget area...";
    this->setCorner(Qt::TopRightCorner,Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

    qDebug()<<"Locking the UI...";
    this->lockUI();

    windowtitlechannel=currentChannel;
    channelMenu=0;
    this->addAction(ui->actionDockUndock);
    qDebug()<<"///channelTab::window(QString s,int i)";
}
void channelTab::pbemotclicked(){
    emoticonsdialog *dia = new emoticonsdialog;
    dia->move(QCursor::pos()-QPoint(dia->width(),dia->height()));
    dia->show();
    dia->raise();
    dia->setFocus();
    connect(dia,SIGNAL(sigemotchoosed(QString)),this,SLOT(insertemot(QString)));
}
void channelTab::insertemot(QString s){
    QString ss=ui->msg->text();
    if(ss.endsWith(" ") || s.isEmpty())
        ui->msg->insert(s + " ");
    else
        ui->msg->insert(" " + s + " ");
    ui->msg->setFocus ();
}
void channelTab::expandchannels() { //expand on startup
    ui->users->setExpanded(singleton<netcoupler>().users.indexbychannelname(currentChannel), 1);
    ui->users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Querys")), 1);
    if(S_S.getbool("cbopenbuddylistonstartup"))
        ui->users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Buddylist")), 1);
    if(ui->users->isExpanded(
           singleton<netcoupler>().users.index(
               singleton<netcoupler>().users.classes.indexOf(currentChannel)
              ,0
           )
       )
    ){
        //disconnect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(QStringList)),this, SLOT(expandchannels(QStringList)));
    } else QTimer::singleShot(500, this, SLOT(expandchannels()));
}
void channelTab::setselection(const QModelIndex &index, const QWidget *w) {
    if (w == this) {
        if (!index.isValid())
            ui->users->clearSelection();
        else {
            ui->users->clearSelection();
            ui->users->selectionModel()->select(index, QItemSelectionModel::Rows
                                               | QItemSelectionModel::Select);
        }
    }
}
void channelTab::userselectionchanged(const QItemSelection &selected,
                                  const QItemSelection&) {

    if (!selected.indexes().isEmpty())
        singleton<netcoupler>().users.selectionchanged(selected.indexes().first(), this);
}
bool channelTab::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->users ) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Return) {
                QModelIndex index=ui->users->currentIndex();
                if (index.internalId() == usermodel::e_Channel)
                    return false;
                QString s = singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ();
                if (s != "")
                    openchatwindow(s);
                return true;
            } else if (keyEvent->key() == Qt::Key_Space) {
                QString s = singleton<netcoupler>().users.data(ui->users->currentIndex().sibling(
                                                                   ui->users->currentIndex().row(), 0)).value<QString> ();
                ui->msg->insert(s + " ");
                ui->msg->setFocus(Qt::MouseFocusReason);
                return true;
            }
            //            if(keyEvent->key()==Qt::Key_F && keyEvent->modifiers()==Qt::ControlModifier)
            //            {
            //                //usersearchwidget *s=new usersearchwidget; //extendent search for users
            //                //s->show();                                //stays unfinisched
            //            }
        }
    }
    return false;
}
void channelTab::getusermessage(usermessage u){
    QString user=u.user();
    if(u.has_type(e_GARBAGE))
        return chat->append(u);
    if(u.has_type(e_NOTICE)){
        singleton<balloonHandler>().alert(u.user(), this);
        chat->append(u);
        return;
    }
    if (u.receiver().toLower()!=currentChannel.toLower() && !containsCI(chatwindowstringlist, u.user())) {
        if (!singleton<netcoupler>().ignorelistcontains(user) && !singleton<netcoupler>().buddylistcontains(user)) {
            if (!containsCI(querylist, user))
                querylist << user;
            u.add_type(e_CHATMSGTOCHANNEL);
            chat->append(u);
            if (S_S.getbool("cbalertfromnormal")) {
                singleton<balloonHandler>().alert(u.user(), this);
            }
#ifdef PHONON
            singleton<soundHandler>().play_normalmsgsound(user);
#endif
        }
    } else if (compareCI(u.receiver(), currentChannel)){
        if(S_S.getbool("cbignorysappearinchannel"))
            chat->append(u);
        else if (!containsCI(S_S.ignorelist, user))
            chat->append(u);
    }
}
void channelTab::gotdebugmsg(const QString &msg) {
    chat->appenddebug(msg.simplified());
}
void channelTab::sendmsg() {
    QString s = ui->msg->text();
    if (s.isEmpty())
        return;
    usermessage u=usermessage::create(s, singleton<netcoupler>().nick, currentChannel);
    chat->append(u);
    singleton<netcoupler>().sendusermessage(u);
    ui->msg->clear();
    chat->moveSliderToMaximum();
}
void channelTab::closeEvent(QCloseEvent * /*event*/) {
    QVariantList windowstates;
    windowstates << saveGeometry();
    windowstates << ui->users->header()->saveState();
    windowstates << ui->hosts->header()->saveState();
    windowstates << ui->hosts->header()->sectionSize(3);
    windowstates << ui->hosts->header()->sectionSize(4);
    windowstates << ui->users->header()->sectionSize(3);
    windowstates << ui->users->header()->sectionSize(4);
    S_S.set(currentChannel + QString::number(whichuiison), windowstates);
}
void channelTab::useritempressed(const QModelIndex &index) {
    if (!index.isValid())
        return;
    if (QApplication::mouseButtons() == Qt::LeftButton && index.column()==usermodel::e_Client){
        QString s=singleton<netcoupler>().users.data(index.sibling(index.row(), usermodel::e_Client)).toString();
        if(isClickableLink(s)){
            s=s.simplified();
            QUrl u1;
            u1.setUrl(s);
            QDesktopServices::openUrl(u1);
        }
        return;
    }
    if (QApplication::mouseButtons() == Qt::LeftButton && index.column()==usermodel::e_Clan){
        QString s;
        if(S_S.spectateleagueserver){
            QString nick=singleton<netcoupler>().users.data(index.sibling(index.row(), 0),Qt::DisplayRole).value<QString> ();
            s=singleton<leagueserverhandler>().map_at_toString(nick,leagueserverhandler::e_clan);
        }
        if(s.isEmpty())
            s=singleton<netcoupler>().users.data(index.sibling(index.row(), usermodel::e_Clan)).toString();
        if(singleton<clantowebpagemapper>().contains(s)){
            s=singleton<clantowebpagemapper>().value(s);
            QUrl u1;
            u1.setUrl(s);
            QDesktopServices::openUrl(u1);
        }
    }
    if (QApplication::mouseButtons() != Qt::RightButton)
        return;
    QAction *a;
    QMenu menu;
    QString user = singleton<netcoupler>().users.data(index.sibling(index.row(), 0),Qt::DisplayRole).value<QString> ();
    if (index.internalId() == usermodel::e_Channel) {
        if(singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ()==usermodel::tr("Querys")){
            menu.addAction(tr("Remove Querys"));
            a = menu.exec(QCursor::pos());
            if(a && a->text()==tr("Remove Querys")){
                querylist.clear();
                safequerylist();
            }
        }
    } else if (index.column() == usermodel::e_Clan) {
        QStringList sl = S_S.dissallowedclannames;
        if (sl.contains(singleton<netcoupler>().users.getuserstructbyindex(index).clan,Qt::CaseInsensitive))
            menu.addAction(tr("Allow this clanname."));
        else
            menu.addAction(tr("Dissallow this clanname."));
        a = menu.exec(QCursor::pos());
        if (!a) return;
        if (a->text() == tr("Allow this clanname."))
            removeCI(sl,singleton<netcoupler>().users.getuserstructbyindex(index).clan);
        else if(a->text() == tr("Dissallow this clanname."))
            sl<< singleton<netcoupler>().users.getuserstructbyindex(index).clan;
        else
            showInformationAboutClan(singleton<netcoupler>().users.getuserstructbyindex(index).clan);
        S_S.set("dissallowedclannames", sl);
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Querys")) {
        QMenu menu;
        menu.addAction(tr("Remove this Query."));
        menu.addSeparator();
        menu.addAction(tr("Show info about this user."))->setIcon(chaticon);
        a = menu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this Query."))
                querylist.removeAll(user);
            else if (a->text() == tr("Show info about this user."))
                getuserinfo(user);
        }
    } else if (singleton<netcoupler>().users.classes[index.internalId()] != usermodel::tr("Buddylist")
               && singleton<netcoupler>().users.classes[index.internalId()] != usermodel::tr("Ignorelist")) {
        QMenu menu;
        if (containsCI(S_S.buddylist, user)) {
            menu.addAction(tr("Remove this user from Buddylist."));
            menu.addSeparator();
            menu.addAction(tr("Show info about this user."))->setIcon(
                        chaticon);
            a = menu.exec(QCursor::pos());
        } else if (containsCI(S_S.ignorelist, user)) {
            menu.addAction(tr("Remove this user from Ignorelist."));
            menu.addSeparator();
            menu.addAction(tr("Show info about this user."))->setIcon(chaticon);
            a = menu.exec(QCursor::pos());
        } else
            a = usermenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Add this user to Buddylist.")) {
                singleton<netcoupler>().users.addbuddy(user);
            } else if (a->text() == tr("Add this user to Ignorelist.")) {
                singleton<netcoupler>().users.addignore(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            } else if (a->text() == tr("Remove this user from Buddylist.")) {
                singleton<netcoupler>().users.deletebuddy(user);
            } else if (a->text() == tr("Remove this user from Ignorelist.")) {
                singleton<netcoupler>().users.deleteignore(user);
            }
        } else
            return;
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Buddylist")) {
        QAction *a = customlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deletebuddy(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Ignorelist")) {
        a = customlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deleteignore(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    }
}
void channelTab::useritemdblclicked(const QModelIndex &index) {

    if (index.internalId() != usermodel::e_Channel
            && singleton<netcoupler>().users.classes[index.internalId()]!= usermodel::tr("Ignorelist")) {
        QString s = singleton<netcoupler>().users.data(index.sibling(index.row(), 0), Qt::DisplayRole).value<QString> ();
        openchatwindow(s);
    } else if (index.internalId() == usermodel::e_Channel){
        QString s=singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ();
        if(s==usermodel::tr("Buddylist") || s==usermodel::tr("Ignorelist") || s==usermodel::tr("Querys"))
            return;
        emit sigjoinchannel(s);
    }
}
void channelTab::hostitemdblclicked(const QModelIndex &index) {
    if (index.internalId() != 999) {
        QString hostinfo = " \"" + hosts->at(index.row()).joinstring() + "&scheme="
                + singleton<netcoupler>().schememap[currentChannel] + "\"";
        QString gamename = hosts->at(index.row()).name();
        singleton<netcoupler>().joingame(hostinfo, currentChannel, gamename);
    } else if (index.internalId() == 999) {
        openHostBox();
    }
}
void channelTab::getuserinfo(const QString &s) {
    QString msg;
    if(S_S.spectateleagueserver){
        QString url=singleton<leagueserverhandler>().map_at_toString(s,leagueserverhandler::e_webpage);
        if(!url.isEmpty()){
            QDesktopServices::openUrl(QUrl(url));
            return;
        }
        QString service=singleton<leagueserverhandler>().service_name();
        msg=tr("Sry but there is no webside for this user available, at ")+service;
        myDebug()<<msg;
    }
    foreach(QString chat,chatwindowstringlist) {
        if (compareCI(chat, s)) {
            foreach(chatwindow *c,chatwindows) {
                if (compareCI(chat, c->chatpartner)) {
                    c->getgamerwho(msg);
                    return;
                }
            }
        }
    }
    openchatwindow(s);
    chatwindows.last()->getgamerwho();
}
void channelTab::openchatwindow(const QString &s) {
    /*Q_ASSERT(s!="");
    if (containsCI(chatwindowstringlist, s)){
        foreach(chatwindow *w,chatwindows){
            if(w->chatpartner==s && w->isHidden())
                w->show();
            if(w->chatpartner==s){
                w->raise();
                qApp->setActiveWindow(w);
            }
            qobjectwrapper<MainWindow>::ref().hiddenchatwindowshelper.removeAll(w);
            querylist.removeAll(s);
        }
        return;
    }emit sigopenchatwindow(s);
    QApplication::processEvents();*/
}
void channelTab::hostitempressed(const QModelIndex &index) {
    if (QApplication::mouseButtons() == Qt::RightButton) {
        QString hostinfo = " \"" + hosts->at(index.row()).joinstring() + "&scheme="
                + singleton<netcoupler>().schememap[currentChannel] + "\"";
        QString gamename = hosts->at(index.row()).name();
        if (index.internalId() == 999) {
            openHostBox();
        } else {
            getjoinmenu();
            QAction *a = joinmenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Choose a Program to join this game.")) {
                    QStringList sl = S_S.getStringList("joinstrings");
#ifdef Q_OS_UNIX
                    QString file = QFileDialog::getOpenFileName(this, tr(
                                                                    "Choose a desktop icon."), "/home", "Linux Desktop Icon (*.desktop);;WINE Executable File (*.exe *.cmd)");
#elif defined(Q_OS_WIN32) | defined(Q_OS_OS2)
                    QString file = QFileDialog::getOpenFileName(this, tr("Choose a Program."), "c:/", "*.exe *.com");
#else
                    QString file = QFileDialog::getOpenFileName(this, tr("Choose a Program."));
#endif
                    if (!sl.contains(file) && file != "") {
                        sl.insert(0, file);
                        S_S.set("joinstrings", sl);
                        joinmenu2.clear();
                        foreach(QString s,S_S.getStringList("joinstrings")) {
                            joinmenu2.addAction(s);
                        }
                        joinmenu2.addAction(tr(
                                                "Choose a Program to join this game."));
                    } else if (sl.contains(file) && file != "") {
                        sl.move(sl.indexOf(file), 0);
                        S_S.set("joinstrings", sl);
                    }
                    QFile f(file);
                    if (f.open(QFile::ReadOnly)) {
                        QTextStream ts(&f);
                        QString s = ts.readLine();
                        while (!s.startsWith("Exec=") && !ts.atEnd())
                            QString s = ts.readLine();
                        myDebug() << s;
                    }
                } else {
                    QStringList sl = S_S.getStringList("joinstrings");
                    sl.move(sl.indexOf(a->text()), 0);
                    S_S.set("joinstrings", sl);
                    singleton<netcoupler>().joingame(hostinfo, currentChannel, gamename);
                }
            }
        }
    }
}
void channelTab::hboxok() {
    int flag=49;
    foreach(userstruct u,singleton<netcoupler>().users.users) {
        if (u.nick == singleton<netcoupler>().nick)
            flag = u.flag;
    }
    singleton<netcoupler>().sendhostinfotoserverandhost(hbox->gamename, hbox->pwd, currentChannel, flag);
}
void channelTab::getjoinmenu() {
    joinmenu2.clear();
    if (!S_S.getStringList("joinstrings").isEmpty()) {
        foreach(QString s,S_S.getStringList("joinstrings"))
            joinmenu2.addAction(s);
    }
    joinmenu2.addAction(tr("Choose a Program to join this game."));
}
void channelTab::openHostBox() {
    hbox = new hostbox(currentChannel);
    hbox->show();
    connect(hbox, SIGNAL(sigok()),this, SLOT(hboxok()));
}
void channelTab::minimize() {
    if (!hiddenchannelwindowshelper.contains(this))
        hiddenchannelwindowshelper.push_back(this);
    hide();
}
void channelTab::changealpha(int i) {
    S_S.set("channeltransparency", i);
    if (i == 100)
        setWindowOpacity(1);
    else
        setWindowOpacity((double) i / 100);
}
void channelTab::showbuttons() {
}
void channelTab::mysetwindowtitle() {
    /*setWindowTitle(QString()
                 +windowtitlechannel+" "
                 +windowtitletime   +" "
                 +tr("Users online")+" "
                 +windowtitleaway.simplified()
    );*/
}
void channelTab::setupWindowTitleOnJoin(QString channel,int amountOfUsers){
    if(channel.toLower()!=currentChannel.toLower())
        return;
    windowtitlechannel=currentChannel;
    windowtitletime=QString::number(amountOfUsers);
    mysetwindowtitle();
    expandchannels();
}
void channelTab::setupWindowTitleOnChangeOfUserAmount(QString channel,int amountOfUsers){
    if(channel.toLower()!=currentChannel.toLower())
        return;
    windowtitletime=QString::number(amountOfUsers);
    mysetwindowtitle();
}
void channelTab::showInformationAboutClan(QString clan){
    clan=clan.toLower();
}
void channelTab::lockUI(){
    /* we're going to completely detach the dock's title bar using setTitleBarWidget to save the */
    /* space it occupies (setting NoDockWidgetFeatuers doesn't free that space), so now we save  */
    /* the dock's original title bar widgets to restore later (in unlockUI)                      */
    hostsDockTitleBar=ui->hostsDock->titleBarWidget();
    usersDockTitleBar=ui->usersDock->titleBarWidget();
    chatDockTitleBar=ui->chatDock->titleBarWidget();

    /* setting the dock behave like a plain splitter (no docking/undocking features enabled)     */
    ui->hostsDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    ui->usersDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    ui->chatDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    /* a fix for when the UI gets unlocked and then locked again, the minimumHeight increases by */
    /* the dock's title bar. I set the minimumHeight back to the height of the chat input box    */
    ui->chatDock->setMinimumHeight(27);

    /* setting an empty QWidget as the dock's title bar to save the space                        */
    ui->hostsDock->setTitleBarWidget(new QWidget(this));
    ui->usersDock->setTitleBarWidget(new QWidget(this));
    ui->chatDock->setTitleBarWidget(new QWidget(this));

    /* locking the toolbar                                                                       */
    ui->toolBar->setMovable(false);
}
void channelTab::unlockUI(){
    /* restoring the docks' original title bars                                                  */
    ui->hostsDock->setTitleBarWidget(hostsDockTitleBar);
    ui->usersDock->setTitleBarWidget(usersDockTitleBar);
    ui->chatDock->setTitleBarWidget(chatDockTitleBar);

    /* enabling the docking/undocking features                                                   */
    ui->hostsDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    ui->usersDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    ui->chatDock->setFeatures(QDockWidget::AllDockWidgetFeatures);

    /* unlocking the toolbar                                                                     */
    ui->toolBar->setMovable(true);
}
void channelTab::monopolizeMenu(QMenu *channelMenu){
    this->channelMenu=channelMenu;
    ui->menuBar->addActions(this->channelMenu->actions());
    ui->menuBar->show();
}
void channelTab::dropMenu(){
    ui->menuBar->clear();
    ui->menuBar->hide();
}
QMenu* channelTab::getMenu(){
    if(0==channelMenu){
        this->channelMenu=new QMenu();
        Ui_ChannelMenu *uiChannelMenu=new Ui_ChannelMenu();
        uiChannelMenu->setupUi(channelMenu);
    }
    return channelMenu;
}
void channelTab::on_actionDockUndock_triggered(bool checked){
    if(checked)
        emit askTabDocking(this);
    else
        emit askTabUndocking(this);
}
void channelTab::setDocked(bool state){
    this->docked=state;
}
void channelTab::setHosts(QList<hoststruct> *hosts){
    this->hosts->clear();
    this->hosts->append(*hosts);
    this->hostModel->hostsChanged();
}
channelTab::~channelTab()
{
    singleton<netcoupler>().partchannel(currentChannel);
    QString s = currentChannel;
    emit sigclosed();
    disconnect();
    hiddenchannelwindowshelper.removeAll(this);
}
