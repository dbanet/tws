#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMenu>
#include <QPointer>

#include "ui_window.h"
#include "ui_window2.h"
#include "ui_window3.h"
#include "netcoupler.h"
#include "userstruct.h"
#include "hoststruct.h"
#include "uihelper.h"
class buttonlayout;
class hostbox;
class chatHandler;
class chatwindow;
class window : public QWidget
{
    Q_OBJECT
public:
    window(QString s,int i=1);
    ~window();
    const QString currentChannel;
    static QList<chatwindow*> chatWindowsList;
    static QStringList chatWindowsStringList;
    void gotdebugmsg(const QString&);

    buttonlayout *buttons;
    static QList< ::window*> hiddenChannelWindowsHelper;
    QPointer<chatHandler> chat;		//handles the whole textbrowser
    QString windowAwayTitle;
    void updateWindowTitle();    
    void getUserMessage(usermessage u);
public slots:
    void minimize();        
private slots:        
    void sendMessage();
    void userItemPressed(const QModelIndex&);
    void userItemDoubleClicked(const QModelIndex&);
    void getUserInfo(const QString&);
    void openChatWindow(const QString&);

    void hostItemPressed(const QModelIndex&);
    void hostItemDoubleClicked(const QModelIndex&);

    void hostBoxOk();

    void userSelectionChanged(const QItemSelection&,const QItemSelection&);
    void setSelection(const QModelIndex&,const QWidget*);

    //void expandchannels(QStringList sl);	//expand on startup
    void expandChannels();
    void getJoinMenu();
    void openHostBox();
    void changeAlpha(int);
    void showButtons();
    void setupWindowTitleOnJoin(QString,int);
    void setupWindowTitleOnChangeOfUserAmount(QString,int);
    void pbEmotClicked();

    void insertEmot(QString s);

private:
    uihelper ui;
    Ui::windowClass ui1;
    Ui::Form ui2;
    Ui::Form3 ui3;

    QMenu joinMenu;
    QMenu joinMenu2;
    QMenu hostMenu;
    QMenu userMenu;
    QMenu customListMenu;

    QPointer<hostbox> hostBox;

    void showInformationAboutClan(QString);    

    QIcon chatIcon;
    int whichUIIsOn;

    QString windowTitleUsersAmount;
    QString windowTitleChannelName;

signals:
    void sigClosed();
    void sigjoinChannel(const QString&);
    void sigOpenChatWindow(const QString&);
    void sigChangeLeagueState();
protected:
    void closeEvent ( QCloseEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);		//for the linedit in ui
};
#endif // WINDOW_H
