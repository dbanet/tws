#include "hostbox.h"
#include "settings.h"
#include "versioninfo.h"
#include "netcoupler.h"
#include "global_functions.h"

#include <QFileDialog>
#include <QPointer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QValidator>
#include <QDesktopServices>
#include <QUrl>

hostbox::hostbox(QString c, QWidget *parent) :
    QWidget(parent),channel(c) {
    ui.setupUi(this);
    setObjectName("normalwidget");

    ui.lgamename->setText(QObject::tr("Gamename"));
    ui.lgameip->setText(QObject::tr("GameIp"));

    QStringList sl = S_S.getStringList("joinstrings");
    ui.icons->addItems(sl);
    ui.cbip->setChecked(S_S.getbool("useacostumipforhosting"));
    ui.leip->setText(S_S.getString("costumipforhosting"));
    ui.cbwormnat2->setChecked (S_S.getbool ("cbwormnat2"));

    ui.legamename->setText(singleton<netcoupler>().nick);
    QString gamename=S_S.getString("legamename");
    if(!gamename.isEmpty())
        ui.legamename->setText(gamename);
    setWindowTitle(hostbox::tr("Create a public game in")+" " + channel);
    ui.chbsendhostinfotochan->setText(hostbox::tr("Send hostinfo to %1").arg(channel));

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));

    ui.chbsendhostinfotochan->setChecked(S_S.getbool("chbsendhostinfotochan"));
    ui.leplayername->setText(S_S.getString("leplayername"));
    ui.lehostport->setText(gethostportbyini());

    ui.legamename->installEventFilter(this);
    ui.lehostport->installEventFilter(this);
    ui.leplayername->installEventFilter(this);
//  resize(9,9);
}
bool hostbox::eventFilter(QObject *obj, QEvent *event){
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(keyEvent && keyEvent->type()==QEvent::KeyPress)
        if(keyEvent->key()==Qt::Key_Enter || keyEvent->key()==Qt::Key_Return){
            okclicked();
            return true;
        }
    return QWidget::eventFilter(obj, event);
}
void hostbox::showEvent(QShowEvent * /*event*/) {
}
void hostbox::addclicked() {
    QString file;
#ifdef Q_OS_UNIX
    file = QFileDialog::getOpenFileName(this, tr(
                                            "Choose a desktop icon."), "/home", "Linux Desktop Icon (*.desktop);;WINE Executable File (*.exe *.cmd)");
#elif defined(Q_OS_WIN32) | defined(Q_OS_OS2)
    file = QFileDialog::getOpenFileName(this, tr(
                                            "Choose a Program."), "c:/", "*.exe *.com");
#else
    file = QFileDialog::getOpenFileName(this, tr("Choose a Program."));
#endif
    if (file != "") {
        QStringList sl = S_S.getStringList("joinstrings");
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
void hostbox::okclicked() {
    S_S.transaction();
    if(ui.lehostport->text()!= S_S.getString("lehostport"))
        sethostport(ui.lehostport->text());
    S_S.set("leplayername", ui.leplayername->text().replace(' ','_'));
    S_S.set("chbsendhostinfotochan", ui.chbsendhostinfotochan->isChecked());
    S_S.set("useacostumipforhosting", ui.cbip->isChecked());
    S_S.set("costumipforhosting", ui.leip->text());
    S_S.set("legamename", ui.legamename->text().replace(' ',(char)0xA0));
    S_S.set("lehostport", ui.lehostport->text());
    S_S.set("cbwormnat2",ui.cbwormnat2->isChecked());
    gamename = ui.legamename->text();
    gamename.replace(' ',(char)0xA0);
    pwd = ui.lepassword->text();
    icon = ui.icons->currentText();    
    S_S.commit();
    QStringList sl = S_S.getStringList("joinstrings");
    if (!sl.isEmpty()) {
        sl.move(sl.indexOf(ui.icons->currentText()), 0);
        S_S.set("joinstrings", sl);

        if( (ui.cbwormnat2->isChecked () && verifywormnat())){
            emit sigok();
            close();
        }
        if(!ui.cbwormnat2->isChecked ()){
            emit sigok();
            close();
        }
    } else
        QMessageBox::warning (this,QObject::tr("Warning"),tr("Please add a Game executable!"));
}
bool hostbox::verifywormnat(){
    QString s = S_S.getStringList("joinstrings").first();
    auto gv = getFileVersion(s);
    s = QFileInfo(s).path();
    if(!QFile::exists(s+ "/wkWormNAT2.dll")) {
        int button = QMessageBox::question(0, QObject::tr("Warning"), tr(
            "Hosting via WormNAT2 requires installing two DLL files "
            "into your Worms Armageddon folder: wkWormNAT2.dll and "
            "madCHook.dll. Older components of WormNAT2(Ex) will be "
            "removed, if found.\n"
            "WormKit module loading (/wk) will be temporarily enabled each "
            "time you host a game with WormNAT2 on W:A 3.7.0.0 and newer.\n"
            "Are you sure you want to continue?"
        ));
        if (button != QMessageBox::Ok)
            return false;
        QFile::copy("Wkfiles/wkWormNAT2.dll", s + "/wkWormNAT2.dll");
        if (gv && gv < "3.7.0.0")
            QMessageBox::warning(0, QObject::tr("Warning"), tr(
                "Your version of the game (%1) is outdated. You might "
                "see some \"unrecognized command-line parameter\" warnings "
                "when hosting a game with WormNAT2. In order to get rid of "
                "them, you need to upgrade Worms Armageddon to version 3.7 "
                "or newer, or ask WormNAT2's authors to implement a workaround "
                "for you. This dialog box is only shown once during installation."
            ).arg(gv.str()));
    }
    else {
        auto nv = getFileVersion("Wkfiles/wkWormNAT2.dll");
        auto ov = getFileVersion(s+ "/wkWormNAT2.dll");
        if (nv > ov) {
            int button = QMessageBox::information(0, QObject::tr("Information"), tr(
                "An older version of WormNAT2 (%1) was detected in your game's "
                "directory. It will now be upgraded to version %2 for "
                "improved stability and enhanced feature set, and might be required for "
                "snooper hosting to work at all. This is a required step, and you must "
                "agree in order to keep using WormNAT2."
            ).arg(ov.str(), nv.str()));
            if (button != QMessageBox::Ok)
                return false;
            QFile::remove(s+ "/wkWormNAT2.dll");
            QFile::copy("Wkfiles/wkWormNAT2.dll", s + "/wkWormNAT2.dll");
        }
    }
    if(!QFile::exists (s+ "/madCHook.dll")) {
        QFile::copy ("Wkfiles/madCHook.dll", s + "/madCHook.dll");
    }
    if(QFile::exists (s + "/wkWormNAT2Ex.dll")) {
        QFile::remove (s + "/wkWormNAT2Ex.dll");
    }
    if(QFile::exists (s + "/wkPackets.dll")) {
        QFile::remove (s + "/wkPackets.dll");
    }
    return true;
}
void hostbox::cancelclicked() {
    close();
}
hostbox::~hostbox() {

}
