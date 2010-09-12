/*
 * chathandler.cpp
 *
 *  Created on: 19.11.2008
 *      Author: looki
 */

#include "chathandler.h"
#include<QTextBrowser>
#include <QTextDocument>
#include<QTime>
#include<QDebug>
#include<QFontDialog>
#include<QColorDialog>
#include<QScrollBar>
#include<QDesktopServices>
#include<QMessageBox>
#include<QPointer>
#include<QWheelEvent>
#include<stdexcept>
#include "charformatsettings.h"
#include "snpsettings.h"
#include"netcoupler.h"
#include"joinprvgame.h"
#include"settingswindow.h"
#include "charformatsettings.h"
#include "sound_handler.h"
#include"global_functions.h"
#include "balloon_handler.h"
extern QPointer<netcoupler> net;
QTextCharFormat chathandler::timeformat;
QTextCharFormat chathandler::nickformat;
QTextCharFormat chathandler::chatformat;
QTextCharFormat chathandler::actionformat;
QTextCharFormat chathandler::buddyformat;
QTextCharFormat chathandler::noticeformat;
QTextCharFormat chathandler::prvformat;
QTextCharFormat chathandler::httpformat;
QTextCharFormat chathandler::waformat;
QTextCharFormat chathandler::debugformat;
QTextCharFormat chathandler::garbagejoinformat;
QTextCharFormat chathandler::garbagepartformat;
QTextCharFormat chathandler::garbagequitformat;
QTextCharFormat chathandler::garbageformat;
QTextCharFormat chathandler::myselfformat;
extern bool fontorcolorchanged;
chathandler::chathandler(QObject *parent, QTextBrowser *t, QString chan) :
        QObject(parent), channel(chan), slideratmaximum(true),gotFirstMessage(false), tb(t) {
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

    QAction *a;
    fontmenu.addAction(tr("Set the font for this texttype"));
    fontmenu.addAction(tr("Set the color for this texttype"));
    a = nickmenu.addAction(tr("Chat with this user"));
    a->setIcon(QIcon("snppictures/Chat_Icon.png"));
    nickmenu.addSeparator();
    nickmenu.addAction(tr("Set the font for this texttype"));
    nickmenu.addAction(tr("Set the color for this texttype"));
    a = prvmenu.addAction(tr("Chat with this user"));
    a->setIcon(QIcon("snppictures/Chat_Icon.png"));
    prvmenu.addSeparator();
    prvmenu.addAction(tr("Set the font for this texttype"));
    prvmenu.addAction(tr("Set the color for this texttype"));
    wamenu.addAction(tr("Play this game."));
    wamenu.addAction(tr("Show game info."));
    wamenu.addSeparator();
    wamenu.addAction(tr("Set the font for this texttype"));
    wamenu.addAction(tr("Set the color for this texttype"));
    a = noticemenu.addAction(tr("Chat with this user"));
    a->setIcon(QIcon("snppictures/Chat_Icon.png"));
    noticemenu.addSeparator();
    noticemenu.addAction(tr("Set the font for this texttype"));
    noticemenu.addAction(tr("Set the color for this texttype"));
    debugmenu.addAction(tr("Set the font for this texttype"));
    debugmenu.addAction(tr("Set the color for this texttype"));

    connect(tb->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(slidermoved(int)));
    connect(net, SIGNAL(sigsettingswindowchanged()),this, SLOT(usesettingswindow()));
    connect(tb,SIGNAL(selectionChanged()),this,SLOT(selectionChanged()));
}
bool chathandler::eventFilter(QObject *obj, QEvent *event){    
    static bool isDown=false;
    if(obj!=tb)
        return false;    
    if(event->type()==QEvent::Wheel){
        slidermoved(tb->verticalScrollBar()->value());
        return true;
    }
    return false;
}

void chathandler::initialformatstarter() {
    initialformat(timeformat, "timeformatfont", "timeformatcolor");
    initialformat(chatformat, "chatformatfont", "chatformatcolor");
    initialformat(actionformat, "actionformatfont", "actionformatcolor");
    initialformat(buddyformat, "buddyformatfont", "buddyformatcolor");
    initialformat(nickformat, "nickformatfont", "nickformatcolor");
    initialformat(prvformat, "prvformatfont", "prvformatcolor");
    initialformat(httpformat, "httpformatfont", "httpformatcolor");
    initialformat(waformat, "waformatfont", "waformatcolor");
    initialformat(noticeformat, "noticeformatfont", "noticeformatcolor");
    initialformat(debugformat, "debugformatfont", "debugformatcolor");
    initialformat(garbagejoinformat, "garbagejoinformatfont",
                  "garbagejoinformatcolor");
    initialformat(garbagepartformat, "garbagepartformatfont",
                  "garbagepartformatcolor");
    initialformat(garbagequitformat, "garbagequitformatfont",
                  "garbagequitformatcolor");
    initialformat(garbageformat, "garbageformatfont", "garbageformatcolor");
    initialformat(myselfformat, "myselfformatfont", "myselfformatcolor");
    garbagejoinformat.setAnchorHref("garbagejoin");
    garbagequitformat.setAnchorHref("garbagequit");
    garbagepartformat.setAnchorHref("garbagepart");
    timeformat.setAnchorHref("time");
    chatformat.setAnchorHref("chat");
    actionformat.setAnchorHref("action");
    buddyformat.setAnchorHref("buddy");
    debugformat.setAnchorHref("debug");
    garbageformat.setAnchorHref("garbage");
    myselfformat.setAnchorHref("myself");
    nickformat.setAnchorHref("nick");
}
void chathandler::initialformatsaver() {
    singleton<charformatsettings>().safe();
}
void chathandler::initialformat(QTextCharFormat &c, const QString &font,
                                const QString &color) {
    if (singleton<charformatsettings>().map.contains(font))
        c.setFont(singleton<charformatsettings>().map[font].value<QFont> ());
    else
        c.setFont(QFont());
    if (singleton<charformatsettings>().map.contains(color))
        c.setForeground(singleton<charformatsettings>().map[color].value<QColor> ());
    else
        c.setForeground(QColor());
    c.setProperty(1, font);
    c.setProperty(2, color);
}
void chathandler::anchorclicked(const QUrl &u) {
    QString s=u.toString();
    if (containsCI(s, "http://") || containsCI(s,"www.")) {
        s.remove("\n");
        s.remove("\r");
        QUrl u1;
        u1.setUrl(s);
        QDesktopServices::openUrl(u1);
    }
}
QString globallastuser;
void chathandler::append(const QString &user, const QString &receiver,
                         const QString &msg) {
    globallastuser = user;
    QString msgtemp = msg;
    msgtemp.remove("\r");
    msgtemp.remove("\n");
    QString time = QTime::currentTime().toString("hh:mm");
    cursor->insertText(time + ":", timeformat);
    if (!containsCI(singleton<snpsettings>().map["buddylist"].value<QStringList> (), user)) {
        if (compareCI(receiver, channel)) {
            if (msg.startsWith("\001ACTION")) {
                msgtemp.remove(0, 7).remove("\001");
                insertText("<" + user + " " + msgtemp + +">", actionformat,user);
            } else {
                nickformat.setAnchorHref("<nick> " + user);
                cursor->insertText(user + "> ", nickformat);
                if(user==net->nick)
                    insertText(msgtemp, myselfformat,user);
                else
                    insertText(msgtemp, chatformat,user);
            }
        } else {
            if (msg.startsWith("\001ACTION")) {
                msgtemp.remove(0, 7).remove("\001");
                insertText("<" + user + " " + msgtemp + ">", actionformat,user);

            } else {
                prvformat.setAnchorHref("<prv> " + user);
                cursor->insertText(user + " to " + receiver + ">", prvformat);
                insertText(msgtemp, prvformat,user);
            }
        }
    } else {
        if (msg.startsWith("\001ACTION")) {
            msgtemp.remove(0, 7).remove("\001");
            insertText("<" + user + " " + msgtemp + ">", buddyformat,user);
        } else {
            nickformat.setAnchorHref("<nick> " + user);
            cursor->insertText(user + "> ", nickformat);
            if(user==net->nick)
                insertText(msgtemp, myselfformat,user);
            else
                insertText(msgtemp, buddyformat,user);
        }
    }
    cursor->insertText("\n");
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendnotice(const QString &user, const QString &receiver,
                               const QString &msg) {
    QString msgtemp = msg;
    msgtemp.remove("\r");
    msgtemp.remove("\n");
    QString time = QTime::currentTime().toString("hh:mm");
    cursor->insertText(time + ":", timeformat);
    noticeformat.setAnchorHref("<notice> " + user);
    if (msg.startsWith("\001ACTION")) {
        msgtemp.remove(0, 7).remove("\001");
        insertText(tr("Notice from") + " " + user + " " + tr("to") + " "
                   + receiver + "> <" + msgtemp + ">", noticeformat,user);
    } else {
        insertText(tr("Notice from") + " " + user + " " + tr("to") + " "
                   + receiver + "> " + msgtemp.remove("\001") + " ", noticeformat,user);
    }
    cursor->insertText("\n");
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appenddebug(const QString &msg) {
    cursor->insertText(msg + "\n", debugformat);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendjoingarbage(const QString &msg) {
    cursor->insertText(msg + "\n", garbagejoinformat);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendpartgarbage(const QString &msg) {
    cursor->insertText(msg + "\n", garbagepartformat);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::appendquitgarbage(const QString &msg) {
    cursor->insertText(msg + "\n", garbagequitformat);
    if (slideratmaximum)
        tb->verticalScrollBar()->setValue(tb->verticalScrollBar()->maximum());
}
void chathandler::insertText(const QString &s, QTextCharFormat &t,QString user) {
    QString temp;
    if (containsCI(s, "http://") || containsCI(s, "wa://") || containsCI(s, "www.")) {
        QStringList sl = s.split(" ", QString::SkipEmptyParts);
        foreach(QString str,sl) {
            QString strtemp = str;
            str = str + " ";
            if (startswithCI(str, "http://") || startswithCI(str, "www.")) {
                httpformat.setAnchorHref(strtemp);
                cursor->insertText(str, httpformat);
            } else if (startswithCI(str, "wa://")) {
                waformat.setAnchorHref(str);
                cursor->insertText(tr("GAMELINK"), waformat);
            } else {
                cursor->insertText(str, t);
            }
        }
    } else if (containsCI(s, net->nick)) {
        if (!t.anchorHref().startsWith("<notice>") && t.anchorHref()!= "action")
            singleton<sound_handler>().play_highlightningsound(globallastuser,qobject_cast<QWidget*> (this->parent()));
        cursor->insertText(s, t);
    } else if (containsOneCI(s, singleton<settingswindow>().from_map("combobox_wrapper").value<QStringList>(),&temp)) {
        singleton<sound_handler>().play_costumwordsound(globallastuser,qobject_cast<QWidget*> (this->parent()));
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
    else {        
        slideratmaximum = 0;
    }
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
    singleton<charformatsettings>().map[c->property(1).toString()] = f;
    c->setFont(f);
    fontorcolorchanged = 1;
}
void chathandler::setschememap(QTextCharFormat *c, QColor f) {
    if (f.isValid()) {
        singleton<charformatsettings>().map[c->property(2).toString()] = f;
        c->setForeground(f);
        fontorcolorchanged = 1;
    }
}
void chathandler::get_new_font_and_color(QTextCharFormat *tempformat,QAction *a){
    if (a != 0) {
        if (a->text() == tr("Set the font for this texttype")) {
            bool *b = new bool;
            QFont f = QFontDialog::getFont(b, tempformat->font(), tb);
            if (*b) {
                setschememap(tempformat, f);
            }
        } else if (a->text() == tr("Set the color for this texttype")) {
            QColor f = QColorDialog::getColor(
                    tempformat->foreground().color(), tb);
            setschememap(tempformat, f);
        }
    }
}

void chathandler::contextrequest(const QPoint &p) {
    if (tb->anchorAt(p) != "") {
        QString anchor = tb->anchorAt(p);
        QTextCharFormat *tempformat;
        if (anchor == "nick") {
            get_new_font_and_color(&nickformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "myself") {
            get_new_font_and_color(&myselfformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "garbage") {
            get_new_font_and_color(&garbageformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "time") {
            get_new_font_and_color(&timeformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "garbagejoin") {
            get_new_font_and_color(&garbagejoinformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "garbagepart") {
            get_new_font_and_color(&garbagepartformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "garbagequit") {
            get_new_font_and_color(&garbagequitformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "chat") {
            get_new_font_and_color(&chatformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "action") {
            get_new_font_and_color(&actionformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "buddy") {
            get_new_font_and_color(&buddyformat,fontmenu.exec(QCursor::pos()));
        } else if (anchor == "debug") {
            get_new_font_and_color(&debugformat,debugmenu.exec(QCursor::pos()));
        } else if (containsCI(anchor, "http://") || containsCI(anchor, "www.")) {
            get_new_font_and_color(&httpformat,fontmenu.exec(QCursor::pos()));
        } else if (containsCI(anchor, "wa://")) {
            QAction *a = wamenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Set the font for this texttype")) {
                    bool *b = new bool;
                    tempformat = &waformat;
                    QFont f = QFontDialog::getFont(b, waformat.font(), tb);
                    if (*b) {
                        setschememap(tempformat, f);
                    }
                } else if (a->text() == tr("Set the color for this texttype")) {
                    tempformat = &waformat;
                    QColor f = QColorDialog::getColor(
                            waformat.foreground().color(), tb);
                    setschememap(tempformat, f);
                } else if (a->text() == tr("Show game info.")) {
                    QMessageBox::information(0,
                                             tr("Info about this gamelink."), anchor);
                } else {
                    joinprvgame *prv = new joinprvgame(anchor, channel);
                    prv->show();
                    connect(prv, SIGNAL(sigjoingamelink(const QString&)),net, SLOT(joingamelink(const QString&)));
                }
            }
        } else if (anchor.startsWith("<prv>")) {
            QAction *a = prvmenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Set the font for this texttype")) {
                    bool *b = new bool;
                    tempformat = &prvformat;
                    QFont f = QFontDialog::getFont(b, prvformat.font(), tb);
                    if (*b) {
                        setschememap(tempformat, f);
                    }
                } else if (a->text() == tr("Set the color for this texttype")) {
                    tempformat = &prvformat;
                    QColor f = QColorDialog::getColor(
                            prvformat.foreground().color(), tb);
                    setschememap(tempformat, f);
                } else {
                    emit sigopenchatwindow(anchor.split(" ").last());
                }
            }
        } else if (anchor.startsWith("<nick>")) {
            QAction *a = nickmenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Set the font for this texttype")) {
                    bool *b = new bool;
                    tempformat = &nickformat;
                    QFont f = QFontDialog::getFont(b, nickformat.font(), tb);
                    if (*b) {
                        setschememap(tempformat, f);
                    }
                } else if (a->text() == tr("Set the color for this texttype")) {
                    tempformat = &nickformat;
                    QColor f = QColorDialog::getColor(
                            nickformat.foreground().color(), tb);
                    setschememap(tempformat, f);
                } else {
                    emit sigopenchatwindow(anchor.split(" ").last());
                }
            }
        } else if (anchor.startsWith("<notice>")) {
            QAction *a = noticemenu.exec(QCursor::pos());
            if (a != 0) {
                if (a->text() == tr("Set the font for this texttype")) {
                    bool *b = new bool;
                    tempformat = &noticeformat;
                    QFont f = QFontDialog::getFont(b, noticeformat.font(), tb);
                    if (*b) {
                        setschememap(tempformat, f);
                    }
                } else if (a->text() == tr("Set the color for this texttype")) {
                    tempformat = &noticeformat;
                    QColor f = QColorDialog::getColor(
                            noticeformat.foreground().color(), tb);
                    setschememap(tempformat, f);
                } else {
                    emit sigopenchatwindow(anchor.split(" ").last());
                }

            }
        } else{
            qDebug() << anchor;
            throw std::runtime_error("void chathandler::contextrequest(const QPoint &p)");
        }
    }
}
void chathandler::usesettingswindow(const QString &s) {
    if (s == "sbmaximumoftextblocks" || s == "") {
        int i = singleton<settingswindow>().from_map("sbmaximumoftextblocks").value<int> ();
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
