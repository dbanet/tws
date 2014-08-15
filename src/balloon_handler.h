#ifndef BALLOON_HANDLER_H
#define BALLOON_HANDLER_H

#include <QObject>
#include <QStringList>
#include "global_macros.h"

class usermessage;
class QSystemTrayIcon;

class balloonHandler : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(balloonHandler)
public:
        QSystemTrayIcon *tray;

signals:

public slots:
    void disconnected();
    void connected();
    void connectedToLeagueServer(QString servicename);
    void buddyArrived();
    void buddyLeft();
    void gotPrivmsg(const usermessage &u);
    void gotGame(const QString playername, const QString gamename);
    void gotCustomWord(const QString word,const QString user);
    void hideTray();
    void setNormalTrayIcon();
    void setAwayTrayIcon();
    void alert(QString user, QWidget *w);

private:
    QStringList balloonHelper;
    void showBalloon();

};

#endif // BALLOON_HANDLER_H
