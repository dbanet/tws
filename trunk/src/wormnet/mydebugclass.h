#ifndef MYDEBUGCLASS_H
#define MYDEBUGCLASS_H
#include<QString>
class myDebugClass
{
public:
    myDebugClass();
    myDebugClass &operator<<(const QString&);
    myDebugClass &operator<<(const QStringList&);
    myDebugClass &operator<<(const int&);    
};

#endif // MYDEBUGCLASS_H
