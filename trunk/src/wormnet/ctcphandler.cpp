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

extern QMap<QString, QString> ctcpcontainer;
QStringList ctcphandler::awayusers;
ctcphandler::ctcphandler(){
    bookedcommands << "away" << "back";
}
bool ctcphandler::getctcp(const QString &user, const QString &msg) {
    QString s = QString(msg).remove(0, 1);
    s.remove("\001");
    s=s.simplified();
    if (s.startsWith("away")) {
        if (!awayusers.contains(user, Qt::CaseInsensitive))
            awayusers << user;
        emit sigctcpcommand(s, user);
    } else if (s=="back") {
        removeCI(awayusers, user);
        emit sigctcpcommand("back", user);
    } else if (s=="status") {
        if (qobjectwrapper<awayhandler>::ref().isaway)
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + user + " :\001away " + qobjectwrapper<awayhandler>::ref().awaymessage + "\001");
        else
            singleton<netcoupler>().sendrawcommand("PRIVMSG " + user + " :\001back\001");
    } else if (singleton<ctctphandlerwidget>().atomicmap.keys().contains(s)) {
        if(!singleton<ctctphandlerwidget>().atomicmap[s]->ui.cbenable->isChecked())
            return false;
        QString str =
                singleton<ctctphandlerwidget>().atomicmap[s]->ui.textEdit->toPlainText();
        singleton<netcoupler>().senduncheckedmessage(user, str);
    } else{
        return false;
    }
    return true;
}
ctcphandler::~ctcphandler() {
    // TODO Auto-generated destructor stub
}
