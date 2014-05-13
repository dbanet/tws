/*
 * chathandler.cpp
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#include <QTextBrowser>
#include <QTextDocument>
#include <QTime>
#include <QFontDialog>
#include <QColorDialog>
#include <QScrollBar>
#include <QDesktopServices>
#include <QMessageBox>
#include <QPointer>
#include <QWheelEvent>
#include <stdexcept>

#include "chathandler.h"
#include "charformatsettings.h"
#include "settings.h"
#include "netcoupler.h"
#include "joinprvgame.h"
#include "settingswindow.h"
#include "charformatsettings.h"
#ifdef PHONON
#include "sound_handler.h"
#endif
#include "global_functions.h"
#include "balloon_handler.h"
#include "usermessage.h"
#include "emoticonhandler.h"

int chatHandler::whatsthispropertyId=0x100000;
int chatHandler::userpropertyId=0x100000+1;
int chatHandler::linkpropertyId=0x100000+2;

QPointer<emoticonhandler> chatHandler::emot;
QHash<int, QTextCharFormat> chatHandler::hash;

extern bool fontOrColorHasChanged;
chatHandler::chatHandler(QObject *parent, QTextBrowser *t, QString chan) :
    QObject(parent), sliderAtMaximum(true), gotFirstMessage(false), tb(t), chatpartner(chan) {
    isprv=0;
    emot = new emoticonhandler;
    tb->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tb->setContextMenuPolicy(Qt::CustomContextMenu);
    tb->setOpenLinks(0);
    tb->setOpenExternalLinks(0);
    tb->installEventFilter(this);
    doc = tb->document();
    useSettingsWindow("sbmaximumoftextblocks");
    cursor = new QTextCursor(doc);   
    initialFormatStarter();
    connect(tb, SIGNAL(customContextMenuRequested ( const QPoint &)),this, SLOT(contextrequest(const QPoint&)));
    connect(tb, SIGNAL(anchorClicked (const QUrl&)),this, SLOT(anchorClicked(const QUrl&)));

    initialFormatStarter();
    QAction *a;
    fontMenu.addAction(tr("Set the font for this texttype"));
    fontMenu.addAction(tr("Set the color for this texttype"));

    a = chatMenu.addAction(tr("Chat with this user"));
    a->setIcon(QIcon("snppictures/Chat_Icon.png"));
    chatMenu.addSeparator();
    chatMenu.addAction(tr("Set the font for this texttype"));
    chatMenu.addAction(tr("Set the color for this texttype"));

    wamenu.addAction(tr("Play this game."));
    wamenu.addAction(tr("Show game info."));
    wamenu.addSeparator();
    wamenu.addAction(tr("Set the font for this texttype"));
    wamenu.addAction(tr("Set the color for this texttype"));

    connect(tb->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(slidermoved(int)));
    connect(&singleton<netcoupler>(), SIGNAL(sigSettingsWindowChanged()),this, SLOT(useSettingsWindow()));
    connect(tb,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
}
bool chatHandler::eventFilter(QObject *obj, QEvent *event){
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

void chatHandler::initialFormatStarter() {
    for(int i=0;i<18;i++){
        hash[i].setProperty(whatsthispropertyId, i);
        initialFormat(hash[i]);
    }
}
void chatHandler::initialformatsaver() {
    singleton<chatFormatSettings>().safe();
}
QStringList formatstrings=QStringList()<<"nick"<<"myself"<<"garbage"<<"time"<<"garbagejoin" <<"garbagepart"
                                       <<"garbagequit" <<"chat" <<"action" <<"buddy" <<"debug" <<"ctcp" <<"raw"
                                       <<"prv"<<"notice"<<"noticeaction"<<"wa"<<"http";
void chatHandler::initialFormat(QTextCharFormat &format){                                   //provisorium TODO
    QString font=formatstrings[format.intProperty(whatsthispropertyId)]+"formatfont";
    QString color=formatstrings[format.intProperty(whatsthispropertyId)]+"formatcolor";

    if (singleton<chatFormatSettings>().map.contains(font))
        format.setFont(singleton<chatFormatSettings>().map[font].value<QFont> ());
    else
        format.setFont(QFont());
    if (singleton<chatFormatSettings>().map.contains(color))
        format.setForeground(singleton<chatFormatSettings>().map[color].value<QColor> ());
    else
        format.setForeground(QColor());
}
void chatHandler::anchorClicked(const QUrl &u) {
    QString s=u.toString();
    if (isClickableLink(s)) {        
        QUrl u1;
        u1.setUrl(s.simplified ());
        QDesktopServices::openUrl(u1);
    }
}
void chatHandler::appendGarbage(usermessage u){
    typedef QPair<QVariant, QTextCharFormat> pair;
    QList<pair> text;
    QTextCharFormat format=getRightFormat(u);
    text<<makePair("[" + u.time() + "]", hash[e_hash_time]);
    if(u.has_type(e_GARBAGEJOIN)){
        if(!S_S.chbjoininfo && !isprv)
            return;
        format=hash[e_hash_garbagejoin];
    }
    else if (u.has_type(e_GARBAGEPART)){
        if(!S_S.chbpartinfo && !isprv)
            return;
        format=hash[e_hash_garbagepart];
    }
    else if (u.has_type(e_GARBAGEQUIT)) {
        if(!S_S.chbquitinfo && !isprv)
            return;
        format=hash[e_hash_garbagequit];
    } else if(u.receiver() != singleton<netcoupler>().nick && u.user() != singleton<netcoupler>().nick)
        text<<makePair(tr("to ")+u.receiver()+ ": ", format);
    QString suffix;
    if(u.has_type(e_CTCP))
        text<<makePair(u.user() + " CTCP:",format);
    else if(u.has_type(e_RAWCOMMAND))
        text<<makePair(u.user() + " RAW:",format);
    else if (u.has_type(e_PRIVMSG)){
        if(u.has_type(e_ACTION)){
            text<<makePair("* " + u.user(),format);
            suffix="";
        }
        else
            text<<makePair(u.user()+">",format);
    } else if(u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION)){
            text<<makePair("<<< " + u.user(),format);
            suffix=">>>";
        }
        else{
            text<<makePair("<< " + u.user() + ":",format);
            suffix=">>";
        }
    } else
        text<<makePair(u.user() + ":",format);
    text<<getSegmentation(u.msg(), format);
    text<<makePair(suffix,format);

    foreach(pair p, text) {
        if(p.first.type() == QVariant::String)
            cursor->insertText(p.first.toString()+ " ",p.second);
        else if(p.first.type() == QVariant::Image){
            cursor->insertImage(p.first.value<QImage>());
            cursor->insertText(" ");
        } else
            myDebug()<<QString() + "##################void chathandlerprv::append(const usermessage u)";
    }
    cursor->insertText("\n");
    if (sliderAtMaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chatHandler::append(const usermessage u){
    typedef QPair<QVariant, QTextCharFormat> pair;
    QList<pair> text;    
    if(u.has_type(e_GARBAGE)){
        appendGarbage(u);
        return;
    }
    QString time = QTime::currentTime().toString("hh:mm");
    text<<makePair("[" + time + "]", hash[e_hash_time]);
    QTextCharFormat format=getRightFormat(u);
    if(u.has_type(e_CHANNELMSGTOCHAT))
        text<<makePair(tr("to ")+u.receiver()+ ": ", format);
    else
        appendhistory(u);        
    format.setProperty(userpropertyId,u.user());
    if(u.has_type(e_CHATMSGTOCHANNEL))
        text<<makePair(S_S.getString("leprvmsgprefix")+ ": ", format);
    hash[e_hash_nick].setProperty(userpropertyId,u.user());
    QString suffix;
    if(u.has_type(e_CTCP)){
        text<<makePair(u.user() + " CTCP: ",hash[e_hash_nick]);
    }
    else if(u.has_type(e_RAWCOMMAND))
        text<<makePair(u.user() + " RAW: ",hash[e_hash_nick]);
    else if(u.has_type(e_SCRIPTCOMMAND))
        text<<makePair(u.user() + " SCRIPT: ",hash[e_hash_nick]);
    else if (u.has_type(e_PRIVMSG)){
        if(u.has_type(e_ACTION)){
            text<<makePair("* " + u.user(),format);
            suffix="";
        }
        else
            text<<makePair(u.user()+">",hash[e_hash_nick]);
    } else if(u.has_type(e_NOTICE)){
        if(u.has_type(e_ACTION)){
            text<<makePair("<<< " + u.user(),format);
            suffix=">>>";
        }
        else{
            text<<makePair("<< " + u.user() + ": ",format);
            suffix=">>";
        }
    }
    text<<getSegmentation(u.msg(), format);
    text<<makePair(suffix,format);

    foreach(pair p, text) {
        if(p.first.type() == QVariant::String)
            cursor->insertText(p.first.toString()+ " ",p.second);
        else if(p.first.type() == QVariant::Image){
            cursor->insertImage(p.first.value<QImage>());
            cursor->insertText(" ");
        } else
            myDebug()<<QString() + "##################void chathandlerprv::append(const usermessage u)";
    }
    cursor->insertText("\n");
    if (sliderAtMaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());

    QString temp;
    if(containsCI(u.msg(), singleton<netcoupler>().nick)){
#ifdef PHONON
        singleton<soundHandler>().play_highlightningsound(u.user(),qobject_cast<QWidget*> ( parent()));
#endif
    }
    else if(containsOneCI(u.msg(), S_S.combobox_wrapper,&temp)){
#ifdef PHONON
        singleton<soundHandler>().play_costumwordsound(u.user(),qobject_cast<QWidget*> ( parent()));
#endif
        singleton<balloonHandler>().gotCustomWord(temp,u.user());
    }
}
void chatHandler::appenddebug(const QString &msg) {
    cursor->insertText(msg + "\n", hash[e_hash_debug]);
    if (sliderAtMaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
QList<QPair<QVariant, QTextCharFormat> > chatHandler::getSegmentation(QString s, QTextCharFormat format){
    QStringList sl=s.split(" ", QString::SkipEmptyParts);
    QList<QPair<QVariant, QTextCharFormat> > text;
    foreach(s,sl){
        if(isClickableLink(s)){
            hash[e_hash_http].setAnchorHref(s);
            text<<makePair(s, hash[e_hash_http]);
        } else if(startswithCI(s, "wa://")){
            hash[e_hash_wa].setAnchorHref(s);
            hash[e_hash_wa].setProperty(linkpropertyId, s);
            text<<makePair("[" + tr("GAMELINK") + "]", hash[e_hash_wa]);
        } else {
            if(S_S.showsmileysinchannels)
                text<<makePair(emot->contains(s),format);
            else
                text<<makePair(s,format);
        }
    }
    return text;
}
QTextCharFormat chatHandler::getRightFormat(const usermessage u){
    QTextCharFormat format;
    if(u.has_type(e_CHATMSGTOCHANNEL))
        format = hash[e_hash_prv];
    else if (!containsCI(S_S.buddylist, u.user()))
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
    } else if(u.has_type(e_RAWCOMMAND) || u.has_type(e_SCRIPTCOMMAND))
        format=hash[e_hash_raw];

    return format;
}
void chatHandler::insertText(const QString &s, QTextCharFormat &t,QString user) {
    QString temp;
    if (containsClickableLink(s) || containsCI(s, "wa://")) {
        QStringList sl = s.split(" ", QString::SkipEmptyParts);
        foreach(QString str,sl) {
            QString strtemp = str;
            str = str + " ";
            if (isClickableLink(str)) {
                hash[e_hash_http].setAnchorHref(strtemp);
                cursor->insertText(str, hash[e_hash_http]);
            } else if (startswithCI(str, "wa://")) {
                hash[e_hash_wa].setAnchorHref(str);
                cursor->insertText(tr("GAMELINK"), hash[e_hash_wa]);
            } else {
                cursor->insertText(str, t);
            }
        }
    } else if (containsCI(s, singleton<netcoupler>().nick)) {
        if (!t.anchorHref().startsWith("<notice>") && t.anchorHref() != "action")
#ifdef PHONON
            singleton<soundHandler>().play_highlightningsound(user,qobject_cast<QWidget*> ( parent()));
#endif
        cursor->insertText(s, t);
    } else if (containsOneCI(s, S_S.combobox_wrapper,&temp)) {
#ifdef PHONON
        singleton<soundHandler>().play_costumwordsound(user,qobject_cast<QWidget*> ( parent()));
#endif
        singleton<balloonHandler>().gotCustomWord(temp,user);
        cursor->insertText(s, t);
    } else
        cursor->insertText(s, t);
    if (sliderAtMaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chatHandler::slidermoved(int i) {
    if(!gotFirstMessage)
    {
        gotFirstMessage=true;
        return;
    }
    if (i == tb->verticalScrollBar()->maximum())
        sliderAtMaximum = 1;
    else
        sliderAtMaximum = 0;
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
void chatHandler::setSchemeMap(QTextCharFormat *c, QFont f) {
    singleton<chatFormatSettings>().map[c->stringProperty(whatsthispropertyId)+"font"] = f;
    c->setFont(f);
    fontOrColorHasChanged = 1;
}
void chatHandler::setSchemeMap(QTextCharFormat *c, QColor f) {
    if (f.isValid()) {
        singleton<chatFormatSettings>().map[c->stringProperty(whatsthispropertyId)+"color"] = f;
        c->setForeground(f);
        fontOrColorHasChanged = 1;
    }
}
void chatHandler::setSchemeMap(int i,QFont f){
    singleton<chatFormatSettings>().map[formatstrings[i]+"formatfont"] = f;
    hash[i].setFont(f);
    fontOrColorHasChanged = 1;
}
void chatHandler::setSchemeMap(int i,QColor c){
    if (!c.isValid())
        return;
    singleton<chatFormatSettings>().map[formatstrings[i]+"formatcolor"] = c;
    hash[i].setForeground(c);
    fontOrColorHasChanged = 1;
}
void chatHandler::get_new_font_and_color(QTextCharFormat *format){
    QAction *a = fontMenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool b;
            QFont f = QFontDialog::getFont(&b, format->font(), 0);
            if (b)
                setSchemeMap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setSchemeMap(format->intProperty(whatsthispropertyId), f);
        }
    }
}
void chatHandler::getNewFontAndColorWithChatwindow(QTextCharFormat *format){
    QAction *a = chatMenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool b;
            QFont f = QFontDialog::getFont(&b, format->font(), tb);
            if (b)
                setSchemeMap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setSchemeMap(format->intProperty(whatsthispropertyId), f);
        } else
            emit sigOpenChatWindow(format->stringProperty(userpropertyId));
    }
}
void chatHandler::getNewFontAndColorWithWalink(QTextCharFormat *format){
    QAction *a = wamenu.exec(QCursor::pos());
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool b;
            QFont f = QFontDialog::getFont(&b, format->font(), tb);
            if (b)
                setSchemeMap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(format->foreground().color(), 0);
            setSchemeMap(format->intProperty(whatsthispropertyId), f);
        } else if (a->text() == tr("Show game info.")) {
            QMessageBox::information(0,tr("Info about this gamelink."), format->anchorHref());
        } else {
            joinprvgame *prv = new joinprvgame(format->anchorHref(), chatpartner);
            prv->show();
            connect(prv, SIGNAL(sigjoingamelink(const QString&)),&singleton<netcoupler>(), SLOT(joinGameLink(const QString&)));
        }
    }
}
void chatHandler::contextrequest(const QPoint &p) {
    QTextCharFormat format=tb->cursorForPosition(p).charFormat();
    int whatsthis=format.intProperty(whatsthispropertyId);
    if(whatsthis==e_hash_wa)
        getNewFontAndColorWithWalink(&format);
    else if(whatsthis == e_hash_prv || whatsthis == e_hash_action || whatsthis == e_hash_nick || whatsthis == e_hash_notice || whatsthis == e_hash_noticeaction || whatsthis == e_hash_chat)
        getNewFontAndColorWithChatwindow(&format);
    else
        get_new_font_and_color(&format);
}
void chatHandler::useSettingsWindow(const QString &s) {
    if (s == "sbmaximumoftextblocks" || s == "") {
        int i = S_S.getint("sbmaximumoftextblocks");
        doc->setMaximumBlockCount(i);
    }
}
chatHandler::~chatHandler() {
    // TODO Auto-generated destructor stub
}
void chatHandler::selectionChanged(){
    tb->copy();
}
void chatHandler::moveSliderToMaximum(){
    tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
    sliderAtMaximum=true;
}
QPair<QVariant, QTextCharFormat> chatHandler::makePair(QVariant v, QTextCharFormat format){
    return qMakePair<QVariant, QTextCharFormat>(v,format);
}
