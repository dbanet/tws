#include "logbrowser.h"
#include"netcoupler.h"
#include<QPointer>
extern QPointer<netcoupler> net;
extern QMap<QString, QStringList> usergarbagemap;
logbrowser::logbrowser(QWidget *parent)
    : QWidget(parent)
{
	this->setObjectName("normalwidget");
	ui.setupUi(this);
	this->setWindowTitle(tr("Log Browser"));
	this->setAttribute(Qt::WA_DeleteOnClose);
	foreach(QString s,usergarbagemap.keys()){
		ui.listWidget->addItem(s);
	}
	connect(ui.close,SIGNAL(clicked()),this,SLOT(closeclicked()));
	connect(ui.chat,SIGNAL(clicked()),this,SLOT(chatclicked()));

}
void logbrowser::closeclicked(){
	this->close();
}
void logbrowser::chatclicked(){
	if(!ui.listWidget->selectedItems().isEmpty()){
	QString s=ui.listWidget->selectedItems().first()->text();
	emit sigopenchatwindow(s);
	}
}
logbrowser::~logbrowser()
{

}
