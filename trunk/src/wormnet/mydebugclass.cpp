#include"mydebugclass.h"
#include"mainwindow.h"
#include"singleton.h"
#include"settingswindow.h"
#include"qobjectwrapper.h"

myDebugClass::myDebugClass()
{
}
myDebugClass &myDebugClass::operator<<(const QString &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    if(qobjectwrapper<mainwindow>::isNull())
        return *this;
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QStringList &msg){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    qobjectwrapper<mainwindow>::ref().appenddebugmessage("("+msg.join(",").simplified()+")\n");
    return *this;
}

myDebugClass &myDebugClass::operator<<(const int &i){
    if(!singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
        return *this;
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString::number(i).simplified()+"\n");
    return *this;
}
