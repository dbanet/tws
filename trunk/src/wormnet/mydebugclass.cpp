#include"mydebugclass.h"
#include"mainwindow.h"
#include"singleton.h"
#include"settingswindow.h"
#include"qobjectwrapper.h"
#include"QDebug"

myDebugClass::myDebugClass()
{
}
myDebugClass &myDebugClass::operator<<(const QString &msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    if(!S_S.cbservermessageinchannelwindows)
        return *this;    
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QStringList &msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    if(!S_S.cbservermessageinchannelwindows)
        return *this;    
    qobjectwrapper<mainwindow>::ref().appenddebugmessage("("+msg.join(",").simplified()+")\n");
    return *this;
}

myDebugClass &myDebugClass::operator<<(const int &i){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<i;
        return *this;
    }
    if(!S_S.cbservermessageinchannelwindows)
        return *this;    
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString::number(i).simplified()+"\n");
    return *this;
}
