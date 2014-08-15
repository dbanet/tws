#ifndef PLAYERNAME_H
#define PLAYERNAME_H

#include <QWidget>
#include "ui_playername.h"

class PlayerName : public QWidget
{
    Q_OBJECT

public:
    PlayerName(QWidget *parent = 0);
    ~PlayerName();
    static QString nick;
private:
    Ui::playernameClass ui;
private slots:
    void okclicked();
};

#endif // PLAYERNAME_H
