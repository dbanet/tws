/*
 * chathandler.cpp
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#include<QTextBrowser>
#include<QTextDocument>
#include<QTime>
#include<QColorDialog>
#include<QScrollBar>
#include<QMessageBox>
#include<QDesktopServices>
#include<QPixmap>

#include"chathandlerprv.h"
#include"settingswindow.h"
#include"settings.h"
#include"netcoupler.h"
#include"joinprvgame.h"
#include"global_functions.h"
#include"usermessage.h"

chathandlerprv::chathandlerprv(QObject *parent, QTextBrowser *t, QString chan) :
        chathandler(parent, t, chan) {              
    disconnect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(contextrequest(const QPoint&)));
    connect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(opencontextmenu(const QPoint&)));
}
void chathandlerprv::opencontextmenu(const QPoint &p){
    QTextCharFormat format=tb->cursorForPosition(p).charFormat();
    int whatsthis=format.intProperty(whatsthispropertyId);
    if(whatsthis==e_wa)
        get_new_font_and_color_with_walink(&format);
    else
        get_new_font_and_color(&format);
}
QList<QPair<QVariant, QTextCharFormat> > chathandlerprv::getSegmentation(QString s, QTextCharFormat format){
    QStringList sl=s.split(" ", QString::SkipEmptyParts);
    QList<QPair<QVariant, QTextCharFormat> > text;
    foreach(s,sl){
        if(isClickableLink(s)){
            hash[e_http].setAnchorHref(s);
            text<<makepair(s, hash[e_http]);
        } else if(startswithCI(s, "wa://")){
            hash[e_wa].setAnchorHref(s);
            hash[e_wa].setProperty(linkpropertyId, s);
            text<<makepair(tr("GAMELINK"), hash[e_wa]);
        } else {
            if(S_S.getbool("chbsmileysinchatwindows"))
                text<<makepair(emot->contains(s),format);
            else
                text<<makepair(s,format);
        }
    }
    return text;
}
void chathandlerprv::append(const usermessage &u){
    typedef QPair<QVariant, QTextCharFormat> pair;
    QList<pair> text;
    QString time = QTime::currentTime().toString("hh:mm");
    text<<makepair(time + ":", hash[e_time]);
    QTextCharFormat format=getRightFormat(u);
    format.setProperty(userpropertyId,u.user());
    QString suffix;
    if(u.has_type(e_CTCP))
        text<<makepair(u.user() + " CTCP: ",hash[e_nick]);
    else if(u.has_type(e_RAWCOMMAND))
        text<<makepair(u.user() + " RAW: ",hash[e_nick]);
    else if (u.has_type(e_PRIVMSG)){
        if(u.has_type(e_ACTION)){
            text<<makepair("< " + u.user() + " ",format);
            suffix=">";
        }
        else
            text<<makepair(u.user()+">",hash[e_nick]);
    } else if(u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION)){
            text<<makepair("<<< " + u.user() + " ",format);
            suffix=">>>";
        }
        else{
            text<<makepair("<< " + u.user() + " ",format);
            suffix=">>";
        }
    }
    text<<getSegmentation(u.msg(), format);
    text<<makepair(suffix,format);

    foreach(pair p, text){
        if(p.first.type() == QVariant::String) {
            cursor->insertText(p.first.toString()+ " ",p.second);
        }
        else if(p.first.type() == QVariant::Image){
            cursor->insertImage(p.first.value<QImage>());
            cursor->insertText(" ");
        } else
            myDebug()<<"##################void chathandlerprv::append(const usermessage &u)";
    }
    cursor->insertText("\n");
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandlerprv::appendgarbage(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_garbage]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandlerprv::appendgarbage(const usermessage &u){
    QString s;
    if(u.has_type(e_PRIVMSG))
        s=QDate::currentDate().toString("dd.MM") + " " + QTime::currentTime().toString("hh:mm") + " ";
    else
        s=QDate::currentDate().toString("dd.MM") + " " + QTime::currentTime().toString("hh:mm") + "NOTICE : ";
    s+=u.user() + ">" + u.msg().simplified();
    cursor->insertText(s + "\n", hash[e_garbage]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
chathandlerprv::~chathandlerprv() {
}
