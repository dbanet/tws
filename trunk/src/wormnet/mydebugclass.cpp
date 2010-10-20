#include "mydebugclass.h"
#include"mainwindow.h"
#include"singleton.h"
#include"settingswindow.h"
myDebugClass::myDebugClass()
{
}
myDebugClass &myDebugClass::operator<<(const QString &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;    
    singleton<mainwindow>().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QStringList &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    singleton<mainwindow>().appenddebugmessage("("+msg.join(",").simplified()+")\n");
    return *this;
}

myDebugClass &myDebugClass::operator<<(const int &i){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    singleton<mainwindow>().appenddebugmessage(QString::number(i).simplified()+"\n");
    return *this;
}
