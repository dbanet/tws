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

    void setaway(const QString &);
    QString awaymessage;
    bool isaway;
    bool wasaway;
    QMap<QString,QString> rememberwhogotaway;
    QString oldawaystring;
    static QString toString(){
        return "awayhandler";
    }
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
    friend class qobjectwrapper<awayhandler>;

};

#endif // AWAYHANDLER_H
