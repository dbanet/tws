#ifndef BALLOON_HANDLER_H
#define BALLOON_HANDLER_H

#include <QObject>
#include<QStringList>
#include"global_macros.h"
class QSystemTrayIcon;
class balloon_handler : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(balloon_handler);
public:
        QSystemTrayIcon *tray;

signals:

public slots:
    void disconnected();
    void connected();
    void buddyarrived();
    void buddyleft();
    void got_privmsg(const QString user, const QString msg);
    void got_game(const QString playername, const QString gamename);
    void got_costum_word(const QString word,const QString user);
    void hide_tray();
    void set_normal_tray_icon();
    void set_away_tray_icon();

private:
    QStringList balloonhelper;
    void showballoon();

};

#endif // BALLOON_HANDLER_H
