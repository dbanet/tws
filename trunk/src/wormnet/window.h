#ifndef WINDOW_H
#define WINDOW_H

#include<QtGui/QWidget>
#include<QMenu>
#include<QPointer>

#include"ui_window.h"
#include"ui_window2.h"
#include"ui_window3.h"
#include"netcoupler.h"
#include"userstruct.h"
#include"hoststruct.h"
#include"uihelper.h"
class buttonlayout;
class hostbox;
class chathandler;
class chatwindow;
class window : public QWidget
{
    Q_OBJECT
public:
    window(QString s,int i=1);
    ~window();
    const QString currentchannel;
    static QList<chatwindow*> chatwindows;
    static QStringList chatwindowstringlist;
    void gotdebugmsg(const QString&);

    buttonlayout *buttons;
    static QList< ::window*> hiddenchannelwindowshelper;
    QPointer<chathandler> chat;		//handles the whole textbrowser
    QString windowtitleaway;
    void mysetwindowtitle();    
    void getusermessage(usermessage u);
public slots:
    void minimize();        
private slots:        
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

    void expandchannels(QStringList sl);	//expand on startup
    void expandchannels();
    void getjoinmenu();
    void openhbox();
    void changealpha(int);
    void showbuttons();
    void getuserscount(QStringList);
    void pbemotclicked();

    void insertemot(QString s);

private:
    uihelper ui;
    Ui::windowClass ui1;
    Ui::Form ui2;
    Ui::Form3 ui3;

    QMenu joinmenu;
    QMenu joinmenu2;
    QMenu hostmenu;
    QMenu usermenu;
    QMenu costumlistmenu;

    QPointer<hostbox> hbox;

    void showInformationAboutClan(QString);    

    QIcon chaticon;
    int whichuiison;

    QString windowtitletime;
    QString windowtitlechannel;

signals:
    void sigclosed();
    void sigjoinchannel(const QString&);
    void sigopenchatwindow(const QString&);
    void sigchangeleaguestate();
protected:
    void closeEvent ( QCloseEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);		//for the linedit in ui
};
#endif // WINDOW_H
