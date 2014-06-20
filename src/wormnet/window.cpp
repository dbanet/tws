#include "window.h"
#include "chatwindow.h"
#include "settings.h"
#include "mainwindow.h"
#include "chathandler.h"
#include "hostbox.h"
#include "settingswindow.h"
#include "buttonlayout.h"
#ifdef PHONON
#include "sound_handler.h"
#endif
#include "global_functions.h"
#include "clantowebpagemapper.h"
#include "leagueserverhandler.h"
#include "usermessage.h"
#include "balloon_handler.h"
#include "emoticonsdialog.h"

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

QList<chatwindow*> window::chatWindowsList;
QStringList window::chatWindowsStringList;
QList< ::window*> window::hiddenChannelWindowsHelper;
extern QStringList querylist;
window::window(QString s, int i) :
    currentChannel(s), chatIcon("snppictures/Chat_Icon.png") {

    buttons = new buttonlayout(this);
    buttons->fillleaguemenus();
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("channelwindow");
    if (i == 1)
        ui1.setupUi(this);
    if (i == 2)
        ui2.setupUi(this);
    if (i == 3)
        ui3.setupUi(this);
    whichUIIsOn = i;    
    connect(buttons, SIGNAL(pbhostclicked()),this, SLOT(openHostBox()));
    connect(buttons, SIGNAL(pbminimizedclicked()),this, SLOT(minimize()));
    connect(buttons, SIGNAL(sigchangealpha(int)),this, SLOT(changeAlpha(int)));
    connect(buttons, SIGNAL(sigshowme()),this, SLOT(showButtons()));
    connect(buttons, SIGNAL(sigChangeLeagueState()),this, SIGNAL(sigChangeLeagueState()));

    ui.getchilds(this);    

    ui.users->setAlternatingRowColors(1);
    ui.hosts->setAlternatingRowColors(1);
    ui.buttonlayout->addWidget(buttons);    
    ui.users->installEventFilter(this);
    connect(&singleton<netcoupler>().users, SIGNAL(sigselectitem(const QModelIndex&,const QWidget*)),this, SLOT(setSelection(const QModelIndex&,const QWidget*)));
    chat = new chatHandler(this, ui.chat, currentChannel);
    ui.msg->installEventFilter(chat);
    connect(chat, SIGNAL(sigOpenChatWindow(const QString&)),this, SLOT(openChatWindow(const QString&)));
    connect(ui.send, SIGNAL(clicked()),ui.msg, SIGNAL(returnPressed()));
    ui.users->setModel(&singleton<netcoupler>().users);
    connect(ui.users->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection&,const QItemSelection&)),this, SLOT(userSelectionChanged(const QItemSelection&,const QItemSelection&)));
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
    connect(ui.users->header(),SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),&singleton<netcoupler>().users,SLOT(sortslot(int, Qt::SortOrder)));

    ui.hosts->setModel(&singleton<netcoupler>().hosts);
    ui.hosts->setEnabled(1);
    ui.hosts->setSortingEnabled(1);
    ui.hosts->header()->swapSections(1, 3);
    ui.hosts->header()->swapSections(0, 2);    
    ui.hosts->setColumnWidth(0, 190);
    ui.hosts->setColumnWidth(1, 120);
    ui.hosts->header()->setSortIndicatorShown(0);

    joinMenu2.setTitle(tr("Join"));
    hostMenu.addAction(tr("Host a game in ") + currentChannel);    
    joinMenu.addMenu(&joinMenu2);
    userMenu.addAction(tr("Add this user to Buddylist."));
    userMenu.addAction(tr("Add this user to Ignorelist."));
    userMenu.addSeparator();
    userMenu.addAction(tr("Show info about this user."))->setIcon(chatIcon);
    customListMenu.addAction(tr("Remove this user from the list."));
    customListMenu.addAction(tr("Show info about this user."))->setIcon(chatIcon);

    connect(ui.users, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(userItemDoubleClicked(const QModelIndex&)));
    connect(ui.users, SIGNAL(pressed(const QModelIndex&)),this, SLOT(userItemPressed(const QModelIndex&)));
    connect(ui.hosts, SIGNAL(pressed(const QModelIndex&)),this, SLOT(hostItemPressed(const QModelIndex&)));
    connect(ui.hosts, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(hostItemDoubleClicked(const QModelIndex&)));
    connect(ui.pbsmiley,SIGNAL(clicked()),this,SLOT(pbEmotClicked()));
    connect(&singleton<netcoupler>(),SIGNAL(sigJoinedChannel(QString,int)),this,SLOT(setupWindowTitleOnJoin(QString,int)));
    connect(&singleton<netcoupler>(),SIGNAL(sigUpdatedAmountOfUsers(QString,int)),this,SLOT(setupWindowTitleOnChangeOfUserAmount(QString,int)));
    connect(ui.msg, SIGNAL(returnPressed()),this, SLOT(sendMessage()));        

    ui.msg->setFocus(Qt::MouseFocusReason);

    QVariantList windowstates = S_S.getlist(currentChannel + QString::number(whichUIIsOn));
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

    windowTitleChannelName =  currentChannel;
}
void window::pbEmotClicked(){
    emoticonsdialog *dia = new emoticonsdialog;
    dia->move(QCursor::pos()-QPoint(dia->width(),dia->height()));
    dia->show();
    dia->raise();
    dia->setFocus();
    connect(dia,SIGNAL(sigemotchoosed(QString)),this,SLOT(insertEmot(QString)));
}
void window::insertEmot(QString s){
    QString ss=ui.msg->text();
    if(ss.endsWith(" ") || s.isEmpty())
        ui.msg->insert(s + " ");
    else
        ui.msg->insert(" " + s + " ");
    ui.msg->setFocus ();
}
void window::expandChannels() { //expand on startup
    ui.hosts->setExpanded(singleton<netcoupler>().hosts.indexbychannelname(currentChannel), 1);
    ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(currentChannel), 1);
    ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Querys")), 1);
    if (S_S.getbool("cbopenbuddylistonstartup"))
        ui.users->setExpanded(singleton<netcoupler>().users.indexbychannelname(usermodel::tr("Buddylist")), 1);
    if (ui.users->isExpanded(singleton<netcoupler>().users.index(singleton<netcoupler>().users.classes.indexOf(
                                                                     currentChannel), 0)) && ui.hosts->isExpanded(
                singleton<netcoupler>().hosts.index(singleton<netcoupler>().hosts.classes.indexOf( currentChannel),
                                                    0))) {
        //disconnect(&singleton<netcoupler>(), SIGNAL(siggotchanellist(QStringList)),this, SLOT(expandchannels(QStringList)));
    } else
        QTimer::singleShot(500, this, SLOT(expandChannels()));
}
void window::setSelection(const QModelIndex &index, const QWidget *w) {
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
void window::userSelectionChanged(const QItemSelection &selected,
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
                    openChatWindow(s);
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
void window::getUserMessage(usermessage u){
    QString user=u.user();
    if(u.has_type(e_GARBAGE))
        return chat->append(u);
    if(u.has_type(e_NOTICE)){
        singleton<balloonHandler>().alert(u.user(), this);
        chat->append(u);
        return;
    }
    if (u.receiver().toLower()!=currentChannel.toLower() && !containsCI(chatWindowsStringList, u.user())) {
        if (!singleton<netcoupler>().ignoreListContains(user) && !singleton<netcoupler>().buddyListContains(user)) {
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
void window::gotdebugmsg(const QString &msg) {
    chat->appenddebug(msg.simplified());
}
void window::sendMessage() {
    QString s = ui.msg->text();
    if (s.isEmpty())
        return;
    usermessage u=usermessage::create(s, singleton<netcoupler>().nick, currentChannel);
    chat->append(u);
    singleton<netcoupler>().sendUserMessage(u);
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
    S_S.set(currentChannel + QString::number(whichUIIsOn), windowstates);
}
void window::userItemPressed(const QModelIndex &index) {
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
        menu.addAction(tr("Show info about this user."))->setIcon(chatIcon);
        a = menu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this Query."))
                querylist.removeAll(user);
            else if (a->text() == tr("Show info about this user."))
                getUserInfo(user);            
        }
    } else if (singleton<netcoupler>().users.classes[index.internalId()] != usermodel::tr("Buddylist")
               && singleton<netcoupler>().users.classes[index.internalId()] != usermodel::tr("Ignorelist")) {
        QMenu menu;
        if (containsCI(S_S.buddylist, user)) {
            menu.addAction(tr("Remove this user from Buddylist."));
            menu.addSeparator();
            menu.addAction(tr("Show info about this user."))->setIcon(
                        chatIcon);
            a = menu.exec(QCursor::pos());
        } else if (containsCI(S_S.ignorelist, user)) {
            menu.addAction(tr("Remove this user from Ignorelist."));
            menu.addSeparator();
            menu.addAction(tr("Show info about this user."))->setIcon(chatIcon);
            a = menu.exec(QCursor::pos());
        } else
            a = userMenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Add this user to Buddylist.")) {
                singleton<netcoupler>().users.addbuddy(user);
            } else if (a->text() == tr("Add this user to Ignorelist.")) {
                singleton<netcoupler>().users.addignore(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getUserInfo(user);
            } else if (a->text() == tr("Remove this user from Buddylist.")) {
                singleton<netcoupler>().users.deletebuddy(user);
            } else if (a->text() == tr("Remove this user from Ignorelist.")) {
                singleton<netcoupler>().users.deleteignore(user);
            }
        } else
            return;
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Buddylist")) {
        QAction *a = customListMenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deletebuddy(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getUserInfo(user);
            }
        }
    } else if (singleton<netcoupler>().users.classes[index.internalId()] == usermodel::tr("Ignorelist")) {
        a = customListMenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == tr("Remove this user from the list.")) {
                singleton<netcoupler>().users.deleteignore(user);
            } else if (a->text() == tr("Show info about this user.")) {
                getUserInfo(user);
            }
        }
    }
}
void window::userItemDoubleClicked(const QModelIndex &index) {

    if (index.internalId() != usermodel::e_Channel
            && singleton<netcoupler>().users.classes[index.internalId()]!= usermodel::tr("Ignorelist")) {
        QString s = singleton<netcoupler>().users.data(index.sibling(index.row(), 0), Qt::DisplayRole).value<QString> ();
        openChatWindow(s);
    } else if (index.internalId() == usermodel::e_Channel){
        QString s=singleton<netcoupler>().users.data(index.sibling(index.row(), 0)).value<QString> ();
        if(s==usermodel::tr("Buddylist") || s==usermodel::tr("Ignorelist") || s==usermodel::tr("Querys"))
            return;
        emit sigJoinChannel(s);
    }
}
void window::hostItemDoubleClicked(const QModelIndex &index) {
    if (index.internalId() != 999) {
        QString hostinfo = singleton<netcoupler>().hosts.joininfo(index) + "&scheme="
                + singleton<netcoupler>().schemeMap[currentChannel];
        QString gamename = singleton<netcoupler>().hosts.gamename(index);
        singleton<netcoupler>().joinGame(hostinfo, currentChannel, gamename);
    } else if (index.internalId() == 999) {
        openHostBox();
    }
}
void window::getUserInfo(const QString &s) {
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
    foreach(QString chat,chatWindowsStringList) {
        if (compareCI(chat, s)) {
            foreach(chatwindow *c,chatWindowsList) {
                if (compareCI(chat, c->chatpartner)) {
                    c->getgamerwho(msg);
                    return;
                }
            }
        }
    }
    openChatWindow(s);
    chatWindowsList.last()->getgamerwho();
}
void window::openChatWindow(const QString &s) {
    Q_ASSERT(s!="");
    if (containsCI(chatWindowsStringList, s)){
        foreach(chatwindow *w,chatWindowsList){
            if(w->chatpartner==s && w->isHidden())
                w->show();
            if(w->chatpartner==s){
                w->raise();
                qApp->setActiveWindow(w);
            }
            qobjectwrapper<mainwindow>::ref().hiddenChatWindowsHelper.removeAll(w);
            querylist.removeAll(s);
        }
        return;
    }emit sigOpenChatWindow(s);
    QApplication::processEvents();    
}
void window::hostItemPressed(const QModelIndex &index) {
    if (QApplication::mouseButtons() == Qt::RightButton) {
        QString hostinfo = singleton<netcoupler>().hosts.joininfo(index) + "&scheme="
                + singleton<netcoupler>().schemeMap[currentChannel];
        QString gamename = singleton<netcoupler>().hosts.gamename(index);
        if (index.internalId() == 999) {
            openHostBox();
        } else {
            getJoinMenu();
            QAction *a = joinMenu.exec(QCursor::pos());
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
                        joinMenu2.clear();
                        foreach(QString s,S_S.getStringList("joinstrings")) {
                            joinMenu2.addAction(s);
                        }
                        joinMenu2.addAction(tr(
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
                    singleton<netcoupler>().joinGame(hostinfo, currentChannel, gamename);
                }
            }
        }
    }
}
void window::hostBoxOk() {
    int flag=49;
    foreach(userstruct u,singleton<netcoupler>().users.users) {
        if (u.nick == singleton<netcoupler>().nick)
            flag = u.flag;        
    }        
    singleton<netcoupler>().sendHostInfoToServerAndHost(hostBox->gamename, hostBox->pwd, currentChannel, flag);
}
void window::getJoinMenu() {
    joinMenu2.clear();
    if (!S_S.getStringList("joinstrings").isEmpty()) {
        foreach(QString s,S_S.getStringList("joinstrings"))
            joinMenu2.addAction(s);        
    }
    joinMenu2.addAction(tr("Choose a Program to join this game."));
}
void window::openHostBox() {
    hostBox = new hostbox(currentChannel);
    hostBox->show();
    connect(hostBox, SIGNAL(sigok()),this, SLOT(hostBoxOk()));
}
void window::minimize() {
    if (!hiddenChannelWindowsHelper.contains(this))
        hiddenChannelWindowsHelper.push_back(this);
    hide();
}
void window::changeAlpha(int i) {
    S_S.set("channeltransparency", i);
    if (i == 100)
        setWindowOpacity(1);
    else
        setWindowOpacity((double) i / 100);
}
void window::showButtons() {
    ui.buttonlayout->addWidget(buttons);
}
void window::updateWindowTitle() {
    setWindowTitle(QString()
                 +windowTitleChannelName+" "
                 +windowTitleUsersAmount   +" "
                 +tr("Users online")+" "
                 +windowAwayTitle.simplified()
    );
}
void window::setupWindowTitleOnJoin(QString channel,int amountOfUsers){
    if(channel.toLower()!=currentChannel.toLower())
        return;
    windowTitleChannelName=currentChannel;
    windowTitleUsersAmount=QString::number(amountOfUsers);
    updateWindowTitle();
    expandChannels();
}
void window::setupWindowTitleOnChangeOfUserAmount(QString channel,int amountOfUsers){
    if(channel.toLower()!=currentChannel.toLower())
        return;
    windowTitleUsersAmount=QString::number(amountOfUsers);
    updateWindowTitle();
}
window::~window() {
    ui.buttonlayout->removeWidget(buttons);
    buttons->setParent(0);       
    singleton<netcoupler>().partChannel(currentChannel);
    QString s = currentChannel;
    containsCI(singleton<netcoupler>().hosts.hostmap.keys(), s);
    singleton<netcoupler>().hosts.sethoststruct(QList<hoststruct> (), s);
    emit sigClosed();
    disconnect();
    hiddenChannelWindowsHelper.removeAll(this);
}
void window::showInformationAboutClan(QString clan){
    clan=clan.toLower();
}
