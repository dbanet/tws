#include "joinprvgame.h"
#include<QStringList>
#include<QFileDialog>
#include "snpsettings.h"
#include<QDebug>
joinprvgame::joinprvgame(const QString &a,const QString &b)
    : chan(b),anchor(a)
{
    this->setObjectName("normalwidget");
    ui.setupUi(this);
    QStringList sl = singleton<snpsettings>().map.value("joinstrings").value<QStringList> ();
    ui.icons->clear();
    ui.icons->addItems(sl);
    this->setWindowTitle(tr("Join game with."));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));
}
void joinprvgame::addclicked() {
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
            "Choose a Program."), "/home", "*.exe *.com");
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
void joinprvgame::okclicked(){
    emit sigjoingamelink(anchor);
    this->close();
    this->deleteLater();
}
void joinprvgame::cancelclicked(){
    this->close();
    this->deleteLater();
}

joinprvgame::~joinprvgame()
{

}
