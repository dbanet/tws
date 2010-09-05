#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include "ui_mainwindow.h"
#include<QList>
#include <QSystemTrayIcon>
#include"maintoolbox.h"
class window;
class netcoupler;
class chatwindow;
class QMenu;
class ctcphandler;
class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = 0);
    ~mainwindow();
    void appenddebugmessage(const QString &);
    static QString debugmsg;
    static QMap<QString,QString> rememberwhogotaway;
    static QList< ::window * > windowlist;
    static QList< ::chatwindow*> hiddenchatwindowshelper;
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
    QRegExpValidator *validatorclient;

    void setlanguage(const QString&);
    void init_menus();
    void get_baseStyleSheet();
    QString baseStyleSheet;
private slots:
    void on_pbabout_clicked();
    void on_pbsettings_clicked();
    void getchannellist(const QStringList &);
    void joinclicked();
    void joinslot(const QString&);
    void chooseclicked();
    void onquit();
    void disconnect_netcoupler();
    void returntotabsettings(int);
    void trayactivation(QSystemTrayIcon::ActivationReason);
    void traymenutriggered(QAction *);
    void pbrememberjoinclicked();

    void snpsetcontains(const QString&);

    void windowremoved(const QString&);
    void usesettingswindow(const QString &s="");

    void chatwinowclosed();

    void quitslot();

    void awayboxok();
    void awaymessagechanged();

    void gotctcpsignal(const QString&,const QString&);

    void settextscheme(const QString&);

    void openchatwindow(const QString &);
    void openchatwindowhidden(const QString &);
    void gotprvmsg(const QString &user, const QString &receiver,
                   const QString &msg);
    void connected();
    void disconnected();
protected:
    void changeEvent (QEvent*);
    void closeEvent ( QCloseEvent * event );
signals:
    void sigopenchatwindow(const QString&);
};

#endif // MAINWINDOW_H
