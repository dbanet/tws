#ifndef CLANTOWEBPAGEMAPPER_H
#define CLANTOWEBPAGEMAPPER_H
#include "global_macros.h"
#include"userstruct.h"
#include<QHash>
#include<QObject>
#include<QNetworkAccessManager>
class QNetworkReply;
class clantowebpagemapper: public QObject{
    Q_OBJECT
public:
    bool contains(const QString &key);
    QString value(const QString &key);
    void refresh();
    QString getInformation(const userstruct &u);
private slots:
    void httpFinished();
    void httpReadyRead();
private:
    QHash<QString,QString> InformationMap;
    QHash<QString,QString> map;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QString file;

    DECLARE_SINGLETON(clantowebpagemapper);
};

#endif // CLANTOWEBPAGEMAPPER_H
