#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "maintoolbox.h"
#include "qobjectwrapper.h"
#include "usermessage.h"

#include <QWidget>
#include <QList>
#include <QSystemTrayIcon>

class window;
class netcoupler;
class chatwindow;
class QMenu;

class mainwindow : public QWidget
{
    Q_OBJECT

public:      
    void appendDebugMessage(const QString &);
    QString debugMsg;
    QList< ::window * > windowList;
    QList< ::chatwindow*> hiddenChatWindowsHelper;
    void fillsnpsettings();
    void quit();

    static QString toString(){
        return "mainwindow";
    }

public slots:
    void returnToLoginTab();

private:
    mainwindow();
    ~mainwindow();
    typedef ::window channelWindow;
    Ui::mainwindowClass ui;
    QStringList joinedChannels;
    QStringList channelList;

    QMenu *trayMenu;
    QMenu *joinMenu;
    QMenu *layoutManu;
    QMenu *styleMenu;
    QMenu *stuffMenu;
    QMenu *textSchemeMenu;
    QMenu *languageMenu;
    QMenu *arrangementMenu;

    int whichUIType;
    bool joinOnStartUp;
    QRegExpValidator *validator;
    void setLanguage(const QString&);
    void initMenus();
    void getBaseStyleSheet();
    void handleAwayBox();
    QString baseStyleSheet;
    void joinGameSurge();    
    void connectToNetwork();
    void setLeague();
    QStringList lastOpenedWindows;
    QStringList lastOpenedChatWindows;

    void openChatWindow(QString user);
    void gotNotice(const usermessage u);
    void gotScriptMessage(const usermessage);

private slots:    
    void on_chbautojoin_clicked(bool checked);
    void on_cbleagueservers_activated(QString );
    void on_pbeditleagueprofile_clicked();
    void on_cbenabletus_toggled(bool checked);
    void on_pbjoin_clicked();
    void join(const QString channel);
    void openChatWindowRaised(const QString &);
    void on_pbabout_clicked();
    void on_pbsettings_clicked();
    void getChannelList(const QStringList &);
    void chooseClicked();
    void trayActivation(QSystemTrayIcon::ActivationReason);
    void trayMenuTriggered(QAction *);
    void on_pbrememberjoin_clicked();
    void snpSetContains(const QString&);
    void useSettingsWindow(const QString &s="");
    void chatWindowClosed();
    void awayBoxOk();
    void awayMessageChanged();
    void setTextScheme(const QString&);
    void openChatWindowHidden(const QString &);
    void gotUserMessage(usermessage u);
    void connected();
    void disconnected();
    void reconnect();
    void reconnect2();
    void reopenChatWindowsAndChannelWindows();
    void leagueServerConnectionFailed();
    void leagueServerConnectionSuccess();
    void currentTabChanged(int);
    void leagueServerProfilePage(QString);
    void windowClosed();

protected:
    void closeEvent ( QCloseEvent * event );    
signals:
    void sigOpenChatWindow(const QString&);
    void sigDisconnected();

    friend class qobjectwrapper<mainwindow>;
};

#endif // MAINWINDOW_H
