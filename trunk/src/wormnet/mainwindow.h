#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include "ui_mainwindow.h"
#include"maintoolbox.h"
#include"global_macros.h"
#include<QList>
#include <QSystemTrayIcon>

class window;
class netcoupler;
class chatwindow;
class QMenu;
class mainwindow : public QWidget
{
    Q_OBJECT

public:  
    void appenddebugmessage(const QString &);
    QString debugmsg;
    QMap<QString,QString> rememberwhogotaway;
    QList< ::window * > windowlist;
    QList< ::chatwindow*> hiddenchatwindowshelper;
    void fillsnpsettings();
    void quit();

    DECLARE_SINGLETON(mainwindow);     
public slots:
    void returntologintab();
private:
    typedef ::window channelwindow;
    Ui::mainwindowClass ui;
    //QList<chatwindow* > chatwindows;
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
    QString baseStyleSheet;

    void joinGameSourge();    

    void connectToNetwork();
    void setleague();
    QStringList lastOpenedWindows;
    QStringList lastOpenedChatWindows;

private slots:
    void on_pbeditleagueprofile_clicked();
    void on_cbenabletus_toggled(bool checked);
    void on_pbjoin_clicked();
    void join(const QString channel);
    void openchatwindow(const QString &);
    void on_pbabout_clicked();
    void on_pbsettings_clicked();
    void getchannellist(const QStringList &);   
    void chooseclicked();        
    void trayactivation(QSystemTrayIcon::ActivationReason);
    void traymenutriggered(QAction *);
    void pbrememberjoinclicked();

    void snpsetcontains(const QString&);

    void windowremoved(const QString&);
    void usesettingswindow(const QString &s="");

    void chatwinowclosed();    

    void awayboxok();
    void awaymessagechanged();

    void gotctcpsignal(const QString&,const QString&);

    void settextscheme(const QString&);

    void openchatwindowhidden(const QString &);
    void gotprvmsg(const QString &user, const QString &receiver,const QString &msg);
    void connected();
    void disconnected();
    void reconnect();

    void reopenChatWindowsAndChannelWindows();

    void leagueserverconnectionfailed();
    void leagueserverconnectionsuccess();

    void currenttabchanged(int);

    void leagueserverprofilepage(QString);

protected:
    void changeEvent (QEvent*);
    void closeEvent ( QCloseEvent * event );
signals:
    void sigopenchatwindow(const QString&);
};

#endif // MAINWINDOW_H
