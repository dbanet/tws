#include "balloon_handler.h"
#include "settingswindow.h"
#include "about.h"
#include "usermodel.h"
#include "netcoupler.h"
#include "global_functions.h"
#include "usermessage.h"

#include <QTime>
#include <QSystemTrayIcon>
#include <QDir>

balloonHandler::balloonHandler() {
    tray=new QSystemTrayIcon;
    tray->setToolTip(QObject::tr("The Wheat Snooper version ")+about::version);
    tray->setIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/tray.png"));
    tray->setObjectName("normalwidget");
    tray->show();
    balloonHelper << "[" + QTime::currentTime().toString("hh:mm") + "] "
            + QObject::tr("The Wheat Snooper version ")+about::version+QObject::tr(" started!");
    if(S_S.getbool("cbshowballoons"))
        tray->showMessage(tr("Notifications."), balloonHelper.join("\n"));
}
balloonHandler::~balloonHandler() {
    delete tray;
}
void balloonHandler::showBalloon( ){
    if(S_S.getbool("cbshowballoons"))
        tray->showMessage(tr("Notifications."), balloonHelper.join("\n"));
    if (! (balloonHelper.size() > S_S.getint("sbmaximumballonmessages")))
        return;
    int max_size=S_S.getint("sbmaximumballonmessages");
    balloonHelper = balloonHelper.mid(balloonHelper.size()- max_size);
}
void balloonHandler::disconnected() {
    balloonHelper << "[" + QTime::currentTime().toString("hh:mm") + "] " + tr("Disconnected from Wormnet");
    showBalloon();
}
void balloonHandler::connected(){
    balloonHelper << "[" + QTime::currentTime().toString("hh:mm") + "] " + tr("Connected to Wormnet");
    showBalloon();
}
void balloonHandler::connectedToLeagueServer(QString servicename){
    balloonHelper << "[" + QTime::currentTime().toString("hh:mm") + "] " + tr("Connected to %1").arg(servicename);
    showBalloon();
}

void balloonHandler::buddyLeft() {
    if (!S_S.getbool("chbbuddyballoonleaves"))
        return;
    balloonHelper << usermodel::buddylefthelper;
    usermodel::buddylefthelper.clear();
    showBalloon();
}
void balloonHandler::buddyArrived() {
    if (!S_S.getbool("chbbuddyballoonarives"))
        return;
    balloonHelper << usermodel::buddyarrivedhelper;
    usermodel::buddyarrivedhelper.clear();
    showBalloon();
}
void balloonHandler::gotPrivmsg(const usermessage &u){
    if (!S_S.getbool("cbshowballoons"))
        return;
    if (containsCI(S_S.getStringList("mutedusers"), u.user())
    || (containsCI(S_S.ignorelist, u.user())))
        return;
    balloonHelper << "[" + QTime::currentTime().toString("hh:mm") + "] " + u.user() + tr(" said: ") + u.msg();
    showBalloon();
}
void balloonHandler::gotGame(const QString playername, const QString gamename) {
    if (gamename.size() > 5) {
        balloonHelper
                << "[" + QTime::currentTime().toString("hh:mm") + "] "
                + playername + " " + tr("is Hosting: ") + gamename.left(5) + "...";
    } else
        balloonHelper<< "[" + QTime::currentTime().toString("hh:mm") + "] "
                + playername + " " + tr("is Hosting: ") + gamename.left(5);
    showBalloon();
}
void balloonHandler::gotCustomWord(const QString word, const QString user) {
    if (containsCI(S_S.getStringList("mutedusers"), user))
        return;
    if (!S_S.getbool("cbcostumword"))
        return;
    balloonHelper<< "[" + QTime::currentTime().toString("hh:mm") + "] "
            +"... " +  word + " ..." + tr("was highlighted by ")+ user;
    showBalloon();
}
void balloonHandler::hideTray() {
    tray->hide();
    tray->disconnect();
}
void balloonHandler::setNormalTrayIcon() {
    tray->setIcon(QIcon(QApplication::applicationDirPath()
                        + QDir::separator() + "snppictures"
                        + QDir::separator() + "tray.png"));
}
void balloonHandler::setAwayTrayIcon(){
    tray->setIcon(QIcon(QApplication::applicationDirPath() + QDir::separator()
                        + "snppictures" + QDir::separator() + "trayaway.png"));
}
void balloonHandler::alert(QString user, QWidget *w){
    if (containsCI(S_S.getStringList("mutedusers"), user))
        return;
    qApp->alert(w);
}
