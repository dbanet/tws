/*
 * chathandler.cpp
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#include<QTextBrowser>
#include<QTextDocument>
#include<QTime>
#include<QFontDialog>
#include<QColorDialog>
#include<QScrollBar>
#include<QDesktopServices>
#include<QMessageBox>
#include<QPointer>
#include<QWheelEvent>
#include<stdexcept>

#include"chathandler.h"
#include"charformatsettings.h"
#include"settings.h"
#include"netcoupler.h"
#include"joinprvgame.h"
#include"settingswindow.h"
#include"charformatsettings.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"balloon_handler.h"
#include"usermessage.h"
#include"emoticonhandler.h"

int chathandler::whatsthispropertyId=0x100000;
int chathandler::userpropertyId=0x100000+1;
int chathandler::linkpropertyId=0x100000+2;

QPointer<emoticonhandler> chathandler::emot;
QHash<int, QTextCharFormat> chathandler::hash;

extern bool fontorcolorchanged;
chathandler::chathandler(QObject *parent, QTextBrowser *t, QString chan) :        
        QObject(parent), channel(chan), slideratmaximum(true),gotFirstMessage(false), tb(t) {
    emot = new emoticonhandler;
    tb->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tb->setContextMenuPolicy(Qt::CustomContextMenu);
    tb->setOpenLinks(0);
    tb->setOpenExternalLinks(0);
    tb->installEventFilter(this);
    doc = tb->document();
    usesettingswindow("sbmaximumoftextblocks");
    cursor = new QTextCursor(doc);   
    initialformatstarter();
    connect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(contextrequest(const QPoint&)));
    connect(tb, SIGNAL(anchorClicked (const QUrl&)),this, SLOT(anchorclicked(const QUrl&)));

    initialformatstarter();
    QAction *a;
    fontmenu.addAction(tr("Set the font for this texttype"));
    fontmenu.addAction(tr("Set the color for this texttype"));

    a = chatmenu.addAction(tr("Chat with this user"));
    a->setIcon(QIcon("snppictures/Chat_Icon.png"));
    chatmenu.addSeparator();
    chatmenu.addAction(tr("Set the font for this texttype"));
    chatmenu.addAction(tr("Set the color for this texttype"));

    wamenu.addAction(tr("Play this game."));
    wamenu.addAction(tr("Show game info."));
    wamenu.addSeparator();
    wamenu.addAction(tr("Set the font for this texttype"));
    wamenu.addAction(tr("Set the color for this texttype"));

    connect(tb->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(slidermoved(int)));
    connect(&singleton<netcoupler>(), SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(tb,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
}
bool chathandler::eventFilter(QObject *obj, QEvent *event){                 
    if(obj==tb && event->type()==QEvent::Wheel)
        slidermoved(tb->verticalScrollBar()->value());
    else if(obj->metaObject()->className()==QString("QLineEdit") && event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
        if (keyEvent->key() == Qt::Key_Up)
            qApp->postEvent(obj,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ControlModifier));
        else if (keyEvent->key() == Qt::Key_Down)
            qApp->postEvent(obj,new QKeyEvent(QEvent::KeyPress,Qt::Key_Z,Qt::ShiftModifier | Qt::ControlModifier));
    }
    return false;
}

void chathandler::initialformatstarter() {
    for(int i=0;i<18;i++){
        hash[i].setProperty(whatsthispropertyId, i);
        initialformat(hash[i]);
    }
}
void chathandler::initialformatsaver() {
    singleton<charformatsettings>().safe();
}
QStringList formatstrings=QStringList()<<"nick"<<"myself"<<"garbage"<<"time"<<"garbagejoin" <<"garbagepart"
                          <<"garbagequit" <<"chat" <<"action" <<"buddy" <<"debug" <<"ctcp" <<"raw"
                          <<"prv"<<"notice"<<"noticeaction"<<"wa"<<"http";
void chathandler::initialformat(QTextCharFormat &format){                                   //provisorium TODO
    QString font=formatstrings[format.intProperty(whatsthispropertyId)]+"formatfont";
    QString color=formatstrings[format.intProperty(whatsthispropertyId)]+"formatcolor";

    if (singleton<charformatsettings>().map.contains(font))
        format.setFont(singleton<charformatsettings>().map[font].value<QFont> ());
    else
        format.setFont(QFont());
    if (singleton<charformatsettings>().map.contains(color))
        format.setForeground(singleton<charformatsettings>().map[color].value<QColor> ());
    else
        format.setForeground(QColor());
}
void chathandler::anchorclicked(const QUrl &u) {
    QString s=u.toString();
    if (isClickableLink(s)) {
        s.remove("\n");
        s.remove("\r");
        QUrl u1;
        u1.setUrl(s);
        QDesktopServices::openUrl(u1);
    }
}
void chathandler::append(const usermessage &u){
    typedef QPair<QVariant, QTextCharFormat> pair;
    QList<pair> text;
    QString time = QTime::currentTime().toString("hh:mm");
    text<<makepair(time + ":", hash[e_time]);
    QTextCharFormat format=getRightFormat(u);
    format.setProperty(userpropertyId,u.user());
    hash[e_nick].setProperty(userpropertyId,u.user());
    QString suffix;
    if(u.has_type(e_CTCP)) {
        text<<makepair(u.user() + " CTCP: ",hash[e_nick]);
    }
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
//void chathandler::append(const QString &user, const QString &receiver,
//                         const QString &msg) {
//    QString msgtemp = msg;
//    msgtemp.remove("\r");
//    msgtemp.remove("\n");
//    QString time = QTime::currentTime().toString("hh:mm");
//    cursor->insertText(time + ": ", hash[e_time]);
//    if (!containsCI(S_S.buddylist, user)) {
//        if (compareCI(receiver, channel)) {
//            if (msg.startsWith("\001ACTION")) {
//                msgtemp.remove(0, 7).remove("\001");
//                insertText("<" + user + " " + msgtemp + +">", hash[e_action],user);
//            } else {
//                hash[e_nick].setAnchorHref("<nick> " + user);
//                cursor->insertText(user + "> ", hash[e_nick]);
//                if(user==singleton<netcoupler>().nick)
//                    insertText(msgtemp, hash[e_myself],user);
//                else
//                    insertText(msgtemp, hash[e_chat],user);
//            }
//        } else {
//            if (msg.startsWith("\001ACTION")) {
//                msgtemp.remove(0, 7).remove("\001");
//                insertText("<" + user + " " + msgtemp + ">", hash[e_action],user);
//            } else {
//                hash[e_prv].setAnchorHref("<prv> " + user);
//                cursor->insertText(user + " to " + receiver + ">", hash[e_prv]);
//                insertText(msgtemp, hash[e_prv],user);
//            }
//        }
//    } else {
//        if (msg.startsWith("\001ACTION")) {
//            msgtemp.remove(0, 7).remove("\001");
//            insertText("<" + user + " " + msgtemp + ">", hash[e_buddy],user);
//        } else {
//            hash[e_nick].setAnchorHref("<nick> " + user);
//            cursor->insertText(user + "> ", hash[e_nick]);
//            if(user==singleton<netcoupler>().nick)
//                insertText(msgtemp, hash[e_myself],user);
//            else
//                insertText(msgtemp, hash[e_buddy],user);
//        }
//    }
//    cursor->insertText("\n");
//    if (slideratmaximum)
//        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
//}
void chathandler::appendnotice(const QString &user, const QString &receiver,
                               const QString &msg) {
    QString msgtemp = msg;
    msgtemp.remove("\r");
    msgtemp.remove("\n");
    QString time = QTime::currentTime().toString("hh:mm");
    cursor->insertText(time + ":", hash[e_time]);
    hash[e_notice].setAnchorHref("<notice> " + user);
    if (msg.startsWith("\001ACTION")) {
        msgtemp.remove(0, 7).remove("\001");
        insertText(tr("Notice from") + " " + user + " " + tr("to") + " "
                   + receiver + "> <" + msgtemp + ">", hash[e_notice],user);
    } else {
        insertText(tr("Notice from") + " " + user + " " + tr("to") + " "
                   + receiver + "> " + msgtemp.remove("\001") + " ", hash[e_notice],user);
    }
    cursor->insertText("\n");
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appenddebug(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_debug]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendjoingarbage(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_garbagejoin]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendpartgarbage(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_garbagepart]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendquitgarbage(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_garbagequit]);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
QList<QPair<QVariant, QTextCharFormat> > chathandler::getSegmentation(QString s, QTextCharFormat format){
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
            if(S_S.getbool("showsmileysinchannels"))
                text<<makepair(emot->contains(s),format);
            else
                text<<makepair(s,format);
        }
    }
    return text;
}
QTextCharFormat chathandler::getRightFormat(const usermessage &u){
    QTextCharFormat format;
    if (!containsCI(S_S.buddylist, u.user()))
        format = hash[e_chat];
    else
        format = hash[e_buddy];
    if(u.user()==singleton<netcoupler>().nick)
        format=hash[e_myself];
    if(u.has_type(e_PRIVMSG) && u.has_type(e_ACTION)){
        format=hash[e_action];
    }
    else if(u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION))
            format=hash[e_noticeaction];
        else
            format=hash[e_notice];
    }
    else if(u.has_type(e_CTCP))
        format=hash[e_ctcp];
    else if(u.has_type(e_RAWCOMMAND))
        format=hash[e_raw];
    return format;
}
void chathandler::insertText(const QString &s, QTextCharFormat &t,QString user) {
    QString temp;
    if (containsClickableLink(s) || containsCI(s, "wa://")) {
        QStringList sl = s.split(" ", QString::SkipEmptyParts);
        foreach(QString str,sl) {
            QString strtemp = str;
            str = str + " ";
            if (isClickableLink(str)) {
                hash[e_http].setAnchorHref(strtemp);
                cursor->insertText(str, hash[e_http]);
            } else if (startswithCI(str, "wa://")) {
                hash[e_wa].setAnchorHref(str);
                cursor->insertText(tr("GAMELINK"), hash[e_wa]);
            } else {
                cursor->insertText(str, t);
            }
        }
    } else if (containsCI(s, singleton<netcoupler>().nick)) {
        if (!t.anchorHref().startsWith("<notice>") && t.anchorHref() != "action")
            singleton<sound_handler>().play_highlightningsound(user,qobject_cast<QWidget*> ( parent()));
        cursor->insertText(s, t);
    } else if (containsOneCI(s, S_S.combobox_wrapper,&temp)) {
        singleton<sound_handler>().play_costumwordsound(user,qobject_cast<QWidget*> ( parent()));
        singleton<balloon_handler>().got_costum_word(temp,user);
        cursor->insertText(s, t);
    } else
        cursor->insertText(s, t);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::slidermoved(int i) {
    if(!gotFirstMessage)
    {        
        gotFirstMessage=true;
        return;
    }
    if (i == tb->verticalScrollBar()->maximum())
        slideratmaximum = 1;
    else
        slideratmaximum = 0;    
}
//void chathandler::slidermovedbyaction(int i) {
//    if (i > 0 && i < 5) {
//        if (tb->verticalScrollBar()->value() == tb->verticalScrollBar()->maximum())
//            slideratmaximum = 1;
//        else {
//            slideratmaximum = 0;
//        }
//    }
//}
void chathandler::setschememap(QTextCharFormat *c, QFont f) {
    singleton<charformatsettings>().map[c->stringProperty(whatsthispropertyId)+"font"] = f;
    c->setFont(f);
    fontorcolorchanged = 1;
}
void chathandler::setschememap(QTextCharFormat *c, QColor f) {
    if (f.isValid()) {
        singleton<charformatsettings>().map[c->stringProperty(whatsthispropertyId)+"color"] = f;
        c->setForeground(f);
        fontorcolorchanged = 1;
    }
}
void chathandler::setschememap(int i,QFont f){
    singleton<charformatsettings>().map[formatstrings[i]+"formatfont"] = f;
    hash[i].setFont(f);
    fontorcolorchanged = 1;
}
void chathandler::setschememap(int i,QColor c){
    if (!c.isValid())
        return;
    singleton<charformatsettings>().map[formatstrings[i]+"formatcolor"] = c;
    hash[i].setForeground(c);
    fontorcolorchanged = 1;
}
void chathandler::get_new_font_and_color(QTextCharFormat *format){
    QAction *a = fontmenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool *b = new bool;
            QFont f = QFontDialog::getFont(b, format->font(), 0);
            if (*b)
                setschememap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setschememap(format->intProperty(whatsthispropertyId), f);
        }
    }
}
void chathandler::get_new_font_and_color_with_chatwindow(QTextCharFormat *format){
    QAction *a = chatmenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool *b = new bool;
            QFont f = QFontDialog::getFont(b, format->font(), tb);
            if (*b)
                setschememap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setschememap(format->intProperty(whatsthispropertyId), f);
        } else
            emit sigopenchatwindow(format->stringProperty(userpropertyId));
    }
}
void chathandler::get_new_font_and_color_with_walink(QTextCharFormat *format){
    QAction *a = wamenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool *b = new bool;
            QFont f = QFontDialog::getFont(b, format->font(), tb);
            if (*b)
                setschememap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setschememap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Show game info.")) {
            QMessageBox::information(0,tr("Info about this gamelink."), format->anchorHref());
        } else {
            joinprvgame *prv = new joinprvgame(format->anchorHref(), channel);
            prv->show();
            connect(prv, SIGNAL(sigjoingamelink(const QString&)),&singleton<netcoupler>(), SLOT(joingamelink(const QString&)));
        }
    }
}
void chathandler::contextrequest(const QPoint &p) {
    QTextCharFormat format=tb->cursorForPosition(p).charFormat();
    int whatsthis=format.intProperty(whatsthispropertyId);
    if(whatsthis==e_wa)
        get_new_font_and_color_with_walink(&format);
    else if(whatsthis == e_prv || whatsthis == e_action || whatsthis == e_nick || whatsthis == e_notice || whatsthis == e_noticeaction || whatsthis == e_chat)
        get_new_font_and_color_with_chatwindow(&format);
    else
        get_new_font_and_color(&format);
}
void chathandler::usesettingswindow(const QString &s) {
    if (s == "sbmaximumoftextblocks" || s == "") {
        int i = S_S.getint("sbmaximumoftextblocks");
        doc->setMaximumBlockCount(i);
    }
}
chathandler::~chathandler() {
    // TODO Auto-generated destructor stub
}
void chathandler::selectionChanged(){
    tb->copy();
}
void chathandler::moveSliderToMaximum(){
    tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
    slideratmaximum=true;
}
QPair<QVariant, QTextCharFormat> chathandler::makepair(QVariant v, QTextCharFormat format){
    return qMakePair<QVariant, QTextCharFormat>(v,format);
}
