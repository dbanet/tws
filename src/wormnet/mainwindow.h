#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "maintoolbox.h"
#include "qobjectwrapper.h"
#include "usermessage.h"

#include <QWidget>
#include <QList>
#include <QSystemTrayIcon>
#include <QMainWindow>

class channelTab;
class netcoupler;
class chatwindow;
class QMenu;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void appenddebugmessage(const QString &);
    QString debugmsg;
    QList< ::channelTab * > windowlist;
    QList< ::chatwindow*> hiddenchatwindowshelper;
    void fillsnpsettings();
    void quit();
    static QString toString(){
        return "mainwindow";
    }
    ~MainWindow();

public slots:
    void returntologintab();
    channelTab* dockTab(channelTab *tab=0);
    //serverTab* dockTab(serverTab *tab=0);
    //privateTab* dockTab(privateTab *tab=0);

    channelTab* undockTab(channelTab *tab=0);
    //serverTab* undockTab(serverTab *tab=0);
    //privateTab* undockTab(privateTab *tab=0);

private:
    Ui::MainWindow *ui;
    typedef ::channelTab channelwindow;
    QStringList currentchannellist;	//TODO: dont forget to clear this lists on ------void returntotabsettings(int);
    QStringList channellist;

    QMenu *traymenu;
    QMenu *joinmenu;
    QMenu *layoutmenu;
    QMenu *stylemenu;
    QMenu *stuffmenu;
    QMenu *textschememenu;
    QMenu *languagemenu;
    QMenu *arrangementmenu;

    int whichuitype;
    bool joinonstartup;
    QRegExpValidator *validator;
    void setlanguage(const QString&);
    void init_menus();
    void get_baseStyleSheet();
    void handleAwayBox();
    QString baseStyleSheet;
    void joinGameSourge();
    void connectToNetwork();
    void setleague();
    QStringList lastOpenedWindows;
    QStringList lastOpenedChatWindows;

    void openchatwindow(QString user);
    void gotnotice(const usermessage u);
    void gotscriptmsg(const usermessage);

private slots:
    void on_chbautojoin_clicked(bool checked);
    void on_cbleagueservers_activated(QString );
    void on_pbeditleagueprofile_clicked();
    void on_cbenabletus_toggled(bool checked);
    void on_pbjoin_clicked();
    void join(const QString channel);
    void openchatwindowraised(const QString &);
    void on_pbabout_clicked();
    void on_pbsettings_clicked();
    void getchannellist(const QStringList &);
    void chooseclicked();
    void trayactivation(QSystemTrayIcon::ActivationReason);
    void traymenutriggered(QAction *);
    void pbrememberjoinclicked();
    void snpsetcontains(const QString&);
    void usesettingswindow(const QString &s="");
    void chatwinowclosed();
    void awayboxok();
    void awaymessagechanged();
    void settextscheme(const QString&);
    void openchatwindowhidden(const QString &);
    void gotusermsg(usermessage u);
    void connected();
    void disconnected();
    void reconnect();
    void reconnect2();
    void reopenChatWindowsAndChannelWindows();
    void leagueserverconnectionfailed();
    void leagueserverconnectionsuccess();
    void currenttabchanged(int);
    void leagueserverprofilepage(QString);
    void windowclosed();
    void actionUndockTabTriggered();

    void on_actionReconnect_triggered();
    void on_actionClose_triggered();
    void on_actionLock_the_UI_triggered(bool checked);
    void on_actionSet_away_triggered();
    void on_actionBuddy_list_triggered();
    void on_actionLog_browser_triggered();
    void on_actionChange_ingame_nick_triggered();
    void on_actionSelect_encoding_triggered();
    void on_actionVolume_triggered();
    void on_actionText_scheme_maker_triggered();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionDisconnect_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionStalk_words_triggered(bool checked);
    void on_actionSound_triggered(bool checked);
    void on_actionTray_notifications_triggered(bool checked);
    void on_actionSort_userlist_triggered(bool checked);
    void on_menuSpectate_triggered(QAction*);
    void on_menuLeagueColor_triggered(QAction*);

protected:
    void closeEvent ( QCloseEvent * event );

signals:
    void sigopenchatwindow(const QString&);
    void sigdisconnected();

    friend class qobjectwrapper<MainWindow>;
};

#endif // MAINWINDOW_H
