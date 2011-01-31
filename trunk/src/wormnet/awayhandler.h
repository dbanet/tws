#ifndef AWAYHANDLER_H
#define AWAYHANDLER_H

#include <QObject>
#include<QTimer>
#include<QMap>
#include"qobjectwrapper.h"

class awayhandler : public QObject
{
    Q_OBJECT
public:
    void startLookingForGame();
    bool away();
    QString message();
    void setaway(const QString &);    
    void sendaway(const QString &user);
    static QString toString(){return "awayhandler";}
    void back();    

public slots:
    void setawaywhilegameing();
private:
    explicit awayhandler(QObject *parent = 0);
    ~awayhandler();
    QTimer lookingForGameTimer;

signals:
    void siggameended();
    void sigawaystringchanged();
private slots:
    void gameTimerTimeout();
    void gamefinished();
private:
    QMap<QString,QString> rememberwhogotaway;
    QString awaymessage;
    bool isaway;
    bool wasaway;
    void sendBack();
    friend class qobjectwrapper<awayhandler>;

};

#endif // AWAYHANDLER_H
