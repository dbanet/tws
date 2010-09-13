#ifndef irc_chatwindow_H
#define irc_chatwindow_H

#include <QtGui/QWidget>
#include<QPointer>
#include "ui_irc_chatwindow.h"
#include "src/wormnet/ctctphandlerwidget.h"
class irc_netcoupler;
class chathandlerprv;
class QStatusBar;
class irc_chatwindow : public QWidget
{
    Q_OBJECT

public:
    irc_chatwindow(irc_netcoupler*, const QString&, QWidget *parent = 0);
    QString chatpartner;
    void getgamerwho();
    ~irc_chatwindow();
    void gotmsg(const QString&,const QString&,const QString&);
    void setaway(bool,const QString &msg="");
private slots:
    void on_pbresize_clicked();
    void sendmsg(QString s="");
    void sendnotice();
    void sendnoticeaction();
    void garbagemapchanged(const QString&,const QString&);
    void usesettingswindow(const QString &s="");
    void pbmuteclicked();
    void pbbuddyclicked();
    void pblogclicked();
    void pbidleclicked();
    void pbctcpclicked();
    void gotidletime(const QString&,int);
    void gotnosuchnick(const QString&);
    void channelmsg(const QString&,const QString&,const QString&);
    void filtergarbage();
private:
    QPointer<chathandlerprv> chat;		//handles the whole textbrowser
    irc_netcoupler *net;
    Ui::chatwindowClass ui;

    QStatusBar *statusbar;
    //bool mouseisinbuttonscrollarea;
    bool userisoffline;
protected:
    void closeEvent ( QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *event);		//for the linedit in ui
signals:
    void sigalert(QWidget*);
    void closed();
};

#endif // irc_chatwindow_H
