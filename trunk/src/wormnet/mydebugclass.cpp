#include "mydebugclass.h"
#include"mainwindow.h"
#include"singleton.h"
#include"settingswindow.h"
mainwindow *myDebugClass::w=NULL;
myDebugClass::myDebugClass()
{
}
myDebugClass &myDebugClass::operator<<(const QString &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    if (w != 0)
        w->appenddebugmessage(QString(msg).simplified()+"\n");
    else
        mainwindow::debugmsg.append(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QStringList &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    if (w != 0)
        w->appenddebugmessage("("+msg.join(",").simplified()+")\n");
    else
        mainwindow::debugmsg.append("("+msg.join(",").simplified()+")\n");
    return *this;
}

myDebugClass &myDebugClass::operator<<(const int &i){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    if (w != 0)
        w->appenddebugmessage(QString::number(i).simplified()+"\n");
    else
        mainwindow::debugmsg.append(QString::number(i).simplified()+"\n");
    return *this;
}
