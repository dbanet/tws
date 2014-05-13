#include "textschemehandler.h"
#include <QFontDialog>
#include <QColorDialog>
#include "chathandler.h"
#include "charformatsettings.h"
#include "mainwindow.h"
#include "window.h"
extern bool fontOrColorHasChanged;
textschemehandler::textschemehandler(QWidget *parent) :
    QWidget(parent) {
    setObjectName("normalwidget");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Textscheme helper"));
    ui.setupUi(this);
    connect(ui.close, SIGNAL(clicked()),this, SLOT(closeclicked()));
    connect(ui.font, SIGNAL(clicked()),this, SLOT(fontclicked()));
    connect(ui.color, SIGNAL(clicked()),this, SLOT(colorclicked()));
    connect(ui.show, SIGNAL(clicked()),this, SLOT(showclicked()));
    map["timeformat"] = &chatHandler::hash[e_hash_time];
    map["nickformat"] = &chatHandler::hash[e_hash_nick];
    map["chatformat"] = &chatHandler::hash[e_hash_chat];
    map["actionformat"] = &chatHandler::hash[e_hash_action];
    map["buddyformat"] = &chatHandler::hash[e_hash_buddy];
    map["noticeformat"] = &chatHandler::hash[e_hash_notice];
    map["noticeactionformat"] = &chatHandler::hash[e_hash_noticeaction];
    map["prvformat"] = &chatHandler::hash[e_hash_prv];
    map["httpformat"] = &chatHandler::hash[e_hash_http];
    map["waformat"] = &chatHandler::hash[e_hash_wa];
    map["debugformat"] = &chatHandler::hash[e_hash_debug];
    map["garbagejoinformat"] = &chatHandler::hash[e_hash_garbagejoin];
    map["garbagepartformat"] = &chatHandler::hash[e_hash_garbagepart];
    map["garbagequitformat"] = &chatHandler::hash[e_hash_garbagequit];
    map["garbageformat"] = &chatHandler::hash[e_hash_garbage];
    map["myselfformat"] = &chatHandler::hash[e_hash_myself];
    map["ctcpformat"] = &chatHandler::hash[e_hash_ctcp];
    map["rawformat"] = &chatHandler::hash[e_hash_raw];        
    ui.listWidget->addItems(map.keys());
}
void textschemehandler::closeclicked() {
    close();
}
void textschemehandler::fontclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        bool b;
        QFont f = QFontDialog::getFont(&b,
                                       singleton<chatFormatSettings>().map[ui.listWidget->selectedItems().first()->text()+ "font"].value<QFont> (), this);
        if (b) {
            singleton<chatFormatSettings>().map[ui.listWidget->selectedItems().first()->text()+ "font"] = f;
            chatHandler::initialFormatStarter();
        }
    }
    fontOrColorHasChanged=1;
}
void textschemehandler::colorclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        QColor c;
        c = QColorDialog::getColor(
                    singleton<chatFormatSettings>().map[ui.listWidget->selectedItems().first()->text()
                    + "color"].value<QColor> (), this);
        if (c.isValid()) {
            singleton<chatFormatSettings>().map[ui.listWidget->selectedItems().first()->text() + "color"] = c;
            chatHandler::initialFormatStarter();
        }
    }
    fontOrColorHasChanged=1;
}
void textschemehandler::showclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        foreach(::window *w,qobjectwrapper<mainwindow>::ref().windowList)
            w->chat->insertText(tr("This is a teststring > ? ! < @ THIS IS A TESTSTRING! 0123456789 xD ,) :) ;P :P :E :D :C ;o :o o0 o_0 ,0.0,"),*map[ui.listWidget->selectedItems().first()->text()]);
    }
}
textschemehandler::~textschemehandler() {
}
