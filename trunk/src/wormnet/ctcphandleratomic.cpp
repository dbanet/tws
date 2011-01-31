#include "ctcphandleratomic.h"

ctcphandleratomic::ctcphandleratomic(QWidget *parent)
    : QWidget(parent)
{
	 setObjectName("normalwidget");
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(delclicked()));
}
void ctcphandleratomic::delclicked(){
	 deleteLater();
}
ctcphandleratomic::~ctcphandleratomic()
{

}
