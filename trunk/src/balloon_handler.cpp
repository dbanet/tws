#include "balloon_handler.h"
#include "settingswindow.h"
#include"about.h"
#include"usermodel.h"
#include<QTime>
#include<QSystemTrayIcon>
#include<QDir>
#include<QDebug>
balloon_handler::balloon_handler()
{
    tray=new QSystemTrayIcon;
    tray->setToolTip(tr("The Wheat Snooper version ")+about::version);
    tray->setIcon(QIcon(QApplication::applicationDirPath() + QDir::separator()
                        + "snppictures" + QDir::separator() + "tray.png"));
    tray->setObjectName("normalwidget");
    tray->show();
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":"
            + "The Wheat Snooper version "+about::version+" started!";
    if(!singleton<settingswindow>().from_map("cbdontshowballoons").toBool())
        tray->showMessage(tr("Notifications."), balloonhelper.join("\n"));
}
balloon_handler::~balloon_handler()
{
    delete tray;
}
void balloon_handler::showballoon(){
    if(!singleton<settingswindow>().from_map("cbdontshowballoons").toBool())
        tray->showMessage(tr("Notifications."), balloonhelper.join("\n"));

    if (! (balloonhelper.size() > singleton<settingswindow>().from_map("sbmaximumballonmessages").toInt()))
        return;
    int max_size=singleton<settingswindow>().from_map("sbmaximumballonmessages").toInt();
    balloonhelper = balloonhelper.mid(balloonhelper.size()- max_size);
}
void balloon_handler::disconnected(){
    this->balloonhelper << QTime::currentTime().toString("hh:mm") + ":"
            + "disconnected from Network";
    showballoon();
}
void balloon_handler::connected(){
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":"
            + "connected to Network";
    showballoon();
}
void balloon_handler::buddyleft() {
    if (!singleton<settingswindow>().from_map("chbbuddyballoonleaves").toBool())
        return;
    balloonhelper << usermodel::buddylefthelper;
    usermodel::buddylefthelper.clear();
    showballoon();
}
void balloon_handler::buddyarrived() {
    if (!singleton<settingswindow>().from_map("chbbuddyballoonarives").toBool())
        return;
    balloonhelper << usermodel::buddyarrivedhelper;
    usermodel::buddyarrivedhelper.clear();
    showballoon();
}
void balloon_handler::got_privmsg(const QString user, const QString msg){
    if(!singleton<settingswindow>().from_map("chbballoonprivmsg").toBool())
        return;
    balloonhelper << QTime::currentTime().toString("hh:mm") + ":"
            + user + tr(" said: ") + msg;
    showballoon();
}
void balloon_handler::got_game(const QString playername, const QString gamename){
    if (gamename.size() > 5) {
        balloonhelper
                << QTime::currentTime().toString("hh:mm") + ":"
                + playername + " " + tr(
                        "is Hosting: ") + gamename.left(5)
                + "...";
    } else
        balloonhelper
                << QTime::currentTime().toString("hh:mm") + ":"
                + playername + " " + tr(
                        "is Hosting: ") + gamename.left(5);
    showballoon();
}
void balloon_handler::got_costum_word(const QString word, const QString user){
    if (!singleton<settingswindow>().from_map("cbcostumword").value<bool> ())
        return;
    balloonhelper
            << QTime::currentTime().toString("hh:mm") + ":"
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
