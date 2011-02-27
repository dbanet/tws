#include"textschemehandler.h"
#include<QFontDialog>
#include<QColorDialog>
#include"chathandler.h"
#include"charformatsettings.h"
#include"mainwindow.h"
#include"window.h"
extern bool fontorcolorchanged;
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
    map["timeformat"] = &chathandler::hash[e_hash_time];
    map["nickformat"] = &chathandler::hash[e_hash_nick];
    map["chatformat"] = &chathandler::hash[e_hash_chat];
    map["actionformat"] = &chathandler::hash[e_hash_action];
    map["buddyformat"] = &chathandler::hash[e_hash_buddy];
    map["noticeformat"] = &chathandler::hash[e_hash_notice];
    map["noticeactionformat"] = &chathandler::hash[e_hash_noticeaction];
    map["prvformat"] = &chathandler::hash[e_hash_prv];
    map["httpformat"] = &chathandler::hash[e_hash_http];
    map["waformat"] = &chathandler::hash[e_hash_wa];
    map["debugformat"] = &chathandler::hash[e_hash_debug];
    map["garbagejoinformat"] = &chathandler::hash[e_hash_garbagejoin];
    map["garbagepartformat"] = &chathandler::hash[e_hash_garbagepart];
    map["garbagequitformat"] = &chathandler::hash[e_hash_garbagequit];
    map["garbageformat"] = &chathandler::hash[e_hash_garbage];
    map["myselfformat"] = &chathandler::hash[e_hash_myself];
    map["ctcpformat"] = &chathandler::hash[e_hash_ctcp];
    map["rawformat"] = &chathandler::hash[e_hash_raw];
    ui.listWidget->addItems(map.keys());
}
void textschemehandler::closeclicked() {
    close();
}
void textschemehandler::fontclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        bool *b = new bool;
        QFont f = QFontDialog::getFont(b,
                                       singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()+ "font"].value<QFont> (), this);
        if (*b) {
            singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()+ "font"] = f;
            chathandler::initialformatstarter();
        }
    }
    fontorcolorchanged=1;
}
void textschemehandler::colorclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        //bool *b = new bool;
        QColor c;
        c = QColorDialog::getColor(
                singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()
                                                    + "color"].value<QColor> (), this);
        if (c.isValid()) {
            singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text() + "color"] = c;
            chathandler::initialformatstarter();
        }
    }
    fontorcolorchanged=1;
}
void textschemehandler::showclicked() {
    if (!ui.listWidget->selectedItems().isEmpty()) {
        foreach(::window *w,qobjectwrapper<mainwindow>::ref().windowlist)
            w->chat->insertText(tr("This is a teststring > ? ! < @ THIS IS A TESTSTRING! 0123456789 xD ,) :) ;P :P :E :D :C ;o :o o0 o_0 ,0.0,"),*map[ui.listWidget->selectedItems().first()->text()]);
    }
}
textschemehandler::~textschemehandler() {   
}
