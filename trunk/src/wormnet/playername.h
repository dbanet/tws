#ifndef PLAYERNAME_H
#define PLAYERNAME_H

#include <QtGui/QWidget>
#include "ui_playername.h"

class playername : public QWidget
{
    Q_OBJECT

public:
    playername(QWidget *parent = 0);
    ~playername();
    static QString nick;
private:
    Ui::playernameClass ui;
private slots:
    void okclicked();
};

#endif // PLAYERNAME_H
