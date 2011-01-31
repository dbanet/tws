#include"settingswindow.h"
#include"netcoupler.h"
#include"combobox_wrapper.h"
#include"myDebug.h"
#include "ui_settingswindow.h"
#include<QLineEdit>
#include<QCheckBox>
#include<QFile>
#include<QDir>
#include<QSpinBox>
#include<QFileDialog>
#include<QPointer>
#include<stdexcept>
settingswindow::settingswindow(){
    setObjectName("normalwidget");
    ui=new Ui::settingswindowClass;
    ui->setupUi(this);
    ui->tabWidget->setObjectName("bgchatwindowwidget");
    ui->tab->setObjectName("bgchatwindowwidget");
    ui->tab_2->setObjectName("bgchatwindowwidget");
    ui->tab_3->setObjectName("bgchatwindowwidget");
    combobox_wrapper *cw=new combobox_wrapper(this);
    cw->setObjectName("combobox_wrapper");
    ui->costum_combobox_layout->addWidget(cw);
    QString s;
    foreach(QObject *o, findChildren<QObject*>()) { //first loaded with default values then initialized with settingswindowini
        s = o->objectName();
        if (s != "qt_spinbox_lineedit") {
            if (qobject_cast<QLineEdit*> (o) != 0) {
                map[s] = qobject_cast<QLineEdit*> (o)->text();
                objectnames << s;
            } else if (qobject_cast<QCheckBox*> (o) != 0) {
                map[s] = qobject_cast<QCheckBox*> (o)->isChecked();
                objectnames << s;
            } else if (qobject_cast<QSpinBox*> (o)) {
                map[s] = qobject_cast<QSpinBox*> (o)->value();
                objectnames << s;
            } else if (qobject_cast<combobox_wrapper*> (o)) {
                map[s] = qobject_cast<combobox_wrapper*> (o)->get();
                objectnames << s;
            }
        }
    }
    load();
    connect(ui->ok, SIGNAL(clicked()),this, SLOT(ok()));
    connect(ui->cancel, SIGNAL(clicked()),this, SLOT(cancel()));
    connect(ui->pbbuddychatmessage, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbbuddychatwindowopened, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbnormalchatmessage, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbstartup, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbbuddyarrives, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbbuddyleaves, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbhighlightning, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbbuddyhosts, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui->pbcostumword, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    ui->tabWidget->setCurrentIndex(0);
}
void settingswindow::load() {
    QFile f(QApplication::applicationDirPath() + "/snpini/settingswindowini");
    if (!f.open(QFile::ReadOnly))
        loadDefaults();
    else {
        QDataStream ds(&f);
        ds.setVersion(QDataStream::Qt_4_3);
        ds >> map;
    }
    checkValidEntries();
    QObject *o;
    foreach(QString s,objectnames) {
        o =  findChild<QObject*> (s);
        Q_ASSERT(o!=0);
        if (qobject_cast<QLineEdit*> (o) != 0) {
            qobject_cast<QLineEdit*> (o)->setText(
                    map[s].value<QString> ());
        } else if (qobject_cast<QCheckBox*> (o) != 0) {
            qobject_cast<QCheckBox*> (o)->setChecked(
                    map[s].value<bool> ());
        } else if (qobject_cast<QSpinBox*> (o)) {
            qobject_cast<QSpinBox*> (o)->setValue(map[s].value<int> ());
        }   else if (qobject_cast<combobox_wrapper*> (o)) {
            qobject_cast<combobox_wrapper*> (o)->set(map[s].value<QStringList> ());
        }
    }   
}
void settingswindow::safe() {
    QFile f(QApplication::applicationDirPath() + "/snpini/settingswindowini");
    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
        QDataStream ds(&f);
        ds.setVersion(QDataStream::Qt_4_3);
        ds << map;
    }
}
void settingswindow::ok() {
    QObject *o;
    foreach(QString s,objectnames) {
        o =  findChild<QObject*> (s);
        Q_ASSERT(o!=0);
        if (qobject_cast<QLineEdit*> (o) != 0) {
            map[s].setValue<QString> (qobject_cast<QLineEdit*> (o)->text());
        } else if (qobject_cast<QCheckBox*> (o) != 0) {
            map[s].setValue<bool> (
                    qobject_cast<QCheckBox*> (o)->isChecked());
        } else if (qobject_cast<QSpinBox*> (o)) {
            map[s].setValue<int> (qobject_cast<QSpinBox*> (o)->value());
        } else if (qobject_cast<combobox_wrapper*> (o)) {
            map[s].setValue<QStringList> (qobject_cast<combobox_wrapper*> (o)->get());
        }
    }
    safe();
    close();
    singleton<netcoupler>().settingswindowemitfunktion();
}
void settingswindow::cancel() {
     close();
}
void settingswindow::soundoptionbuttonslot() {
    QPushButton *pb = qobject_cast<QPushButton*> (sender());
    Q_ASSERT(pb!=0);
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Choose a Sound (mp3 or wav) file for this event"),
                                                QApplication::applicationDirPath() + "/wav");
    if (file == QString())
        return;
    if (pb->objectName() == "pbbuddychatmessage") {
        ui->lebuddychatmessage->setText(file);
    } else if (pb->objectName() == "pbbuddychatwindowopened") {
        ui->lebuddychatwindowsopened->setText(file);
    } else if (pb->objectName() == "pbnormalchatmessage") {
        ui->lenormalchatmessage->setText(file);
    } else if (pb->objectName() == "pbstartup") {
        ui->lestartup->setText(file);
    } else if (pb->objectName() == "pbbuddyarrives") {
        ui->lebuddyarrives->setText(file);
    } else if (pb->objectName() == "pbbuddyleaves") {
        ui->lebuddyleaves->setText(file);
    } else if (pb->objectName() == "pbhighlightning") {
        ui->lehighlightning->setText(file);
    } else if (pb->objectName() == "pbbuddyhosts")
        ui->lehostsound->setText(file);
    else if (pb->objectName() == "pbcostumword")
        ui->lecostumword->setText(file);
    else
        throw std::runtime_error("settingswindow::soundoptionbuttonslot()");
}
settingswindow::~settingswindow() {

}
const QVariant settingswindow::from_map(const QString &s) const{
    return map[s];
}
void settingswindow::to_map(const QString &s, const QVariant &v){
    map[s]=v;
    if(s=="cbcostumword")
        ui->cbcostumword->setChecked(v.toBool());
    else if(s=="chbsmileys")
        ui->chbsmileys->setChecked(v.toBool());
    else if(s=="cbdontplaysound")
        ui->cbdontplaysound->setChecked(v.toBool());
    else if(s=="cbdontshowballoons")
        ui->cbdontshowballoons->setChecked(v.toBool());
    else if(s=="cbdontsortinchannels")
        ui->cbdontsortinchannels->setChecked(v.toBool());
    else
        throw std::runtime_error("\nvoid settingswindow::to_map(const QString &s, const QVariant &v)\n");
    safe();
}
void settingswindow::loadDefaults(){
    map["leawaystring"].setValue<QString>("I'm in a Game!");
    map["lebackmessage"].setValue<QString>("I'm back.");

    map["lestartup"].setValue<QString>("wav/startup.mp3");
    map["lebuddyarrives"].setValue<QString>("wav/buddyarrives.mp3");
    map["lebuddyleaves"].setValue<QString>("wav/buddyleaves.mp3");
    map["lebuddychatmessage"].setValue<QString>("wav/buddymessage.mp3");
    map["lebuddychatwindowsopened"].setValue<QString>("wav/buddychatwindowopened.mp3");
    map["lenormalchatmessage"].setValue<QString>("wav/normalprivmsg.mp3");
    map["lehighlightning"].setValue<QString>("wav/highlightningsound.mp3");
    map["lecostumword"].setValue<QString>("wav/costumword.mp3");
    map["lehostsound"].setValue<QString>("wav/buddyhosts.mp3");

    map["cbalertmeonnotice"].setValue<bool>(true);
    map["cbalertfromnormal"].setValue<bool>(true);
    map["chbactionwhenjoining"].setValue<bool>(true);
    map["cbsetawaywhilegaming"].setValue<bool>(true);
    map["chbbacktonormals"].setValue<bool>(true);
    map["chbbacktobuddys"].setValue<bool>(true);
    map["cbignorysappearinchannel"].setValue<bool>(true);
    map["cbsafequerys"].setValue<bool>(true);
    map["cbopenbuddylistonstartup"].setValue<bool>(true);
    map["chbshowchannelchatinchatwindows"].setValue<bool>(true);
    map["cbservermessageinchannelwindows"].setValue<bool>(true);
    map["chbjoininfo"].setValue<bool>(true);
    map["chbpartinfo"].setValue<bool>(true);
    map["chbquitinfo"].setValue<bool>(true);
    map["chbbuddyballoonarives"].setValue<bool>(true);
    map["chbbuddyballoonleaves"].setValue<bool>(true);
    map["chbballoonprivmsg"].setValue<bool>(true);
    map["chbshowbaloonwhenbuddyhosts"].setValue<bool>(true);
    map["cbstartup"].setValue<bool>(true);
    map["cbbuddyarrives"].setValue<bool>(true);
    map["cbbuddyleaves"].setValue<bool>(true);
    map["cbplaybuddychatmessage"].setValue<bool>(true);
    map["cbplaybuddychatwindowopened"].setValue<bool>(true);
    map["cbplaynormalchatmessage"].setValue<bool>(true);
    map["cbhighlightning"].setValue<bool>(true);
    map["cbcostumword"].setValue<bool>(true);
    map["chbhostsound"].setValue<bool>(true);

    map["sbmaximumoftextblocksinlog"].setValue<int>(60);
    map["sbmaximumballonmessages"].setValue<int>(3);
    map["sbwhorepead"].setValue<int>(3000);
    map["sbhostrepead"].setValue<int>(15000);
    map["sbmaximumoftextblocks"].setValue<int>(500);
    map["sbsecureloggingrepeatdelay"].setValue<int>(10*100);
     map["cbonlyshowranksfromverifiedusers"]=true;
}
void settingswindow::checkValidEntries(){
    if(map["lestartup"].toString().startsWith("wav/"))
        map["lestartup"].setValue<QString>("wav/startup.mp3");

    if(map["lebuddyarrives"].toString().startsWith("wav/"))
        map["lebuddyarrives"].setValue<QString>("wav/buddyarrives.mp3");

    if(map["lebuddyleaves"].toString().startsWith("wav/"))
        map["lebuddyleaves"].setValue<QString>("wav/buddyleaves.mp3");

    if(map["lebuddychatmessage"].toString().startsWith("wav/"))
        map["lebuddychatmessage"].setValue<QString>("wav/buddymessage.mp3");

    if(map["lebuddychatwindowsopened"].toString().startsWith("wav/"))
        map["lebuddychatwindowsopened"].setValue<QString>("wav/buddychatwindowopened.mp3");

    if(map["lenormalchatmessage"].toString().startsWith("wav/"))
        map["lenormalchatmessage"].setValue<QString>("wav/normalprivmsg.mp3");

    if(map["lehighlightning"].toString().startsWith("wav/"))
        map["lehighlightning"].setValue<QString>("wav/highlightningsound.mp3");

    if(map["lecostumword"].toString().startsWith("wav/"))
        map["lecostumword"].setValue<QString>("wav/costumword.mp3");

    if(map["lehostsound"].toString().startsWith("wav/"))
        map["lehostsound"].setValue<QString>("wav/buddyhosts.mp3");
    if(map["sbsecureloggingrepeatdelay"].toInt()==0)
        map["sbsecureloggingrepeatdelay"].setValue<int>(10*1000);
    if(!map.contains("cbonlyshowranksfromverifiedusers"))
        map["cbonlyshowranksfromverifiedusers"]=true;    
}

void settingswindow::on_pbloadsoundpack_clicked()
{
    QString s=QFileDialog::getExistingDirectory(this,tr("Choose the directory from the soundpack."),"sounpacks");
    if(s.isEmpty())
        return;
    QDir dir=s;
    foreach(s,dir.entryList(QStringList()<<"*.*",QDir::Files)){
        if(s.contains("startup"))
            ui->lestartup->setText(dir.path()+"/"+s);
        else if(s.contains("buddyarrives"))
            ui->lebuddyarrives->setText(dir.path()+"/"+s);
        else if(s.contains("buddyleaves"))
            ui->lebuddyleaves->setText(dir.path()+"/"+s);
        else if(s.contains("buddymessage"))
            ui->lebuddychatmessage->setText(dir.path()+"/"+s);
        else if(s.contains("buddychatwindowopened"))
            ui->lebuddychatwindowsopened->setText(dir.path()+"/"+s);
        else if(s.contains("normalprivmsg"))
            ui->lenormalchatmessage->setText(dir.path()+"/"+s);
        else if(s.contains("highlightning"))
            ui->lehighlightning->setText(dir.path()+"/"+s);
        else if(s.contains("costumword"))
            ui->lecostumword->setText(dir.path()+"/"+s);
        else if(s.contains("buddyhosts"))
            ui->lehostsound->setText(dir.path()+"/"+s);

    }

}

void settingswindow::on_pbloaddefaultsounds_clicked()
{
    QDir dir("wav");
    foreach(QString s,dir.entryList(QStringList()<<"*.*",QDir::Files)){
        if(s.contains("startup"))
            ui->lestartup->setText(dir.path()+"/"+s);
        else if(s.contains("buddyarrives"))
            ui->lebuddyarrives->setText(dir.path()+"/"+s);
        else if(s.contains("buddyleaves"))
            ui->lebuddyleaves->setText(dir.path()+"/"+s);
        else if(s.contains("buddymessage"))
            ui->lebuddychatmessage->setText(dir.path()+"/"+s);
        else if(s.contains("buddychatwindowopened"))
            ui->lebuddychatwindowsopened->setText(dir.path()+"/"+s);
        else if(s.contains("normalprivmsg"))
            ui->lenormalchatmessage->setText(dir.path()+"/"+s);
        else if(s.contains("highlightning"))
            ui->lehighlightning->setText(dir.path()+"/"+s);
        else if(s.contains("costumword"))
            ui->lecostumword->setText(dir.path()+"/"+s);
        else if(s.contains("buddyhosts"))
            ui->lehostsound->setText(dir.path()+"/"+s);

    }
}
