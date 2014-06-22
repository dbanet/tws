#include"mydebugclass.h"
#include"mainwindow.h"
#include"singleton.h"
#include"settingswindow.h"
#include"qobjectwrapper.h"
#include"usermessage.h"

#include<QDebug>

myDebugClass::myDebugClass()
{
}
myDebugClass &myDebugClass::operator<<(const char *msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QString &msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QByteArray &msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString(msg).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const QStringList &msg){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<msg;
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage("("+msg.join(",").simplified()+")\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const int &i){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<i;
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(QString::number(i).simplified()+"\n");
    return *this;
}
myDebugClass &myDebugClass::operator<<(const usermessage &u){
    if(qobjectwrapper<mainwindow>::isNull()){
        qDebug()<<u.msg();
        qDebug()<<u.type();
        qDebug()<<u.user();
        qDebug()<<u.receiver();
        return *this;
    }
    qobjectwrapper<mainwindow>::ref().appenddebugmessage(u.msg()+"\n"+QString::number(u.type())+"\n"+u.user()+"\n"+u.receiver()+"\n");
    return *this;
}
