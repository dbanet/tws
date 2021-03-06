#ifndef HOSTBOX_H
#define HOSTBOX_H

#include <QWidget>
#include "ui_hostbox.h"
class netcoupler;
class hostbox : public QWidget
{
    Q_OBJECT

public:
    hostbox(netcoupler *netc,QString,QWidget *parent = 0);
    ~hostbox();
    QString gamename;
    QString pwd;
    QString icon;
    const QString channel;
private:
    netcoupler *netc;
    Ui::hostboxClass ui;
    QRegExpValidator *validator;

    bool verifywormnat();
private slots:  
    void addclicked();
    void okclicked();
    void cancelclicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent ( QShowEvent * event );
signals:
    void sigok();
};

#endif // HOSTBOX_H
