/*
 * ctcphandler.cpp
 *
 *  Created on: 13.01.2009
 *      Author: looki
 */
#include"ctcphandler.h"
#include"netcoupler.h"
#include"chatwindow.h"
#include"ctcphandleratomic.h"
#include"global_functions.h"
#include"qobjectwrapper.h"
#include"awayhandler.h"
#include"usermessage.h"
#include"window.h"

extern QMap<QString, QString> ctcpcontainer;
QStringList ctcphandler::awayusers;
ctcphandler::ctcphandler(){
    bookedcommands << "away" << "back";
}
bool ctcphandler::getctcp(const usermessage u){
    if (u.msg().startsWith("away")) {
        if (!awayusers.contains(u.user(), Qt::CaseInsensitive)){
            awayusers << u.user();
            foreach(chatwindow *w, ::window::chatwindows){
                if(w->chatpartner == u.user()){
                    w->setaway(1,u.msg());
                    break;
                }
            }
        }
    } else if (u.msg()=="back") {
        foreach(chatwindow *w, ::window::chatwindows){
            if(w->chatpartner == u.user()){
                w->setaway(0);
                break;
            }
        }
        removeCI(awayusers, u.user());
    } else if (singleton<ctctphandlerwidget>().atomicmap.keys().contains(u.msg())) {
        if(!singleton<ctctphandlerwidget>().atomicmap[u.msg()]->ui.cbenable->isChecked())
            return false;
        QString s =singleton<ctctphandlerwidget>().atomicmap[u.msg()]->ui.textEdit->toPlainText();
        usermessage uu=usermessage::create(s, u.receiver(), u.user());
        singleton<netcoupler>().sendusermessage(uu);
    } else
        return false;
    return true;
}

ctcphandler::~ctcphandler() {}
