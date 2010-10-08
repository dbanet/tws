#include "textschemehandler.h"
#include<QFontDialog>
#include<QColorDialog>
#include"chathandler.h"
#include "charformatsettings.h"
#include"mainwindow.h"
#include"window.h"
extern bool fontorcolorchanged;
textschemehandler::textschemehandler(QWidget *parent) :
	QWidget(parent) {
	this->setObjectName("normalwidget");
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowTitle(tr("Textscheme helper"));
	ui.setupUi(this);
	connect(ui.close, SIGNAL(clicked()),this, SLOT(closeclicked()));
	connect(ui.font, SIGNAL(clicked()),this, SLOT(fontclicked()));
	connect(ui.color, SIGNAL(clicked()),this, SLOT(colorclicked()));
	connect(ui.show, SIGNAL(clicked()),this, SLOT(showclicked()));
	map["timeformat"] = &chathandler::timeformat;
	map["nickformat"] = &chathandler::nickformat;
	map["chatformat"] = &chathandler::chatformat;
	map["actionformat"] = &chathandler::actionformat;
	map["buddyformat"] = &chathandler::buddyformat;
	map["noticeformat"] = &chathandler::noticeformat;
	map["prvformat"] = &chathandler::prvformat;
	map["httpformat"] = &chathandler::httpformat;
	map["waformat"] = &chathandler::waformat;
	map["debugformat"] = &chathandler::debugformat;
	map["garbagejoinformat"] = &chathandler::garbagejoinformat;
	map["garbagepartformat"] = &chathandler::garbagepartformat;
	map["garbagequitformat"] = &chathandler::garbagequitformat;
	map["garbageformat"] = &chathandler::garbageformat;
        map["myselfformat"] = &chathandler::myselfformat;
	ui.listWidget->addItems(map.keys());
}
void textschemehandler::closeclicked() {
	this->close();
}
void textschemehandler::fontclicked() {
	if (!ui.listWidget->selectedItems().isEmpty()) {
		bool *b = new bool;
		QFont f = QFontDialog::getFont(b,
                                singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()
						+ "font"].value<QFont> (), this);
		if (*b) {
                        singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()
					+ "font"] = f;
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
                        singleton<charformatsettings>().map[ui.listWidget->selectedItems().first()->text()
					+ "color"] = c;
			chathandler::initialformatstarter();
		}
	}
	fontorcolorchanged=1;
}
void textschemehandler::showclicked() {
	if (!ui.listWidget->selectedItems().isEmpty()) {
		foreach(::window *w,mainwindow::windowlist){
			w->chat->insertText(tr("This is a teststring > ? ! < @ THIS IS A TESTSTRING! 0123456789 xD ,) :) ;P :P :E :D :C ;o :o o0 o_0 ,0.0,"),*map[ui.listWidget->selectedItems().first()->text()]);
		}
	}

}
textschemehandler::~textschemehandler() {

}
