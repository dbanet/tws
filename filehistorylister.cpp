#include "filehistorylister.h"
#include "uploadedfileatomic.h"
#include<QFile>
filehistorylister::filehistorylister(QFrame *parent) :
	QFrame(parent) {
	this->setObjectName("filehistory");
	this->setAttribute(Qt::WA_DeleteOnClose);
	ui.setupUi(this);
	QFile file(QApplication::applicationDirPath() + "/uploads.dat");
	if (file.open(QIODevice::ReadOnly)) {
		uploadedfileatomic *u;
		QString s;
		while (!file.atEnd()) {
			u=new uploadedfileatomic(QString(file.readLine()),QString(file.readLine()));
			ui.verticalLayout_2->addWidget(u);
			file.readLine();
			file.readLine();
		}
	}
	connect(ui.close, SIGNAL(clicked()),this, SLOT(closeclicked()));
}
void filehistorylister::closeclicked() {
	this->close();
}

filehistorylister::~filehistorylister() {

}
