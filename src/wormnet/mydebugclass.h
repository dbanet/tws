#ifndef MYDEBUGCLASS_H
#define MYDEBUGCLASS_H
#include<QString>
class usermessage;
class QVariant;
class myDebugClass
{
public:
    myDebugClass();
    myDebugClass &operator<<(const char*);
    myDebugClass &operator<<(const QString&);
    myDebugClass &operator<<(const QByteArray&);
    myDebugClass &operator<<(const QStringList&);
    myDebugClass &operator<<(const int&);    
    myDebugClass &operator<<(const usermessage&);    
};

#endif // MYDEBUGCLASS_H
