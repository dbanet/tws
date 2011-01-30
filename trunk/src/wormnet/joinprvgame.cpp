#include<QStringList>
#include<QFileDialog>

#include"joinprvgame.h"
#include"sqlsettings.h"

joinprvgame::joinprvgame(const QString &a,const QString &b)
    : chan(b),anchor(a)
{
    this->setObjectName("normalwidget");
    ui.setupUi(this);
    QStringList sl = S_S.getstringlist("joinstrings");
    ui.icons->clear();
    ui.icons->addItems(sl);
    this->setWindowTitle(tr("Join game with."));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));
}
void joinprvgame::addclicked() {
#ifdef Q_WS_S60
    QString file;
    return;
#endif
#ifdef Q_WS_MAC
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_X11
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_WIN
    QString file = QFileDialog::getOpenFileName(this, tr(
            "Choose a Program."), "/home", "*.exe *.com");
#endif
    QStringList sl = S_S.getstringlist("joinstrings");
    if (file != "") {
        if (!sl.contains(file) && file != "") {
            sl.insert(0, file);
            S_S.set("joinstrings", sl);
            ui.icons->clear();
            ui.icons->addItems(sl);
        } else if (sl.contains(file) && file != "") {
            sl.move(sl.indexOf(file), 0);
            S_S.set("joinstrings", sl);
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
