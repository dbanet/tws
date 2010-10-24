#include "mynetworkreply.h"

mynetworkreply::mynetworkreply(QObject *parent) :
    QNetworkReply(parent)
{
}
void mynetworkreply::setError(NetworkError errorCode, const QString & errorString){
    QNetworkReply::setError(errorCode,errorString);
}
