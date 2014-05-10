#include <QStringList>
#include <QFileDialog>

#include "joinprvgame.h"
#include "settings.h"

joinprvgame::joinprvgame(const QString &a,const QString &b)
    : chan(b),anchor(a)
{
     setObjectName("normalwidget");
    ui.setupUi(this);
    QStringList sl = S_S.getStringList("joinstrings");
    ui.icons->clear();
    ui.icons->addItems(sl);
     setWindowTitle(tr("Join game with."));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));
}
void joinprvgame::addclicked() {
    QString file;
#ifdef Q_OS_UNIX
    file = QFileDialog::getOpenFileName(this, tr(
            "Choose a desktop icon."), "/home", "*.desktop");
#elif defined(Q_OS_WIN32) | defined(Q_OS_OS2)
    file = QFileDialog::getOpenFileName(this, tr(
            "Choose a Program."), "/home", "*.exe *.com");
#else
    file = QFileDialog::getOpenFileName(this, tr(
            "Choose a Program."));
#endif
    QStringList sl = S_S.getStringList("joinstrings");
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
     close();
     deleteLater();
}
void joinprvgame::cancelclicked(){
     close();
     deleteLater();
}

joinprvgame::~joinprvgame()
{

}
