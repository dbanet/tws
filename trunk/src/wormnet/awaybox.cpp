#include "awaybox.h"
#include "snpsettings.h"
#include"netcoupler.h"
#include<QPointer>
bool awaybox::ison=0;
awaybox::awaybox(QWidget *parent) :
	QWidget(parent) {
	this->setObjectName("awaybox");
	ui.setupUi(this);
	this->setWindowTitle(tr("Type in your awaymessage."));
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowIcon(QIcon("snppictures/tray.png"));
        sl = singleton<snpsettings>().map["awaymessage"].value<QStringList>();
	counter=sl.size();
	i=counter-1;
        if(!singleton<snpsettings>().map["awaymessage"].value<QStringList>().isEmpty()){
		ui.textEdit->setText(sl[i]);
	}
	connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
	connect(ui.back, SIGNAL(clicked()),this, SLOT(backclicked()));
	connect(ui.forward, SIGNAL(clicked()),this, SLOT(forwardclicked()));
	this->setWindowFlags(windowFlags() ^= Qt::WindowSystemMenuHint);
	ison=1;
	//connect(ui.cancel,SIGNAL(clicked()),this,SLOT(cancelclicked()));
}
void awaybox::okclicked() {
        singleton<netcoupler>().setaway(ui.textEdit->toPlainText());
	if(!sl.contains(ui.textEdit->toPlainText()))
		sl<<ui.textEdit->toPlainText();
	else
		sl.move(sl.indexOf(ui.textEdit->toPlainText()),sl.size()-1);
        singleton<snpsettings>().map["awaymessage"]=sl;
        singleton<snpsettings>().safe();
	emit sigok();
	QApplication::processEvents();
	this->close();
}
void awaybox::cancelclicked() {
	this->close();
}
void awaybox::backclicked() {
	if (i > 0)
		ui.textEdit->setText(sl[--i]);
}
void awaybox::forwardclicked() {
	if (i < counter-1)
		ui.textEdit->setText(sl[++i]);
}
void awaybox::closeEvent(QCloseEvent *) {
	ison=0;
        singleton<netcoupler>().setaway(ui.textEdit->toPlainText());
}
awaybox::~awaybox() {
}
