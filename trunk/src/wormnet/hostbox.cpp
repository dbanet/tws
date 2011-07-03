#include"hostbox.h"
#include"settings.h"
#include"netcoupler.h"
#include"global_functions.h"

#include<QFileDialog>
#include<QPointer>
#include<QKeyEvent>
#include<QMessageBox>
#include<QValidator>
#include<QDesktopServices>
#include<QUrl>

hostbox::hostbox(QString c, QWidget *parent) :
    QWidget(parent),channel(c) {
    ui.setupUi(this);
    setObjectName("normalwidget");

    ui.lgamename->setText(QObject::tr("Gamename"));
    ui.lgameip->setText(QObject::tr("GameIp"));

    QStringList sl = S_S.getstringlist("joinstrings");
    ui.icons->addItems(sl);
    ui.cbip->setChecked(S_S.getbool("useacostumipforhosting"));
    ui.leip->setText(S_S.getstring("costumipforhosting"));
    ui.cbwormnat2->setChecked (S_S.getbool ("cbwormnat2"));

    ui.legamename->setText(singleton<netcoupler>().nick);
    QString gamename=S_S.getstring("legamename");
    if(!gamename.isEmpty())
        ui.legamename->setText(gamename);
    setWindowTitle(tr("Create a public game in")+" " + channel + ".");
    ui.chbsendhostinfotochan->setText(tr("Send hostinfo to %1.").arg(channel));

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui.add, SIGNAL(clicked()),this, SLOT(addclicked()));
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancelclicked()));

    ui.chbsendhostinfotochan->setChecked(S_S.getbool("chbsendhostinfotochan"));
    ui.leplayername->setText(S_S.getstring("leplayername"));
    ui.lehostport->setText(gethostportbyini());

    ui.legamename->installEventFilter(this);
    ui.lehostport->installEventFilter(this);
    ui.leplayername->installEventFilter(this);
    resize(9,9);
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
#ifdef Q_WS_QWS
    error;
#endif
#ifdef Q_WS_MAC
    file = QFileDialog::getOpenFileName(this, tr(
                                            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_X11
    file = QFileDialog::getOpenFileName(this, tr(
                                            "Choose a desktop icon."), "/home", "*.desktop");
#endif
#ifdef Q_WS_WIN
    file = QFileDialog::getOpenFileName(this, tr(
                                            "Choose a Program."), "c:/", "*.exe *.com");
#endif       
    if (file != "") {
        QStringList sl = S_S.getstringlist("joinstrings");
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
    if(ui.lehostport->text()!= S_S.getstring("lehostport"))
        sethostport(ui.lehostport->text());
    S_S.set("leplayername", ui.leplayername->text().replace(" ","_"));
    S_S.set("chbsendhostinfotochan", ui.chbsendhostinfotochan->isChecked());
    S_S.set("useacostumipforhosting", ui.cbip->isChecked());
    S_S.set("costumipforhosting", ui.leip->text());
    S_S.set("legamename", ui.legamename->text().replace(" ","_"));
    S_S.set("lehostport", ui.lehostport->text());
    S_S.set("cbwormnat2",ui.cbwormnat2->isChecked ());    
    gamename = ui.legamename->text();
    gamename.replace(" ", "_");
    pwd = ui.lepassword->text();
    icon = ui.icons->currentText();    
    S_S.commit();
    QStringList sl = S_S.getstringlist("joinstrings");
    if (!sl.isEmpty()) {
        sl.move(sl.indexOf(ui.icons->currentText()), 0);
        S_S.set("joinstrings", sl);

        if( (ui.cbwormnat2->isChecked () && verifywormnat())){
            //            if(verifywormnat()){
            //                QMessageBox::information (this,QObject::tr("Information"), tr("Sorry for inconvenience but CyberShadow adviced me to post this: \n"
            //                                                                              "This is a reminder message to remind you that WormNAT2\n"
            //                                                                              "is a free service. Using WormNAT2 tunnels all data\n"
            //                                                                              "through a proxy server hosted by the community, thus\n"
            //                                                                              "consuming bandwidth and other resources. Therefore,\n"
            //                                                                              "we'd like to ask you to only use WormNAT2 when you\n"
            //                                                                              "have already tried configuring hosting the proper way.\n"
            //                                                                              "Don't forget that you can find instructions on how\n"
            //                                                                              "to set up hosting here\n"
            //                                                                              "http://worms2d.info/Hosting"));
            //            }
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
    QString s=S_S.getstringlist("joinstrings").first ();
    s=QFileInfo(s).path ();
    QString msg=tr("Using Wormnat2 hosting requieres some wk files to be moved to you Worms Armageddon folder, these files may replace old files."
                   "Do you want that?");
    if(!QFile::exists (s+ "/madCHook.dll")) {
        int button=QMessageBox::question (0, QObject::tr("Warning"), msg);
        if(button!=QMessageBox::Ok)
            return false;
        QFile::copy ("Wkfiles/dsound.dll", s + "/dsound.dll");
        QFile::copy ("Wkfiles/wkPackets.dll", s + "/wkPackets.dll");
        QFile::copy ("Wkfiles/borlndmm.dll", s + "/borlndmm.dll");
        QFile::copy ("Wkfiles/madCHook.dll", s + "/madCHook.dll");
    } else if(!QFile::exists (s+ "/wkPackets.dll")) {
        int button=QMessageBox::question (0, QObject::tr("Warning"), msg);
        if(button!=QMessageBox::Ok)
            return false;
        QFile::copy ("Wkfiles/wkPackets.dll", s + "/wkPackets.dll");
        QFile::copy ("Wkfiles/borlndmm.dll", s + "/borlndmm.dll");
    }
    if(QFile::exists (s + "/wkWormNAT2Ex.dll"))
        return true;
    int button=QMessageBox::question (0, QObject::tr("Warning"), msg);
    if(button!=QMessageBox::Ok)
        return false;
    QFile::remove (s+ "/wkWormNAT2.dll");
    QFile::copy ("Wkfiles/wkWormNAT2Ex.dll", s + "/wkWormNAT2Ex.dll");
    return true;
}
void hostbox::cancelclicked() {
    close();
}
hostbox::~hostbox() {

}
