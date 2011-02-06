#include "balloon_handler.h"
#include "settingswindow.h"
#include"about.h"
#include"usermodel.h"
#include<QTime>
#include<QSystemTrayIcon>
#include<QDir>
balloon_handler::balloon_handler()
{
    tray=new QSystemTrayIcon;
    tray->setToolTip(QObject::tr("The Wheat Snooper version ")+about::version);
    tray->setIcon(QIcon(QApplication::applicationDirPath() + "/snppictures/tray.png"));
    tray->setObjectName("normalwidget");
    tray->show();
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":"
            + QObject::tr("The Wheat Snooper version ")+about::version+QObject::tr(" started!");
    if(!S_S.getbool("cbdontshowballoons"))
        tray->showMessage(tr("Notifications."), balloonhelper.join("\n"));
}
balloon_handler::~balloon_handler()
{
    delete tray;
}
void balloon_handler::showballoon(){
    if(!S_S.getbool("cbdontshowballoons"))
        tray->showMessage(tr("Notifications."), balloonhelper.join("\n"));
    if (! (balloonhelper.size() > S_S.getint("sbmaximumballonmessages")))
        return;
    int max_size=S_S.getint("sbmaximumballonmessages");
    balloonhelper = balloonhelper.mid(balloonhelper.size()- max_size);
}
void balloon_handler::disconnected(){
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":" + tr("Disconnected from Wormnet");
    showballoon();
}
void balloon_handler::connected(){
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":" + tr("Connected to Wormnet");
    showballoon();
}
void balloon_handler::connectedtoleagueserver(QString servicename){
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":" + tr("Connected to %1").arg(servicename);
    showballoon();
}

void balloon_handler::buddyleft() {
    if (!S_S.getbool("chbbuddyballoonleaves"))
        return;
    balloonhelper << usermodel::buddylefthelper;
    usermodel::buddylefthelper.clear();
    showballoon();
}
void balloon_handler::buddyarrived() {
    if (!S_S.getbool("chbbuddyballoonarives"))
        return;
    balloonhelper << usermodel::buddyarrivedhelper;
    usermodel::buddyarrivedhelper.clear();
    showballoon();
}
void balloon_handler::got_privmsg(const QString user, const QString msg){
    if(!S_S.getbool("chbballoonprivmsg"))
        return;
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":" + user + tr(" said: ") + msg;
    showballoon();
}
void balloon_handler::got_game(const QString playername, const QString gamename){
    if (gamename.size() > 5) {
        balloonhelper
                << QTime::currentTime().toString("hh:mm") + ":"
                + playername + " " + tr("is Hosting: ") + gamename.left(5) + "...";
    } else
        balloonhelper<< QTime::currentTime().toString("hh:mm") + ":"
                + playername + " " + tr("is Hosting: ") + gamename.left(5);
    showballoon();
}
void balloon_handler::got_costum_word(const QString word, const QString user){
    if (!S_S.getbool("cbcostumword"))
        return;
    balloonhelper<< QTime::currentTime().toString("hh:mm") + ":"
            +"... " +  word + " ..." + tr("was highlighted by ")+ user;
    showballoon();
}
void balloon_handler::hide_tray(){
    tray->hide();
    tray->disconnect();
}
void balloon_handler::set_normal_tray_icon(){
    tray->setIcon(QIcon(QApplication::applicationDirPath()
                        + QDir::separator() + "snppictures"
                        + QDir::separator() + "tray.png"));
}
void balloon_handler::set_away_tray_icon(){
    tray->setIcon(QIcon(QApplication::applicationDirPath() + QDir::separator()
                        + "snppictures" + QDir::separator() + "trayaway.png"));
}
