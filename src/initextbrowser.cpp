#include "initextbrowser.h"
#include<QHBoxLayout>
#include<QFile>
#include<QTextStream>
#include<QDir>
#include<QDebug>
#include<QTextCharFormat>
#include<QColor>
initextbrowser::initextbrowser(QWidget *parent) :
	QWidget(parent) {
	ui.setupUi(this);
	l = new QHBoxLayout(this);
	l->addWidget(ui.verticalLayoutWidget);
	this->setWindowTitle(tr("Edit your ini file."));
	connect(ui.save, SIGNAL(clicked()),this, SLOT(save()));
}
void initextbrowser::start(const QString &s) {
	ui.textBrowser->clear();
	inifilename = s;
	QFile f(QApplication::applicationDirPath() + QDir::separator() + "snpini"
			+ QDir::separator() + inifilename);
	f.open(QFile::ReadOnly);
	QTextStream ts(&f);
	QString str;
	QTextCharFormat a;
	ui.textBrowser->setFont(QFont("Times", 12, QFont::Bold));
	while (!ts.atEnd()) {
		str = ts.readLine();
		if (str.startsWith("[")) {
			a.setForeground(QBrush(Qt::blue));
			ui.textBrowser->setCurrentCharFormat(a);
			ui.textBrowser->append(str);
		} else if (str.startsWith("//img://")) {
			a.setForeground(QBrush(QColor(55, 155, 55)));
			ui.textBrowser->setCurrentCharFormat(a);
			ui.textBrowser->append(str);
			str.remove("//img://");
			ui.textBrowser->append("///");
			QImage img(QApplication::applicationDirPath() + QDir::separator()
							+ "snppictures" + QDir::separator() + str);
			ui.textBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(str), img);
			ui.textBrowser->textCursor().insertImage(str);
		} else if (str.startsWith("//")) {
			a.setForeground(QBrush(QColor(55, 155, 55)));
			ui.textBrowser->setCurrentCharFormat(a);
			ui.textBrowser->append(str);
		} else {
			a.setForeground(QBrush(Qt::black));
			ui.textBrowser->setCurrentCharFormat(a);
			ui.textBrowser->append(str);
		}
	}
}
void initextbrowser::save() {
	QFile f(QApplication::applicationDirPath() + QDir::separator() + "snpini"
			+ QDir::separator() + inifilename);
	f.open(QFile::WriteOnly | QFile::Truncate);
	QTextStream ts(&f);
	QStringList sl=ui.textBrowser->toPlainText().split("\n");
	QStringList sl0;
	foreach(QString s,sl){
		if(!s.startsWith("///"))
			sl0<<s;;
	}
	ts <<sl0.join("\n");
	this->close();
}
initextbrowser::~initextbrowser() {

}
