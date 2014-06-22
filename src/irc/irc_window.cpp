#include "irc_window.h"
#include"src/irc/irc_chatwindow.h"
#include<QHBoxLayout>
#include<QHeaderView>
#include<QFileDialog>
#include<QProcess>
#include<QKeyEvent>
#include<QItemSelectionModel>
#include<QItemSelection>
#include"src/wormnet/snpsettings.h"
#include"src/wormnet/chathandler.h"
#include"src/wormnet/settingswindow.h"
#include"src/wormnet/buttonlayout.h"
#include"src/wormnet/sound_handler.h"
#include"src/wormnet/global_functions.h"
#include"src/wormnet/usermodel.h"
#include"src/wormnet/window.h"
#include"src/wormnet/balloon_handler.h"
#include"irc_ircnet.h"
#include"src/wormnet/ctcphandler.h"
#include<QMessageBox>
#include<QDate>
#include<QScrollBar>
#include<QDesktopServices>
QList<irc_chatwindow*> irc_window::chatwindows;
QStringList irc_window::chatwindowstringlist;
QList< ::irc_window*> irc_window::hiddenchannelwindowshelper;
extern QStringList querylist;
irc_window::irc_window(irc_netcoupler *n, QString s, int i) :
        currentchannel(s), net(n), chaticon("snppictures/Chat_Icon.png") {
    buttons = new buttonlayout(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setObjectName("channelwindow");
    ui.setupUi(this);
    connect(buttons, SIGNAL(pbminimizedclicked()),this, SLOT(minimize()));
    connect(buttons, SIGNAL(sigchangealpha(int)),this, SLOT(changealpha(int)));

    ui.buttonlayout->addWidget(buttons);
    ui.msg->installEventFilter(this);
    ui.users->installEventFilter(this);
    connect(&net->users, SIGNAL(sigselectitem(const QModelIndex&,const QWidget*)),this, SLOT(setselection(const QModelIndex&,const QWidget*)));
    chat = new chathandler(this, ui.chat, currentchannel);
    connect(chat, SIGNAL(sigopenchatwindow(const QString&)),this, SLOT(openchatwindow(const QString&)));
    connect(ui.send, SIGNAL(clicked()),ui.msg, SIGNAL(returnPressed()));
    ui.users->setModel(&net->users);
    connect(ui.users->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection&,const QItemSelection&)),this, SLOT(userselectionchanged(const QItemSelection&,const QItemSelection&)));
    ui.users->setEnabled(1);
    ui.users->header()->swapSections(0, 1);
    ui.users->header()->swapSections(1, 2);
    ui.users->setColumnWidth(0, 180);
    ui.users->setColumnWidth(1, 22);
    ui.users->setColumnWidth(2, 48);
    ui.users->setColumnWidth(3, 48);
    ui.users->setColumnWidth(4, 120);
    ui.users->header()->setResizeMode(1, QHeaderView::Fixed);
    ui.users->header()->setResizeMode(2, QHeaderView::Fixed);

    ui.users->setSortingEnabled(1);
    ui.users->header()->setSortIndicator(0, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(1, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(2, Qt::AscendingOrder);
    ui.users->header()->setSortIndicator(3, Qt::AscendingOrder);
    ui.users->header()->setSortIndicatorShown(1);
    connect(ui.users->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),&net->users, SLOT(sortslot(int, Qt::SortOrder)));

    usermenu.addAction(window::tr("Add this user to Buddylist."));
    usermenu.addAction(window::tr("Add this user to Ignorelist."));
    usermenu.addSeparator();
    usermenu.addAction(window::tr("Show info about this user."))->setIcon(chaticon);
    costumlistmenu.addAction(window::tr("Remove this user from the list."));
    costumlistmenu.addAction(window::tr("Show info about this user."))->setIcon(
            chaticon);

    connect(ui.users, SIGNAL(doubleClicked ( const QModelIndex &)),this, SLOT(useritemdblclicked(const QModelIndex&)));
    connect(ui.users, SIGNAL(pressed(const QModelIndex&)),this, SLOT(useritempressed(const QModelIndex&)));

    connect(ui.msg, SIGNAL(returnPressed()),this, SLOT(sendmsg()));
    connect(net, SIGNAL(siggotmsg(const QString&,const QString&,const QString&)),this, SLOT(gotmsg(const QString&,const QString&,const QString&)));
    connect(net, SIGNAL(siggotnotice(const QString&,const QString&,const QString&)),this, SLOT(gotnotice(const QString&,const QString&,const QString&)));
    connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));

    expandchannels(QStringList());
    ui.msg->setFocus(Qt::MouseFocusReason);

    QVariantList windowstates = singleton<snpsettings>().map[this->currentchannel].toList();
    if (!windowstates.isEmpty()) {
        if (!windowstates.isEmpty())
            this->restoreGeometry(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.splitter1->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.splitter2->restoreState(windowstates.takeFirst().toByteArray());
        if (!windowstates.isEmpty())
            ui.users->header()->restoreState(
                    windowstates.takeFirst().toByteArray());
    }
    usesettingswindow();
    this->windowtitlechannel = this->currentchannel;

    setWindowTitle("irc.gamesurge.net");

}

void irc_window::expandchannels(QStringList) { //expand on startup
    ui.users->setExpanded(net->users.indexbychannelname(net->irc->ircChannel),1);
    ui.users->setExpanded(net->users.indexbychannelname(currentchannel), 1);
    ui.users->setExpanded(net->users.indexbychannelname(usermodel::tr("Querys")), 1);
    if (singleton<settingswindow>().from_map("cbopenbuddylistonstartup").value<bool> ())
        ui.users->setExpanded(net->users.indexbychannelname(usermodel::tr(
                "Buddylist")), 1);
}
void irc_window::setselection(const QModelIndex &index, const QWidget *w) {
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
void irc_window::userselectionchanged(const QItemSelection &selected,
                                      const QItemSelection&) {

    if (!selected.indexes().isEmpty())
        net->users.selectionchanged(selected.indexes().first(), this);
}
bool irc_window::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui.msg) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            if (keyEvent->key() == Qt::Key_Up)
                qApp->postEvent(ui.msg,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ControlModifier));
            else if (keyEvent->key() == Qt::Key_Down)
                qApp->postEvent(ui.msg,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ShiftModifier | Qt::ControlModifier));
        }
        return false;
    }
    if (obj == ui.users) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
            Q_ASSERT(keyEvent!=0);
            if (keyEvent->key() == Qt::Key_Return) {
                QString s = net->users.data(ui.users->currentIndex().sibling(
                        ui.users->currentIndex().row(), 0)).value<QString> ();
                if (s != "")
                    this->openchatwindow(s);
                return true;
            } else if (keyEvent->key() == Qt::Key_Space) {
                QString s = net->users.data(ui.users->currentIndex().sibling(
                        ui.users->currentIndex().row(), 0)).value<QString> ();
                ui.msg->insert(s + " ");
                ui.msg->setFocus(Qt::MouseFocusReason);
                return true;
            }
            if(keyEvent->key()==Qt::Key_F && keyEvent->modifiers()==Qt::ControlModifier)
            {
                //usersearchwidget *s=new usersearchwidget; //extendent search for users
                //s->show();                                //stays unfinisched
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
void irc_window::gotprvmsg(const QString &user, const QString &receiver,
                           const QString &msg){
    userstruct u=userstruct::whoami(user);
    if (net->getUsersInChannel(usermodel::tr("Ignorelist")).count(u))
        return;
    if (!containsCI(chatwindowstringlist, user)) {
        userstruct u=userstruct::whoami(user);
        if(net->getUsersInChannel(usermodel::tr("Buddylist")).count(u)){
            openchatwindow(user);
        }
        if (!containsCI(querylist, user))
            querylist << user;
        chat->append(user, receiver, msg);
        if (alertonprivmsg) {
            QApplication::alert(this);
            emit sigalert(this);
        }
        singleton<balloon_handler>().got_privmsg(user, msg);
        singleton<sound_handler>().play_normalmsgsound(user);                                        ;
        return;
    } else{
        foreach(irc_chatwindow *w,chatwindows) {
            if(w->chatpartner==user)
                w->gotmsg(user, receiver, msg);
            return;
        }
    }
}

void irc_window::gotmsg(const QString &user, const QString &receiver,
                        const QString &msg) {
    if(receiver==net->nick){
        gotprvmsg(user,receiver,msg);
        return;
    }

    bool ctcp_wants_it = 0;
    if (!msg.startsWith("\001ACTION", Qt::CaseInsensitive) && msg.startsWith(
            "\001")) {
        ctcp_wants_it = singleton<ctcphandler>().getctcp(user, msg);
    }
    if (ctcp_wants_it)
        return;
    usergarbagemap[user.toLower()]<< QDate::currentDate().toString("dd.MM") + " "
            + QTime::currentTime().toString("hh:mm") + " " + user
            + ">" + QString(msg).remove("\n").remove("\r");
    if (containsCI(singleton<snpsettings>().map["ignorelist"].value<QStringList> (), user)
        && acceptignorys) {
        chat->append(user, receiver, msg);
        return;
    }
    chat->append(user, receiver, msg);
}
void irc_window::gotnotice(const QString &user, const QString &receiver,
                           const QString &msg) {

    if (!containsCI(singleton<snpsettings>().map["ignorelist"].value<QStringList> (), user)) {
        if (containsCI(net->channellist, receiver)) { //notice from user to a channel
            if (compareCI(receiver, currentchannel))
                chat->appendnotice(user, receiver, msg); //only one channel will get the notice
        } else { //notice from user to yourself
            chat->appendnotice(user, receiver, msg);
        }
        if (alertonnotice) {
            QApplication::alert(this);
            emit sigalert(this);
        }
    }
}
void irc_window::gotdebugmsg(const QString &msg) {
    chat->appenddebug(msg.simplified());
}
void irc_window::gotgarbagejoin(const QString &user, const QString &msg) {
    chat->appendjoingarbage(user + "> " + msg);
}
void irc_window::gotgarbagepart(const QString &user, const QString &msg) {
    chat->appendpartgarbage(user + "> " + msg);
}
void irc_window::gotgarbagequit(const QString &user, const QString &msg) {
    chat->appendquitgarbage(user + "> " + msg);
}
void irc_window::sendmsg() {
    QString s = ui.msg->text();
    if (s != "") {
        if (s.startsWith(">!")) {
            net->sendrawcommand(QString("PRIVMSG ") + currentchannel + " :\001"
                                + s.remove(0, 2) + " \001");
            chat->append(net->nick, currentchannel, "\001" + s + "\001");
        } else if (s.startsWith("/")) {
            net->sendrawcommand(s.remove(0, 1));
            chat->append(net->nick, currentchannel, s);
        } else if (s.startsWith(">>>")) {
            sendnoticeaction();
            chat->append(net->nick, currentchannel, s);
        } else if (s.startsWith(">>")) {
            sendnotice();
            chat->append(net->nick, currentchannel, s);
        } else if (s.startsWith(">")) {
            net->sendrawcommand(QString("PRIVMSG ") + currentchannel
                                + " :\001ACTION " + s.remove(0, 1).remove("\n") + " \001");
            chat->append(net->nick, currentchannel, "\001ACTION " + s + " \001");
        } else {
            net->sendmessage(currentchannel, s);
            chat->append(net->nick, currentchannel, s);
        }
        ui.msg->clear();
        chat->moveSliderToMaximum();
    }
}
void irc_window::sendnotice() {
    const QString s = ui.msg->text().remove(0, 2);
    if (s != "") {
        net->sendnotice(currentchannel, s);
        gotnotice(net->nick, this->currentchannel, s);
        ui.msg->clear();
        chat->moveSliderToMaximum();
    }
}
void irc_window::sendnoticeaction() {
    const QString s = ui.msg->text().remove(0, 3).remove("\n");
    if (s != "") {
        net->sendrawcommand(QString("NOTICE ") + this->currentchannel
                            + " :\001ACTION " + s + " \001");
        gotnotice(net->nick, currentchannel, "<" + s + ">");
        ui.msg->clear();
        chat->moveSliderToMaximum();
    }
}
void irc_window::closeEvent(QCloseEvent * /*event*/) {
}
void irc_window::useritempressed(const QModelIndex &index) {
    if (!index.isValid())
        return;
    if (QApplication::mouseButtons() == Qt::LeftButton && index.column()==usermodel::e_Client){
        QString s=net->users.data(index.sibling(index.row(), usermodel::e_Client)).toString();
        if(isClickableLink(s)){
            s=s.trimmed();
            QUrl u1;
            u1.setUrl(s);
            QDesktopServices::openUrl(u1);
        }
        return;
    }
    if (QApplication::mouseButtons() != Qt::RightButton)
        return;
    QAction *a;
    QMenu menu;
    QString user = net->users.data(index.sibling(index.row(), 0),
                                   Qt::DisplayRole).value<QString> ();
    if (index.internalId() == usermodel::e_Channel) {
        if(net->users.data(index.sibling(index.row(), 0)).value<QString> ()==usermodel::tr("Querys")){
            menu.addAction(window::tr("Remove Querys"));
            a = menu.exec(QCursor::pos());
            if(a && a->text()==window::tr("Remove Querys")){
                querylist.clear();
                safequerylist();
            }
        }
    } else if (index.column() == usermodel::e_Clan) {
        QStringList sl = singleton<snpsettings>().map["dissallowedclannames"].value<QStringList>();
        if (sl.contains(net->users.getuserstructbyindex(index).nickfromclient,Qt::CaseInsensitive))
            menu.addAction(window::tr("Allow this clanname."));
        else
            menu.addAction(window::tr("Dissallow this clanname."));
        a = menu.exec(QCursor::pos());
        if (!a) return;
        if (a->text() == window::tr("Allow this clanname."))
            removeCI(sl,net->users.getuserstructbyindex(index).nickfromclient);
        else if(a->text() == window::tr("Dissallow this clanname."))
            sl<< net->users.getuserstructbyindex(index).nickfromclient;
        else
            showInformationAboutClan(net->users.getuserstructbyindex(index).nickfromclient);
        singleton<snpsettings>().map["dissallowedclannames"].setValue<QStringList> (sl);
        singleton<snpsettings>().safe();
    } else if (net->users.classes[index.internalId()] == usermodel::tr("Querys")) {
        QMenu menu;
        menu.addAction(window::tr("Remove this Query."));
        menu.addSeparator();
        menu.addAction(window::tr("Show info about this user."))->setIcon(
                chaticon);
        a = menu.exec(QCursor::pos());
        if (a) {
            if (a->text() == window::tr("Remove this Query.")) {
                querylist.removeAll(user);
            } else if (a->text() == window::tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    } else if (net->users.classes[index.internalId()] != usermodel::tr(
            "Buddylist") && net->users.classes[index.internalId()]
               != usermodel::tr("Ignorelist")) {
        QMenu menu;
        if (containsCI(singleton<snpsettings>().map["buddylist"].value<QStringList> (),
                       user)) {
            menu.addAction(window::tr("Remove this user from Buddylist."));
            menu.addSeparator();
            menu.addAction(window::tr("Show info about this user."))->setIcon(
                    chaticon);
            a = menu.exec(QCursor::pos());
        } else if (containsCI(singleton<snpsettings>().map["ignorelist"].value<
                              QStringList> (), user)) {
            menu.addAction(window::tr("Remove this user from Ignorelist."));
            menu.addSeparator();
            menu.addAction(window::tr("Show info about this user."))->setIcon(
                    chaticon);
            a = menu.exec(QCursor::pos());
        } else
            a = usermenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == window::tr("Add this user to Buddylist.")) {
                net->users.addbuddy(user);
            } else if (a->text() == window::tr("Add this user to Ignorelist.")) {
                net->users.addignore(user);
            } else if (a->text() == window::tr("Show info about this user.")) {
                getuserinfo(user);
            } else if (a->text() == window::tr(
                    "Remove this user from Buddylist.")) {
                net->users.deletebuddy(user);
            } else if (a->text() == window::tr(
                    "Remove this user from Ignorelist.")) {
                net->users.deleteignore(user);
            }
        } else
            return;
    } else if (net->users.classes[index.internalId()] == usermodel::tr("Buddylist")) {
        QAction *a = costumlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == window::tr("Remove this user from the list.")) {
                net->users.deletebuddy(user);
            } else if (a->text() == window::tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    } else if (net->users.classes[index.internalId()] == usermodel::tr("Ignorelist")) {
        a = costumlistmenu.exec(QCursor::pos());
        if (a) {
            if (a->text() == window::tr("Remove this user from the list.")) {
                net->users.deleteignore(user);
            } else if (a->text() == window::tr("Show info about this user.")) {
                getuserinfo(user);
            }
        }
    }
}
void irc_window::useritemdblclicked(const QModelIndex &index) {
    if (index.internalId() != usermodel::e_Channel
        && net->users.classes[index.internalId()]!= usermodel::tr("Ignorelist")) {
        QString s = net->users.data(index.sibling(index.row(), 0),
                                    Qt::DisplayRole).value<QString> ();
        openchatwindow(s);
    }
}
void irc_window::getuserinfo(const QString &s) {
    foreach(QString chat,chatwindowstringlist) {
        if (compareCI(chat, s)) {
            foreach(irc_chatwindow *c,chatwindows) {
                if (compareCI(chat, c->chatpartner)) {
                    c->getgamerwho();
                    return;
                }
            }
        }
    }
    openchatwindow(s);
    chatwindows.last()->getgamerwho();
}
void irc_window::openchatwindow(const QString &s) {
    Q_ASSERT(s!="");
    if (containsCI(chatwindowstringlist, s)){
        foreach(irc_chatwindow *w,chatwindows){
            if(w->chatpartner==s && w->isHidden())
                w->show();
            if(w->chatpartner==s){
                w->raise();
                qApp->setActiveWindow(w);
            }
            hiddenchatwindowshelper.removeAll(w);
            querylist.removeAll(s);
        }
        return;
    }
    emit sigopenchatwindow(s);
    QApplication::processEvents();
    chatwindowstringlist << s;
    chatwindows.push_back(new irc_chatwindow(net, s));
    connect(chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
    chatwindows.last()->show();
}
void irc_window::usesettingswindow(const QString &s) {
    if (s == "cbalertmeonnotice" || s == "")
        alertonnotice = singleton<settingswindow>().from_map("cbalertmeonnotice").value<bool> ();
    if (s == "cbalertfromnormal" || s == "")
        alertonprivmsg
                = singleton<settingswindow>().from_map("cbalertfromnormal").value<bool> ();
    if (s == "cbignorysappearinchannel" || s == "")
        acceptignorys = singleton<settingswindow>().from_map("cbignorysappearinchannel").value<
                        bool> ();
    disconnect(net, SIGNAL(sigusergarbagejoin(const QString&,const QString&)),this, SLOT(gotgarbagejoin(const QString&,const QString&)));
    disconnect(net, SIGNAL(sigusergarbagepart(const QString&,const QString&)),this, SLOT(gotgarbagepart(const QString&,const QString&)));
    disconnect(net, SIGNAL(sigusergarbagequit(const QString&,const QString&)),this, SLOT(gotgarbagequit(const QString&,const QString&)));
    if (s == "chbjoininfo" || s == "") {
        if (singleton<settingswindow>().from_map("chbjoininfo").value<bool> ())
            connect(net, SIGNAL(sigusergarbagejoin(const QString&,const QString&)),this, SLOT(gotgarbagejoin(const QString&,const QString&)));
    }
    if (s == "chbpartinfo" || s == "") {
        if (singleton<settingswindow>().from_map("chbpartinfo").value<bool> ())
            connect(net, SIGNAL(sigusergarbagepart(const QString&,const QString&)),this, SLOT(gotgarbagepart(const QString&,const QString&)));
    }
    if (s == "chbquitinfo" || s == "") {
        if (singleton<settingswindow>().from_map("chbquitinfo").value<bool> ())
            connect(net, SIGNAL(sigusergarbagequit(const QString&,const QString&)),this, SLOT(gotgarbagequit(const QString&,const QString&)));
    }

}
void irc_window::minimize() {
    if (!hiddenchannelwindowshelper.contains(this))
        this->hiddenchannelwindowshelper.push_back(this);
    this->hide();
}
void irc_window::changealpha(int i) {
    singleton<snpsettings>().map["channeltransparency"] = i;
    if (i == 100)
        this->setWindowOpacity(1);
    else
        this->setWindowOpacity((double) i / 100);
    singleton<snpsettings>().safe();
}
void irc_window::mysetwindowtitle() {
    setWindowTitle(windowtitlechannel + " " + windowtitletime + " " + window::tr(
            "Users online") + windowtitleaway.simplified());
}
void irc_window::getuserscount(QStringList sl) {
    QString s;
    foreach(QString str,sl) {
        if (str.contains(this->currentchannel))
            s = str;
    }
    sl = s.split(" ", QString::SkipEmptyParts);
    if (sl.size() > 1) {
        this->windowtitletime = sl[1];
    }
    this->mysetwindowtitle();
}
irc_window::~irc_window() {
    ui.buttonlayout->removeWidget(buttons);
    buttons->setParent(0);
    singleton<snpsettings>().map[this->currentchannel];
    QVariantList windowstates;
    windowstates << this->saveGeometry();
    windowstates << ui.splitter1->saveState();
    windowstates << ui.splitter2->saveState();
    windowstates << ui.users->header()->saveState();

    singleton<snpsettings>().map[this->currentchannel]
            = windowstates;
    if(net!=0)
        net->partchannel(currentchannel);
    QString s = currentchannel;
    disconnect();
    hiddenchannelwindowshelper.removeAll(this);
    net->deleteLater();
}
void irc_window::showInformationAboutClan(QString clan){
    clan=clan.toLower();
}
void irc_window::openchatwindowhidden(const QString &s) {
    chatwindowstringlist << s;
    chatwindows.push_back(new irc_chatwindow(net, s));
    hiddenchatwindowshelper << chatwindows.last();
    connect(chatwindows.last(), SIGNAL(closed()),this, SLOT(chatwinowclosed()));
}
