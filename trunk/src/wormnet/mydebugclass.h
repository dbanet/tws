#ifndef MYDEBUGCLASS_H
#define MYDEBUGCLASS_H
#include<QString>
class usermessage;
class myDebugClass
{
public:
    myDebugClass();
    myDebugClass &operator<<(const QString&);
    myDebugClass &operator<<(const QStringList&);
    myDebugClass &operator<<(const int&);    
    myDebugClass &operator<<(const usermessage&);
};

#endif // MYDEBUGCLASS_H
