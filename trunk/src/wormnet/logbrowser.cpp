#include"logbrowser.h"
#include"global_functions.h"

#include<QPointer>
logbrowser::logbrowser(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("normalwidget");
    ui.setupUi(this);
    setWindowTitle(logbrowser::tr("Log Browser"));
    setAttribute(Qt::WA_DeleteOnClose);
    foreach(QString s,history().keys())
        ui.listWidget->addItem(s);
    ui.listWidget->sortItems();
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
