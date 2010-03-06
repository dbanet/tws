#include "ctcphandleratomic.h"

ctcphandleratomic::ctcphandleratomic(QWidget *parent)
    : QWidget(parent)
{
	this->setObjectName("normalwidget");
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(delclicked()));
}
void ctcphandleratomic::delclicked(){
	this->deleteLater();
}
ctcphandleratomic::~ctcphandleratomic()
{

}
