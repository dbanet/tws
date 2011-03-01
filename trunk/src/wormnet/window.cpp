#include"window.h"
#include"chatwindow.h"
#include"settings.h"
#include"mainwindow.h"
#include"chathandler.h"
#include"hostbox.h"
#include"settingswindow.h"
#include"buttonlayout.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"clantowebpagemapper.h"
#include"leagueserverhandler.h"
#include"usermessage.h"
#include"balloon_handler.h"

#include<QHBoxLayout>
#include<QHeaderView>
#include<QFileDialog>
#include<QProcess>
#include<QKeyEvent>
#include<QItemSelectionModel>
#include<QItemSelection>
#include<QMessageBox>
#include<QDate>
#include<QScrollBar>
#include<QDesktopServices>

QList<chatwindow*> window::chatwindows;
QStringList window::chatwindowstringlist;
QList< ::window*> window::hiddenchannelwindowshelper;
extern QStringList querylist;
window::window(QString s, int i) :
	currentchannel(s), chaticon("snppictures/Chat_Icon.png") {

    buttons = new buttonlayout(this);
    buttons->fillleaguemenu();
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("channelwindow");
    if (i == 1)
        ui1.setupUi(this);
    if (i == 2)
        ui2.setupUi(this);
    if (i == 3)
        ui3.setupUi(this);
    whichuiison = i;    
    connect(buttons, SIGNAL(pbhostclicked()),this, SLOT(openhbox()));
    connect(buttons, SIGNAL(pbminimizedclicked()),this, SLOT(minimize()));
    connect(buttons, SIGNAL(sigchangealpha(int)),this, SLOT(changealpha(int)));
    connect(buttons, SIGNAL(sigshowme()),this, SLOT(showbuttons()));
    ui.getchilds(this);    

    ui.users->setAlternatingRowColors(1);
    ui.hosts->setAlternatingRowColors(1);
    ui.buttonlayout->addWidget(buttons);    
    ui.users->installEventFilter(this);
    connect(&singleton<netcoupler>().users, SIGNAL(sigselectitem(const QModelIndex&,const QWidget*)),this, SLOT(setselection(const QModelIndex&,const QWidget*)));    
    chat = new chathandler(this, ui.chat, currentchannel);
    ui.msg->installEventFilter(chat);
    connect(chat, SIGNAL(sigopenchatwindow(const QString&)),this, SLOT(openchatwindow(const QString&)));
    connect(ui.send, SIGNAL(clicked()),ui.msg, SIGNAL(returnPressed()));
    ui.users->setModel(&singleton<netcoupler>().users);
    connect(ui.users->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection&,const QItemSelection&)),this, SLOT(userselectionchanged(const QItemSelection&,const QItemSelection&)));
    ui.users->setEnabled(1);
    ui.users->header()->swapSections(0, 1);
    ui.users->header()->swapSections(1, 2);
    ui.users->setColumnWidth(0, 180);    
    ui.users->setColumnWidth(3, 48);
    ui.users->setColumnWidth(4, 120);    

    ui.users->setSortingEnabled(1);
    ui.users->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(1, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(2, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(3, Qt::AscendingOrder);
    ui.users->header()->setSortIndicatorShown(1);
    connect(ui.users->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),&singleton<netcoupler>().users, SLOT(sortslot(int, Qt::SortOrder)));

    ui.hosts->setModel(&singleton<netcoupler>().hosts);
    ui.hosts->setEnabled(1);
    ui.hosts->setSortingEnabled(1);
    ui.hosts->header()->swapSections(1, 3);
    ui.hosts->header()->swapSections(0, 2);    
    ui.hosts->setColumnWidth(0, 190);
    ui.hosts->setColumnWidth(1, 120);
    ui.hosts->header()->setSortIndicatorShown(0);

    joinmenu2.setTitle(tr("Join"));
    hostmenu.addAction(tr("Host a game in ") + currentchannel);    
    joinmenu.addMenu(&joinmenu2);
    usermenu.addAction(tr("Add this user to Buddylist."));
    usermenu.addAction(tr("Add this user to Ignorelist."));
    usermenu.addSeparator();
    usermenu.addAction(tr("Show info about this user."))->setIcon(chaticon);
    costumlistmenu.addAction(tr("Remove this user from the list."));
    costumlistmenu.addAction(tr("Show info about this user."))->setIcon(
            chaticon);

    connect(ui.users, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(useritemdblclicked(const QModelIndex&)));
    connect(ui.users, SIGNAL(pressed(const QModelIndex&)),this, SLOT(useritempressed(const QModelIndex&)));
    connect(ui.hosts, SIGNAL(pressed(const QModelIndex&)),this, SLOT(hostitempressed(const QModelIndex&)));
    connect(ui.hosts, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(hostitemdblclicked(const QModelIndex&)));

    connect(ui.msg, SIGNAL(returnPressed()),this, SLOT(sendmsg()));        

    singleton<netcoupler>().refreshwho();

    connect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(QStringList)),this, SLOT(expandchannels(QStringList)));
    connect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(QStringList)),this, SLOT(getuserscount(QStringList)));
    ui.msg->setFocus(Qt::MouseFocusReason);

    QVariantList windowstates = S_S.getlist(currentchannel + QString::number(whichuiison));
    if (!windowstates.isEmpty()) {
        if (!windowstates.isEmpty())
            restoreGeometry(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.splitter1->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.splitter2->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.users->header()->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.hosts->header()->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.hosts->header()->resizeSection(3,windowstates.takeFirst().toInt());
        if (!windowstates.isEmpty())
            ui.hosts->header()->resizeSection(4,windowstates.takeFirst().toInt());
        if (!windowstates.isEmpty())
            ui.users->header()->resizeSection(3,windowstates.takeFirst().toInt());
        if (!windowstates.isEmpty())
            ui.users->header()->resizeSection(4,windowstates.takeFirst().toInt());
    }
    ui.hosts->setColumnWidth(3, 16);
    ui.hosts->setColumnWidth(2, 22);
    ui.users->setColumnWidth(1, 22);
    ui.users->setColumnWidth(2, 48);
    ui.users->header()->setResizeMode(1, QHeaderView::Fixed);
    ui.users->header()->setResizeMode(2, QHeaderView::Fixed);
    ui.hosts->header()->setResizeMode(2, QHeaderView::Fixed);
    ui.hosts->header()->setResizeMode(3, QHeaderView::Fixed);

    windowtitlechannel =  currentchannel;

}
void window::expandchannels() { //expand on startup
    expandchannels(QStringList());
}
void window::expandchannels(QStringList) { //expand on startup
    ui.hosts->setExpanded(singleton<netcoupler>().hosts.indexbychannelname(currentchannel), 1);
    ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(currentchannel), 1);
    ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Querys")), 1);
    if (S_S.getbool("cbopenbuddylistonstartup"))
        ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Buddylist")), 1);
    if (ui.users->isExpanded(singleton<netcoupler>().users.index(singleton<netcoupler>().users.classes.indexOf(
            currentchannel), 0)) && ui.hosts->isExpanded(
                    singleton<netcoupler>().hosts.index(singleton<netcoupler>().hosts.classes.indexOf( currentchannel),
                                                        0))) {
        disconnect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(QStringList)),this, SLOT(expandchannels(QStringList)));
    } else
        QTimer::singleShot(500, this, SLOT(expandchannels()));
}
void window::setselection(const QModelIndex &index, const QWidget *w) {
    if (w == this) {
        if (!index.isValid())
            ui.users->clearSelection();
        else {
            ui.users->clearSelection();
            ui.users->selectionModel()->select(index, QItemSelectionModel::Rows
                                               | QItemSelectionModel::Select);
        }
    }
}
void window::userselectionchanged(const QItemSelection &selected,
                                  const QItemSelection&) {

    if (!selected.indexes().isEmpty())
        singleton<netcoupler>().users.selectionchanged(selected.indexes().first(), this);
}
bool window::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui.users ) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Return) {
                QModelIndex index=ui.users->currentIndex();
                if (index.internalId() == usermodel::e_Channel)
                    return false;
                QString s = singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ();
                if (s != "")
                    openchatwindow(s);
                return true;
            } else if (keyEvent->key() == Qt::Key_Space) {
                QString s = singleton<netcoupler>().users.data(ui.users->currentIndex().sibling(
                        ui.users->currentIndex().row(), 0)).value<QString> ();
                ui.msg->insert(s + " ");
                ui.msg->setFocus(Qt::MouseFocusReason);
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
void window::getusermessage(usermessage u){
    QString user=u.user();
    if(u.has_type(e_GARBAGE))
        return chat->append(u);
    if (u.receiver() != currentchannel && !containsCI(chatwindowstringlist, u.user())) {
        if (!singleton<netcoupler>().ignorelistcontains(user) && !singleton<netcoupler>().buddylistcontains(user)) {
            if (!containsCI(querylist, user))
                querylist << user;
            u.add_type(e_CHATMSGTOCHANNEL);
            chat->append(u);
            if (S_S.getbool("cbalertfromnormal")) {
                singleton<balloon_handler>().alert(u.user(), this);
            }
            singleton<sound_handler>().play_normalmsgsound(user);
        }
    } else if (compareCI(u.receiver(), currentchannel)){
        if(S_S.getbool("cbignorysappearinchannel"))
            chat->append(u);
        else if (!containsCI(S_S.ignorelist, user))
            chat->append(u);
    }
}
void window::gotdebugmsg(const QString &msg) {
    chat->appenddebug(msg.simplified());
}
void window::sendmsg() {
    QString s = ui.msg->text();
    if (s.isEmpty())
        return;
    usermessage u=usermessage::create(s, singleton<netcoupler>().nick, currentchannel);
    chat->append(u);
    singleton<netcoupler>().sendusermessage(u);
    ui.msg->clear();
    chat->moveSliderToMaximum();
}
void window::closeEvent(QCloseEvent * /*event*/) {    
    QVariantList windowstates;
    windowstates << saveGeometry();
    windowstates << ui.splitter1->saveState();
    windowstates << ui.splitter2->saveState();
    windowstates << ui.users->header()->saveState();
    windowstates << ui.hosts->header()->saveState();
    windowstates << ui.hosts->header()->sectionSize(3);
    windowstates << ui.hosts->header()->sectionSize(4);
    windowstates << ui.users->header()->sectionSize(3);
    windowstates << ui.users->header()->sectionSize(4);
    S_S.set(currentchannel + QString::number(whichuiison), windowstates);
}
void window::useritempressed(const QModelIndex &index) {
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
        menu.addAction(tr("Show info about this user."))->setIcon(
                chaticon);
        a = menu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this Query.")) {
                querylist.removeAll(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            }
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
        QAction *a = costumlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deletebuddy(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Ignorelist")) {
        a = costumlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deleteignore(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    }
}
void window::useritemdblclicked(const QModelIndex &index) {

    if (index.internalId() != usermodel::e_Channel
        && singleton<netcoupler>().users.classes[index.internalId()]!= usermodel::tr("Ignorelist")) {
        QString s = singleton<netcoupler>().users.data(index.sibling(index.row(), 0),
                                                       Qt::DisplayRole).value<QString> ();
        openchatwindow(s);
    } else if (index.internalId() == usermodel::e_Channel){
        QString s=singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ();
        if(s==usermodel::tr("Buddylist") || s==usermodel::tr("Ignorelist") || s==usermodel::tr("Querys"))
            return;
        emit sigjoinchannel(s);
    }
}
void window::hostitemdblclicked(const QModelIndex &index) {
    if (index.internalId() != 999) {
        QString hostinfo = " \"" + singleton<netcoupler>().hosts.joininfo(index) + "&scheme="
                           + singleton<netcoupler>().schememap[currentchannel] + "\"";
        QString gamename = singleton<netcoupler>().hosts.gamename(index);
        singleton<netcoupler>().joingame(hostinfo, currentchannel, gamename);
    } else if (index.internalId() == 999) {
        openhbox();
    }
}
void window::getuserinfo(const QString &s) {
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
void window::openchatwindow(const QString &s) {
    Q_ASSERT(s!="");
    if (containsCI(chatwindowstringlist, s)){
        foreach(chatwindow *w,chatwindows){
            if(w->chatpartner==s && w->isHidden())
                w->show();
            if(w->chatpartner==s){
                w->raise();
                qApp->setActiveWindow(w);
            }
            qobjectwrapper<mainwindow>::ref().hiddenchatwindowshelper.removeAll(w);
            querylist.removeAll(s);
        }
        return;
    }emit sigopenchatwindow(s);
    QApplication::processEvents();
    /*chatwindowstringlist << s;
	 chatwindows.push_back(new chatwindow(net, s));
	 connect(chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
	 chatwindows.last()->show();*/
}
void window::hostitempressed(const QModelIndex &index) {
    if (QApplication::mouseButtons() == Qt::RightButton) {
        QString hostinfo = " \"" + singleton<netcoupler>().hosts.joininfo(index) + "&scheme="
                           + singleton<netcoupler>().schememap[currentchannel] + "\"";
        QString gamename = singleton<netcoupler>().hosts.gamename(index);
        if (index.internalId() == 999) {
            openhbox();
        } else {
            getjoinmenu();
            QAction *a = joinmenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Choose a Program to join this game.")) {
                    QStringList sl = S_S.getstringlist("joinstrings");
#ifdef Q_WS_S60
                    QString file;
                    return;
#endif
#ifdef Q_WS_MAC
                    QString file = QFileDialog::getOpenFileName(this, tr(
                            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_WIN
                    QString file = QFileDialog::getOpenFileName(this, tr("Choose a Program."), "c:/", "*.exe *.com");
#endif
#ifdef Q_WS_X11
                    QString file = QFileDialog::getOpenFileName(this, tr(
                            "Choose a Desktopicon."), "/home", "*.desktop");
#endif
                    if (!sl.contains(file) && file != "") {
                        sl.insert(0, file);
                        S_S.set("joinstrings", sl);
                        joinmenu2.clear();
                        foreach(QString s,S_S.getstringlist("joinstrings")) {
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
                    QStringList sl = S_S.getstringlist("joinstrings");
                    sl.move(sl.indexOf(a->text()), 0);
                    S_S.set("joinstrings", sl);
                    singleton<netcoupler>().joingame(hostinfo, currentchannel, gamename);
                }
            }
        }
    }
}
void window::hboxok() {
    int flag=49;
    foreach(userstruct u,singleton<netcoupler>().users.users) {
        if (u.nick == singleton<netcoupler>().nick)
            flag = u.flag;        
    }        
    singleton<netcoupler>().sendhostinfotoserverandhost(hbox->gamename, hbox->pwd, currentchannel, flag);
    hbox->deleteLater();
}
void window::getjoinmenu() {
    joinmenu2.clear();
    if (!S_S.getstringlist("joinstrings").isEmpty()) {
        foreach(QString s,S_S.getstringlist("joinstrings"))
            joinmenu2.addAction(s);        
    }
    joinmenu2.addAction(tr("Choose a Program to join this game."));
}
void window::openhbox() {
    hbox = new hostbox(currentchannel);
    hbox->show();
    connect(hbox, SIGNAL(sigok()),this, SLOT(hboxok()));
}
void window::minimize() {
    if (!hiddenchannelwindowshelper.contains(this))
        hiddenchannelwindowshelper.push_back(this);
    hide();
}
void window::changealpha(int i) {
    S_S.set("channeltransparency", i);
    if (i == 100)
        setWindowOpacity(1);
    else
        setWindowOpacity((double) i / 100);
}
void window::showbuttons() {
    ui.buttonlayout->addWidget(buttons);
}
void window::mysetwindowtitle() {
    setWindowTitle(windowtitlechannel + " " + windowtitletime + " " + tr(
            "Users online") + windowtitleaway.simplified());
}
void window::getuserscount(QStringList sl) {
    QString s;
    foreach(QString str,sl) {
        if (str.contains( currentchannel))
            s = str;
    }
    sl = s.split(" ", QString::SkipEmptyParts);
    if (sl.size() > 1)
        windowtitletime = sl[1];
    mysetwindowtitle();
}
window::~window() {
    ui.buttonlayout->removeWidget(buttons);
    buttons->setParent(0);       
    singleton<netcoupler>().partchannel(currentchannel);
    QString s = currentchannel;
    containsCI(singleton<netcoupler>().hosts.hostmap.keys(), s);
    singleton<netcoupler>().hosts.sethoststruct(QList<hoststruct> (), s);
    emit sigwindowclosed(currentchannel);
    disconnect();
    hiddenchannelwindowshelper.removeAll(this);
}
void window::showInformationAboutClan(QString clan){
    clan=clan.toLower();
}
