#include "uploadedfileatomic.h"
#include<QTextStream>
#include<QClipboard>
#include<QFile>

uploadedfileatomic::uploadedfileatomic(const QString &s,const QString &str,QWidget *parent)
    : QWidget(parent)
{
	this->setObjectName("normalwidget");
	ui.setupUi(this);
	ui.label->setText(s);
	link=str;
	connect(ui.copy,SIGNAL(clicked()),this,SLOT(copyclicked()));
	connect(ui.del,SIGNAL(clicked()),this,SLOT(delclicked()));
}
void uploadedfileatomic::copyclicked(){
	QClipboard *c=QApplication::clipboard();
	c->setText(link);
}
void uploadedfileatomic::delclicked(){
	QFile file(QApplication::applicationDirPath() + "/uploads.dat");
	if(file.open(QIODevice::ReadOnly)){
		QString s;
		QString text;
		while(!file.atEnd()){
			s=file.readLine();
			if(s==ui.label->text()){
				file.readLine();
				file.readLine();
				file.readLine();
			}
			else
				text.append(s);
		}
		file.close();
		if(file.open(QFile::Truncate | QFile::WriteOnly)){
			QTextStream ts(&file);
			ts<<text;
		}
	}
	this->setParent(0);
	this->deleteLater();
}
uploadedfileatomic::~uploadedfileatomic()
{

}
