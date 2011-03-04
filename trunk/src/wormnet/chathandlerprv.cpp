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
    isprv=1;
    disconnect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(contextrequest(const QPoint&)));
    connect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(opencontextmenu(const QPoint&)));
}
void chathandlerprv::opencontextmenu(const QPoint &p){
    QTextCharFormat format=tb->cursorForPosition(p).charFormat();
    int whatsthis=format.intProperty(whatsthispropertyId);
    if(whatsthis==e_hash_wa)
        get_new_font_and_color_with_walink(&format);
    else
        get_new_font_and_color(&format);
}
QList<QPair<QVariant, QTextCharFormat> > chathandlerprv::getSegmentation(QString s, QTextCharFormat format){
    QStringList sl=s.split(" ", QString::SkipEmptyParts);
    QList<QPair<QVariant, QTextCharFormat> > text;
    foreach(s,sl){
        if(isClickableLink(s)){
            hash[e_hash_http].setAnchorHref(s);
            text<<makepair(s, hash[e_hash_http]);
        } else if(startswithCI(s, "wa://")){
            hash[e_hash_wa].setAnchorHref(s);
            hash[e_hash_wa].setProperty(linkpropertyId, s);
            text<<makepair(tr("GAMELINK"), hash[e_hash_wa]);
        } else {
            if(S_S.getbool("chbsmileysinchatwindows"))
                text<<makepair(emot->contains(s),format);
            else
                text<<makepair(s,format);
        }
    }
    return text;
}
QTextCharFormat chathandlerprv::getRightFormat(const usermessage u){
    QTextCharFormat format;    
    if (!containsCI(S_S.buddylist, u.user()))
        format = hash[e_hash_chat];
    else
        format = hash[e_hash_buddy];
    if(u.user()==singleton<netcoupler>().nick)
        format=hash[e_hash_myself];
    if(u.has_type(e_PRIVMSG) && u.has_type(e_ACTION))
        format=hash[e_hash_action];
    else if(u.has_type(e_CTCP))
        format=hash[e_hash_ctcp];
    else if(u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION))
            format=hash[e_hash_noticeaction];
        else
            format=hash[e_hash_notice];
    }else if(u.has_type(e_RAWCOMMAND))
        format=hash[e_hash_raw];
    return format;
}
chathandlerprv::~chathandlerprv() {
}
