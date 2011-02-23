#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QtGui/QWidget>
#include<QPointer>
#include "ui_chatwindow.h"
#include "ctctphandlerwidget.h"
class netcoupler;
class chathandlerprv;
class QStatusBar;
class chatwindow : public QWidget
{
    Q_OBJECT

public:
    chatwindow(const QString&, QWidget *parent = 0);
    QString chatpartner;
    void getgamerwho(QString prefix=QString());
    ~chatwindow();
    void gotmsg(const QString&,const QString&,const QString&);
    void setaway(bool,const QString &msg="");
private slots:
    void on_pbresize_clicked();
    void sendmsg();
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
    void sendnotice();
    void sendnoticeaction();
    QPointer<chathandlerprv> chat;		//handles the whole textbrowser
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

#endif // CHATWINDOW_H
