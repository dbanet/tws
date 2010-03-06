#include "playername.h"
#include<QFile>
#include<QProcess>
#include<QTextStream>
QString playername::nick = "";
playername::playername(QWidget *parent) :
	QWidget(parent) {
	this->setObjectName("normalwidget");
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);
	QStringList env = QProcess::systemEnvironment();
	QString systemroot;
	foreach(QString s,env) {
			if (s.contains("SystemRoot=")) {
				systemroot = s;
				systemroot = systemroot.split("=").last();
			}
		}
	QFile winini(systemroot + "/win.ini");
	QString name;
	if (winini.exists() && winini.open(QIODevice::ReadOnly)) {
		QStringList sl = QString(winini.readAll()).split("\n");
		foreach(QString s,sl) {
				if (s.contains("PlayerName=")) {
					if (s.split("=", QString::SkipEmptyParts).size() == 2) {
						name = s.split("=", QString::SkipEmptyParts).last();
					}
				}
			}
	}
	ui.lineEdit->setText(name);
	nick=name;
	ui.label->setText(
			tr("This is your Playername in a game.\n"
				"This name supports more letters then the wormnet name.\n"
				"If this string is empty, worms will probably not host or join a game."));
	connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
}
void playername::okclicked() {
	if (ui.lineEdit->text() != "") {
		nick=ui.lineEdit->text();
		QStringList env = QProcess::systemEnvironment();
		QString systemroot;
		foreach(QString s,env) {
				if (s.contains("SystemRoot=")) {
					systemroot = s;
					systemroot = systemroot.split("=").last();
				}
			}
		QFile winini(systemroot + "/win.ini");
		QString name;
		if (winini.exists() && winini.open(QIODevice::ReadOnly)) {
			QStringList sl = QString(winini.readAll()).split("\n");
			int i = 0;
			foreach(QString s,sl) {
					if (s.contains("PlayerName=")) {
						s = "PlayerName=" + ui.lineEdit->text();
					}
					sl[i] = s;
					i++;
				}
			winini.close();
			QString s = qPrintable(sl.join(""));
			if (winini.open(QFile::WriteOnly | QFile::Truncate)) {
				QTextStream ts(&winini);
				foreach(QString s,sl) {
						s = s.simplified();
						ts << s + "\n";
					}
			}
		}
	}
	this->close();
}

playername::~playername() {

}
