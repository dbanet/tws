#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui/QWidget>
#include<QMenu>
#include<QPointer>
#include "ui_window.h"
#include "ui_window2.h"
#include "ui_window3.h"
#include"netcoupler.h"
#include"userstruct.h"
#include"hoststruct.h"
#include"uihelper.h"
class buttonlayout;
class hostbox;
class hostprvbox;
class chathandler;
class chatwindow;
class window : public QWidget
{
    Q_OBJECT

public:
    window(netcoupler*,QString s,int i=1);
    ~window();
	const QString currentchannel;
	static QList<chatwindow*> chatwindows;
	static QStringList chatwindowstringlist;
	void gotdebugmsg(const QString&);
	void gotprvmsg(const QString&,const QString&,const QString&);

        buttonlayout *buttons;
	static QList< ::window*> hiddenchannelwindowshelper;
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

	void hostitempressed(const QModelIndex&);
	void hostitemdblclicked(const QModelIndex&);


	void hboxok();
	void hboxprvok(const QString&);

	void userselectionchanged(const QItemSelection&,const QItemSelection&);
	void setselection(const QModelIndex&,const QWidget*);

	void usesettingswindow(const QString &s="");

	void expandchannels(QStringList sl);	//expand on startup
	void expandchannels();
	void getjoinmenu();
	void openhbox();	
	void changealpha(int);
	void showbuttons();
	void getuserscount(QStringList);

private:
	netcoupler *net;
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
	QPointer<hostprvbox> hprvbox;

	void sendnoticeaction();
        void showInformationAboutClan(QString);

	bool alertonnotice;
	bool alertonprivmsg;
	bool acceptignorys;

	QIcon chaticon;
	int whichuiison;

	QString windowtitletime;
	QString windowtitlechannel;
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
#endif // WINDOW_H
