#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QString>
#include <QStringList>

class ircMessage
{
public:
    ircMessage(QString);
    bool isUserMsg();
    bool isServMsg();
    bool hasTrailing();
    QString prefix;
    QString command;
    QStringList paramList;
    QString trailing;
private:
    QString message;
    enum msgType{
        SERVER,
        USER
    };
    msgType type;
    bool hasTrailPart;
};

#endif // IRCMESSAGE_H
