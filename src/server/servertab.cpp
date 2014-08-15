#include "servertab.h"
#include "ui_servertab.h"
#include "../netcoupler.h"
#include "../global_macros.h"
#include "../sqlsettings.h"
#include "ui_chanUserListMenu.h"
#include "../global_functions.h"

serverTab::serverTab(MainWindow *mainWnd,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serverTab),
    mainWnd(mainWnd)
{
    ui->setupUi(this);
    connect(&singleton<netcoupler>(),SIGNAL(sigGotChanList(const QStringList &)),this,SLOT(gotChannelList(const QStringList &)));

    /***********************/
    /* setting up userlist */
    /***********************/
    this->noChannelUsers=new QList<userstruct>();
    this->noChannelUserModel=new ChanUserModel("*",noChannelUsers);

    this->sortedNoChannelUserModel=new QSortFilterProxyModel();
    sortedNoChannelUserModel->setSourceModel(noChannelUserModel);
    sortedNoChannelUserModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    sortedNoChannelUserModel->setSortRole(noChannelUserModel->SortingRole);
    ui->noChannelUsersView->setSortingEnabled(true);
    ui->noChannelUsersView->header()->setSortIndicatorShown(true);

    ui->noChannelUsersView->setModel(this->sortedNoChannelUserModel);
    ui->noChannelUsersView->setEnabled(1);
    ui->noChannelUsersView->setAlternatingRowColors(1);
    ui->noChannelUsersView->setRootIsDecorated(false);
    ui->noChannelUsersView->setIndentation(0);
    ui->noChannelUsersView->installEventFilter(this);
    ui->noChannelUsersView->setColumnWidth(0,22+3);
    ui->noChannelUsersView->setColumnWidth(1,48+3);
    ui->noChannelUsersView->setColumnWidth(2,18+3);
    ui->noChannelUsersView->header()->setStretchLastSection(false);
    ui->noChannelUsersView->header()->setResizeMode(0,QHeaderView::Fixed);
    ui->noChannelUsersView->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->noChannelUsersView->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->noChannelUsersView->header()->setResizeMode(3,QHeaderView::Stretch);
    ui->noChannelUsersView->header()->setResizeMode(4,QHeaderView::Interactive);
    ui->noChannelUsersView->header()->setResizeMode(5,QHeaderView::Interactive);
}

serverTab::~serverTab()
{
    delete ui;
}

void serverTab::on_pbjoin_clicked()
{
    mainWnd->join(ui->cbchannels->currentText());
}

void serverTab::on_pbrememberjoin_clicked()
{
    S_S.set("joinonstartup", ui->cbchannels->currentText());
    ui->pbrememberjoin->setText(tr("Autojoin:") + "\n" + S_S.getString("joinonstartup"));
}

void serverTab::on_chbautojoin_clicked(bool checked)
{
    if(checked)
        on_pbrememberjoin_clicked();

}

void serverTab::gotChannelList(const QStringList &sl) {
    connect(*(&singleton<netcoupler>().irc),SIGNAL(sigIRCUpdatedUserList(QList<userstruct>*)),this,SLOT(gotUserList(QList<userstruct>*)));
    ui->cbchannels->clear();
    foreach(QString s,sl)
        ui->cbchannels->addItem(s);
    channelList = sl;
    ui->pbjoin->setEnabled(1);
    //joinmenu->clear();
    //foreach(QString s,channelList) {
    //    joinmenu->addAction(s);
    //}
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesourgeChannelName);
#endif
    if (mainWnd->joinonstartup == 0) {
        mainWnd->snpsetcontains("joinonstartup");
        mainWnd->joinonstartup = 1;
    }
}

void serverTab::gotUserList(QList<userstruct> *users){
    this->noChannelUsers->clear();
    foreach(userstruct user,*users)
        if(user.channel=="*")
            this->noChannelUsers->append(user);
    this->noChannelUserModel->usersChanged();
}

void serverTab::fillSnpSettings(){
    S_S.set("chbminimized", ui->chbminimized->isChecked());
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
}

void serverTab::returnToLoginTab(){
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
    ui->cbchannels->clear();
    ui->pbjoin->setEnabled(0);
}

void serverTab::addToServInfo(QString debugmsg){
    ui->labeltraydescription->insertPlainText(debugmsg);
}

void serverTab::set_chbautojoin(bool state){
    ui->chbautojoin->setChecked(state);
}

void serverTab::set_chbminimized(bool state){
    ui->chbminimized->setChecked(state);
}

void serverTab::on_noChannelUsersView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index=this->sortedNoChannelUserModel->mapToSource(ui->noChannelUsersView->indexAt(pos));
    if  (!index.isValid()) return;
    int row   =index.row();
    userstruct user=noChannelUsers->at(row);

    QMenu *userListContextMenu=new QMenu();
    Ui_UserListContextMenu *uiUserListContextMenu=new Ui_UserListContextMenu();
    uiUserListContextMenu->setupUi(userListContextMenu);

    connect(uiUserListContextMenu->actionInclInBuddyList,SIGNAL(triggered(bool)),this,SLOT(userListContextMenu_inclInBuddyList(bool)));
    uiUserListContextMenu->actionInclInBuddyList->setData(pos); // now when the slot gets called, it will be easier to figure out what's the item clicked
    uiUserListContextMenu->actionInclInBuddyList->setChecked(S_S.buddylist.contains(user.nick,Qt::CaseInsensitive));

    connect(uiUserListContextMenu->actionInclInIgnrList,SIGNAL(triggered(bool)),this,SLOT(userListContextMenu_inclInIgnrList(bool)));
    uiUserListContextMenu->actionInclInIgnrList->setData(pos);
    uiUserListContextMenu->actionInclInIgnrList->setChecked(S_S.ignorelist.contains(user.nick,Qt::CaseInsensitive));

    connect(uiUserListContextMenu->actionStartPrvTalk,SIGNAL(triggered()),this,SLOT(userListContextMenu_startPrvTalk()));
    uiUserListContextMenu->actionStartPrvTalk->setData(pos);

    connect(uiUserListContextMenu->actionInclInDisallowedClanNames,SIGNAL(triggered(bool)),this,SLOT(userListContextMenu_actionInclInDisallowedClanNames(bool)));
    uiUserListContextMenu->actionInclInDisallowedClanNames->setData(pos);
    uiUserListContextMenu->actionInclInDisallowedClanNames->setChecked(S_S.dissallowedclannames.contains(user.clan,Qt::CaseInsensitive));

    userListContextMenu->exec(ui->noChannelUsersView->mapToGlobal(pos));
}

void serverTab::userListContextMenu_inclInBuddyList(bool checked){
    QAction *clickedAction=qobject_cast<QAction*>(QObject::sender());
    QPoint   clickedPos=clickedAction->data().toPoint();
    QModelIndex index=this->sortedNoChannelUserModel->mapToSource(ui->noChannelUsersView->indexAt(clickedPos));
    if  (!index.isValid()) return;
    int row   =index.row();
    userstruct user=noChannelUsers->at(row);

    if(checked){
        S_S.set("buddylist",S_S.buddylist<<user.nick);
        if(containsCI(S_S.ignorelist,user.nick))
            S_S.set("ignorelist",removeCI(S_S.ignorelist,user.nick));
    }
    else
        S_S.set("buddylist",removeCI(S_S.buddylist,user.nick));

    /* updating the model so the change will be displayed */
    this->noChannelUserModel->usersChanged();
}

void serverTab::userListContextMenu_inclInIgnrList(bool checked){
    QAction *clickedAction=qobject_cast<QAction*>(QObject::sender());
    QPoint   clickedPos=clickedAction->data().toPoint();
    QModelIndex index=this->sortedNoChannelUserModel->mapToSource(ui->noChannelUsersView->indexAt(clickedPos));
    if  (!index.isValid()) return;
    int row   =index.row();
    userstruct user=noChannelUsers->at(row);

    if(checked){
        S_S.set("ignorelist",S_S.ignorelist<<user.nick);
        if(containsCI(S_S.buddylist,user.nick))
            S_S.set("buddylist",removeCI(S_S.buddylist,user.nick));
    }
    else
        S_S.set("ignorelist",removeCI(S_S.ignorelist,user.nick));

    /* updating the model so the change will be displayed */
    this->noChannelUserModel->usersChanged();
}

void serverTab::userListContextMenu_startPrvTalk(){
/*    QAction *clickedAction=qobject_cast<QAction*>(QObject::sender());
    QPoint   clickedPos=clickedAction->data().toPoint();
    QModelIndex index=this->sortedNoChannelUserModel->mapToSource(ui->noChannelUsersView->indexAt(clickedPos));
    if  (!index.isValid()) return;
    int row   =index.row();
    int column=index.column();
    userstruct user=noChannelUsers->at(row);

    openchatwindow(user.nick);*/
}

void serverTab::userListContextMenu_actionInclInDisallowedClanNames(bool checked){
    QAction *clickedAction=qobject_cast<QAction*>(QObject::sender());
    QPoint   clickedPos=clickedAction->data().toPoint();
    QModelIndex index=this->sortedNoChannelUserModel->mapToSource(ui->noChannelUsersView->indexAt(clickedPos));
    if  (!index.isValid()) return;
    int row   =index.row();
    userstruct user=noChannelUsers->at(row);

    QStringList disallowedClanNames;
    if(checked)
        disallowedClanNames<<S_S.dissallowedclannames<<user.clan;
    else
        foreach(QString disallowedClanName,S_S.dissallowedclannames)
            if(disallowedClanName.toLower()!=user.clan.toLower())
                disallowedClanNames<<disallowedClanName;
    S_S.set("dissallowedclannames",disallowedClanNames);

    this->noChannelUserModel->usersChanged();
}
