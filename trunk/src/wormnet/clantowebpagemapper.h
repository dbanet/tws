#ifndef CLANTOWEBPAGEMAPPER_H
#define CLANTOWEBPAGEMAPPER_H
#include "global_macros.h"
#include<QMap>
#include<QObject>
#include<QNetworkAccessManager>
class QNetworkReply;
class clantowebpagemapper: public QObject
{
    Q_OBJECT
public:
    void load();
    void safe();
    void loadDefaults();
    bool contains(const QString &key);
    QString value(const QString &key);
    void refresh();
    QString getInformation(QString key);
private slots:
    void httpFinished();
    void httpReadyRead();
private:
    QMap<QString,QString> InformationMap;
    QMap<QString,QString> map;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QString file;

    DECLARE_SINGLETON(clantowebpagemapper);
};

#endif // CLANTOWEBPAGEMAPPER_H
