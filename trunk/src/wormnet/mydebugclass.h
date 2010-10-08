#ifndef MYDEBUGCLASS_H
#define MYDEBUGCLASS_H
#include<QString>
class mainwindow;
class myDebugClass
{
public:
    myDebugClass();
    myDebugClass &operator<<(const QString&);
    static mainwindow *w;
};

#endif // MYDEBUGCLASS_H
