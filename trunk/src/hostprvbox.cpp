#include "hostprvbox.h"
#include "snpsettings.h"
#include"netcoupler.h"
#include <QFileDialog>
#include<QDebug>
#include<QPointer>
#include"global_functions.h"
extern QPointer<netcoupler> net;
hostprvbox::hostprvbox(QWidget *parent) :
	QWidget(parent) {
    this->setObjectName("normalwidget");
    ui.setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QStringList sl = singleton<snpsettings>().map.value("joinstrings").value<QStringList> ();
    ui.icons->addItems(sl);
    ui.cbip->setChecked(singleton<snpsettings>().map["useacostumipforhosting"].value<bool> ());
    ui.leip->setText(singleton<snpsettings>().map["costumipforhosting"].value<QString>());
    this->setWindowTitle(tr("Create a private game."));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.okjust, SIGNAL(clicked()),this, SLOT(okjustclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));
    connect(ui.pbscheme, SIGNAL(clicked()),this, SLOT(showschemeclicked()));
    connect(ui.clip, SIGNAL(clicked()),this, SLOT(copyslot()));
    ui.lehostport->setText(gethostport());
}
void hostprvbox::showEvent(QShowEvent * /*event*/) {
}
void hostprvbox::addclicked() {
#ifdef Q_WS_MAC
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a Program."), "/home", "*.*");
#endif
#ifdef Q_WS_X11
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_WIN
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a desktop icon."), "c:/", "*.exe *.com");
#endif
    QStringList sl = singleton<snpsettings>().map.value("joinstrings").value<QStringList> ();
    if (file != "") {
        if (!sl.contains(file) && file != "") {
            sl.insert(0, file);
            singleton<snpsettings>().map["joinstrings"] = sl;
            ui.icons->clear();
            ui.icons->addItems(sl);
            singleton<snpsettings>().safe();
        } else if (sl.contains(file) && file != "") {
            sl.move(sl.indexOf(file), 0);
            singleton<snpsettings>().map["joinstrings"] = sl;
            singleton<snpsettings>().safe();
        }
    }
}
void hostprvbox::okclicked() {
    sethostport(ui.lehostport->text());
    icon = ui.icons->currentText();
    QStringList sl = singleton<snpsettings>().map.value("joinstrings").value<QStringList> ();
    singleton<snpsettings>().map["useacostumipforhosting"].setValue<bool> (
            ui.cbip->isChecked());
    singleton<snpsettings>().map["costumipforhosting"].setValue<QString>(ui.leip->text());
    if (!sl.isEmpty()) {
        sl.move(sl.indexOf(ui.icons->currentText()), 0);
        singleton<snpsettings>().map["joinstrings"] = sl;
        singleton<snpsettings>().safe();
        net->sendprvhosttobuddys(ui.lescheme->text());emit
		sigok(ui.lescheme->text());
        this->close();
        this->deleteLater();
    }
}
void hostprvbox::okjustclicked() {
    sethostport(ui.lehostport->text());
    icon = ui.icons->currentText();
    QStringList sl = singleton<snpsettings>().map.value("joinstrings").value<QStringList> ();
    singleton<snpsettings>().map["useacostumipforhosting"].setValue<bool> (
            ui.cbip->isChecked());
    singleton<snpsettings>().map["costumipforhosting"].setValue<QString>(ui.leip->text());
    if (!sl.isEmpty()) {
        sl.move(sl.indexOf(ui.icons->currentText()), 0);
        singleton<snpsettings>().map["joinstrings"] = sl;
        singleton<snpsettings>().safe();emit
		sigok(ui.lescheme->text());
        this->close();
        this->deleteLater();
    }
}
void hostprvbox::showschemeclicked() {
    QPixmap p(QApplication::applicationDirPath()
              + "/snppictures/channelscheme-explaination.png");
    schemelabel.setPixmap(p);
    schemelabel.resize(p.size());
    schemelabel.show();
    schemelabel.setWindowTitle(tr("A description about the channel schemes."));
}
void hostprvbox::cancelclicked() {
    this->close();
    this->deleteLater();
}
void hostprvbox::copyslot() {
    sethostport(ui.lehostport->text());
    net->copyprvhosttoclipboard(ui.lescheme->text());
}
hostprvbox::~hostprvbox() {

}
