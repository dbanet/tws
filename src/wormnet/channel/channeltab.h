#ifndef CHANNELTAB_H
#define CHANNELTAB_H

#include <QMainWindow>
#include <QItemSelection>
#include <QWidget>
#include <QMenu>
#include <QPointer>
#include <QDockWidget>
#include <QSortFilterProxyModel>

#include "../netcoupler.h"
#include "../userstruct.h"
#include "../hoststruct.h"
#include "hostmodel.h"
#include "chanusermodel.h"

namespace Ui {
    class channelTab;
}
class hostbox;
class chatHandler;
class chatwindow;
class channelTab : public QMainWindow
{
    Q_OBJECT

public:
    explicit channelTab(QString,QMenu*,QWidget *parent=0);
    ~channelTab();
    QString currentChannel;
    static QList<chatwindow*> chatwindows;
    static QStringList chatwindowstringlist;
    void gotdebugmsg(const QString&);

    static QList< ::channelTab*> hiddenchannelwindowshelper;
    QPointer<chatHandler> chat;		//handles the whole textbrowser
    QString windowtitleaway;
    void mysetwindowtitle();
    void getusermessage(usermessage u);
    void lockUI();
    void unlockUI();
    void monopolizeMenu(QMenu *channelMenu);
    void dropMenu();
    QMenu* getMenu();
    bool docked;
    void setDocked(bool);
    QList<hoststruct> *hosts;
    QList<userstruct> *users;

public slots:
    void minimize();
    void sendmsg();
    void useritempressed(const QModelIndex&);
    void useritemdblclicked(const QModelIndex&);
    void getuserinfo(const QString&);
    void openchatwindow(const QString&);

    void hostitempressed(const QModelIndex&);
    void hostitemdblclicked(const QModelIndex&);

    void hboxok();

    void userselectionchanged(const QItemSelection&,const QItemSelection&);
    void setselection(const QModelIndex&,const QWidget*);

    //void expandchannels(QStringList sl);	//expand on startup
    void expandchannels();
    void getjoinmenu();
    void openHostBox();
    void changealpha(int);
    void showbuttons();
    void setupWindowTitleOnJoin(QString,int);
    void setupWindowTitleOnChangeOfUserAmount(QString,int);
    void pbemotclicked();

    void insertemot(QString s);

    void setHosts(QList<hoststruct>*);
    void setUsers(QList<userstruct>*);

private:
    Ui::channelTab *ui;

    QMenu joinmenu;
    QMenu joinmenu2;
    QMenu hostmenu;
    QMenu usermenu;
    QMenu customlistmenu;
    QMenu *channelMenu;

    QPointer<hostbox> hbox;

    void showInformationAboutClan(QString);

    QIcon chaticon;
    int whichuiison;

    QString windowtitletime;
    QString windowtitlechannel;

    QWidget *hostsDockTitleBar;
    QWidget *usersDockTitleBar;
    QWidget *chatDockTitleBar;

    HostModel *hostModel;
    QSortFilterProxyModel *sortedHostModel;
    ChanUserModel *chanUserModel;
    QSortFilterProxyModel *sortedChanUserModel;

signals:
    void sigclosed();
    void sigjoinchannel(const QString&);
    void sigopenchatwindow(const QString&);
    void askTabDocking(channelTab*);
    void askTabUndocking(channelTab*);
protected:
    void closeEvent ( QCloseEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);		//for the linedit in ui
private slots:
    void on_actionDockUndock_triggered(bool);
    void on_users_customContextMenuRequested(const QPoint &pos);
    void userListContextMenu_inclInBuddyList(bool);
    void userListContextMenu_inclInIgnrList(bool);
    void userListContextMenu_startPrvTalk();
    void userListContextMenu_actionInclInDisallowedClanNames(bool);
};

#endif // CHANNELTAB_H
