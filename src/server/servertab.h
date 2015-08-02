#ifndef SERVERTAB_H
#define SERVERTAB_H

#include <QtWebKit>
#include <QWebView>
#include <QMainWindow>
#include <QPointer>
#include <QSortFilterProxyModel>

#include "../mainwindow.h"
#include "../userstruct.h"
#include "../channel/chanusermodel.h"
class MainWindow;

namespace Ui {
    class serverTab;
}

class serverTab : public QMainWindow
{
    Q_OBJECT

public:
    explicit serverTab(netcoupler *netc,MainWindow *mainWnd,QWidget *parent = 0);
    QList<userstruct> *noChannelUsers;

    void fillSnpSettings();
    void returnToLoginTab();
    void addToServInfo(QString);
    void set_chbautojoin(bool);
    void set_chbminimized(bool);
    ~serverTab();

public slots:
    void gotChannelList(const QStringList&);
    void gotUserList(QList<userstruct>*);

private slots:
    void on_pbjoin_clicked();
    void on_pbrememberjoin_clicked();
    void on_chbautojoin_clicked(bool checked);
    void userListContextMenu_inclInBuddyList(bool);
    void userListContextMenu_inclInIgnrList(bool);
    void userListContextMenu_startPrvTalk();
    void userListContextMenu_actionInclInDisallowedClanNames(bool);
    void on_noChannelUsersView_customContextMenuRequested(const QPoint&);

private:
    Ui::serverTab *ui;
    QPointer<MainWindow> mainWnd;
    QStringList channelList;
    ChanUserModel *noChannelUserModel;
    QSortFilterProxyModel *sortedNoChannelUserModel;
    netcoupler *netc;
};

#endif // SERVERTAB_H
