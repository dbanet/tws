#ifndef MYNETWORKREPLY_H
#define MYNETWORKREPLY_H

#include <QNetworkReply>

class mynetworkreply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit mynetworkreply(QObject *parent = 0);
    void setError(NetworkError errorCode, const QString & errorString="");

signals:

public slots:

};

#endif // MYNETWORKREPLY_H
