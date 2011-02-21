#include "logbrowser.h"
#include"netcoupler.h"
#include<QPointer>
extern QHash<QString, QStringList> usergarbagemap;
logbrowser::logbrowser(QWidget *parent)
    : QWidget(parent)
{
	 setObjectName("normalwidget");
	ui.setupUi(this);
	 setWindowTitle(tr("Log Browser"));
	 setAttribute(Qt::WA_DeleteOnClose);
	foreach(QString s,usergarbagemap.keys()){
		ui.listWidget->addItem(s);
	}
	connect(ui.close,SIGNAL(clicked()),this,SLOT(closeclicked()));
	connect(ui.chat,SIGNAL(clicked()),this,SLOT(chatclicked()));

}
void logbrowser::closeclicked(){
	 close();
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
