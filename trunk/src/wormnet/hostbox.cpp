#include "hostbox.h"
#include "snpsettings.h"
#include"netcoupler.h"
#include"global_functions.h"
#include <QFileDialog>
#include<QPointer>
#include<QKeyEvent>
#include<QMessageBox>
#include<QValidator>
hostbox::hostbox(QString c, QWidget *parent) :
        QWidget(parent),channel(c) {
    ui.setupUi(this);
    setObjectName("normalwidget");

    ui.lgamename->setText(QObject::tr("Gamename"));
    ui.lgameip->setText(QObject::tr("GameIp"));

    QStringList sl = S_S.map.value("joinstrings").value<QStringList> ();
    ui.icons->addItems(sl);
    ui.cbip->setChecked(S_S.map["useacostumipforhosting"].value<bool> ());
    ui.leip->setText(S_S.map["costumipforhosting"].value<QString>());

    ui.legamename->setText(singleton<netcoupler>().nick);
    QString gamename=S_S.map["legamename"].toString();        
    if(!gamename.isEmpty())
        ui.legamename->setText(gamename);
    setWindowTitle(tr("Create a public game in")+" " + channel + ".");
    ui.chbsendhostinfotochan->setText(tr("Send hostinfo to %1.").arg(channel));

    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));    

    ui.chbsendhostinfotochan->setChecked(S_S.map["chbsendhostinfotochan"].toBool());
    ui.leplayername->setText(S_S.map["leplayername"].toString());
    ui.lehostport->setText(gethostport());

    ui.legamename->installEventFilter(this);
    ui.lehostport->installEventFilter(this);
    ui.leplayername->installEventFilter(this);
    resize(9,9);
}
bool hostbox::eventFilter(QObject *obj, QEvent *event){
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(keyEvent && keyEvent->type()==QEvent::KeyPress)
        if(keyEvent->key()==Qt::Key_Enter || keyEvent->key()==Qt::Key_Return){
        this->okclicked();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}
void hostbox::showEvent(QShowEvent * /*event*/) {
}
void hostbox::addclicked() {
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
            "Choose a Program."), "c:/", "*.exe *.com");
#endif
    QStringList sl = S_S.map.value("joinstrings").value<QStringList> ();
    if (file != "") {
        if (!sl.contains(file) && file != "") {
            sl.insert(0, file);
            S_S.map["joinstrings"] = sl;
            ui.icons->clear();
            ui.icons->addItems(sl);
            S_S.safe();
        } else if (sl.contains(file) && file != "") {
            sl.move(sl.indexOf(file), 0);
            S_S.map["joinstrings"] = sl;
            S_S.safe();
        }
    }
}
void hostbox::okclicked() {
    sethostport(ui.lehostport->text());
    S_S.map["leplayername"].setValue<QString> (ui.leplayername->text().replace(" ","_"));
    S_S.map["chbsendhostinfotochan"] = ui.chbsendhostinfotochan->isChecked();    
    S_S.map["useacostumipforhosting"].setValue<bool> (ui.cbip->isChecked());
    S_S.map["costumipforhosting"].setValue<QString>(ui.leip->text());
    S_S.map["legamename"]=ui.legamename->text().replace(" ","_");
    gamename = ui.legamename->text();
    gamename.replace(" ", "_");
    pwd = ui.lepassword->text();
    icon = ui.icons->currentText();
    QStringList sl = S_S.map.value("joinstrings").value<QStringList> ();
    if (!sl.isEmpty()) {
        sl.move(sl.indexOf(ui.icons->currentText()), 0);
        S_S.map["joinstrings"] = sl;
        emit sigok();
    }
    S_S.safe();
    this->close();
}
void hostbox::cancelclicked() {
    this->close();
    this->deleteLater();
}
hostbox::~hostbox() {

}
