#ifndef irc_WINDOW_H
#define irc_WINDOW_H

#include <QtGui/QWidget>
#include<QMenu>
#include<QPointer>
#include"ui_irc_window.h"
#include"irc_netcoupler.h"
#include"src/wormnet/userstruct.h"
class buttonlayout;
class chathandler;
class irc_chatwindow;
class irc_window : public QWidget
{
    Q_OBJECT

public:
    irc_window(irc_netcoupler*,QString s,int i=1);
    ~irc_window();
    const QString currentchannel;
    static QList<irc_chatwindow*> chatwindows;
    static QStringList chatwindowstringlist;
    void gotdebugmsg(const QString&);   
    buttonlayout *buttons;
    static QList< ::irc_window*> hiddenchannelwindowshelper;
    QPointer<chathandler> chat;		//handles the whole textbrowser
    QString windowtitleaway;
    void mysetwindowtitle();
public slots:
    void minimize();
private slots:
    void gotmsg(const QString&,const QString&,const QString&);    
    void gotnotice(const QString&,const QString&,const QString&);
    void gotgarbagejoin(const QString&,const QString&);
    void gotgarbagepart(const QString&,const QString&);
    void gotgarbagequit(const QString&,const QString&);
    void sendmsg();
    void sendnotice();
    void useritempressed(const QModelIndex&);
    void useritemdblclicked(const QModelIndex&);
    void getuserinfo(const QString&);
    void openchatwindow(const QString&);     

    void userselectionchanged(const QItemSelection&,const QItemSelection&);
    void setselection(const QModelIndex&,const QWidget*);

    void usesettingswindow(const QString &s="");

    void expandchannels(QStringList sl);	//expand on startup   
    void changealpha(int);
    void getuserscount(QStringList);    
    void openchatwindowhidden(const QString &);

private:
    void gotprvmsg(const QString&,const QString&,const QString&);
    irc_netcoupler *net;
    Ui::irc_windowClass ui;

    QMenu usermenu;
    QMenu costumlistmenu;

    void sendnoticeaction();
    void showInformationAboutClan(QString);

    bool alertonnotice;
    bool alertonprivmsg;
    bool acceptignorys;

    QIcon chaticon;

    QString windowtitletime;
    QString windowtitlechannel;
    QList< irc_chatwindow*> hiddenchatwindowshelper;
signals:
    void sigwindowclosed(const QString&);
    void sigalert(QWidget*);
    void sigjoinchannel(const QString&);
    void sigopenchatwindow(const QString&);
    //void sigopenmmconvo();
protected:
    void closeEvent ( QCloseEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);		//for the linedit in ui
};
#endif // irc_WINDOW_H
