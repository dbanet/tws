#include"settingswindow.h"
#include"netcoupler.h"
#include"combobox_wrapper.h"
#include"myDebug.h"
#include"ui_settingswindow.h"

#include<QLineEdit>
#include<QCheckBox>
#include<QFile>
#include<QDir>
#include<QSpinBox>
#include<QFileDialog>
#include<QPointer>
#include<stdexcept>

settingswindow::settingswindow() {
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
    foreach(QObject *o, findChildren<QObject*>()) { //getting objectnames
        s = o->objectName();
        if (s != "qt_spinbox_lineedit")
            if (qobject_cast<QLineEdit*> (o) != 0
                || qobject_cast<QCheckBox*> (o) != 0
                || qobject_cast<QSpinBox*> (o)
                || qobject_cast<combobox_wrapper*> (o))
                objectnames << s;
    }
    QObject *o;
    foreach(QString key,objectnames) {
        o =  findChild<QObject*> (key);
        Q_ASSERT(o!=0);
        if (qobject_cast<QLineEdit*> (o) != 0) {
            qobject_cast<QLineEdit*> (o)->setText(S_S.getstring(key));
        } else if (qobject_cast<QCheckBox*> (o) != 0) {
            qobject_cast<QCheckBox*> (o)->setChecked(S_S.getbool(key));
        } else if (qobject_cast<QSpinBox*> (o)) {
            qobject_cast<QSpinBox*> (o)->setValue(S_S.getint(key));
        }   else if (qobject_cast<combobox_wrapper*> (o)) {
            qobject_cast<combobox_wrapper*> (o)->set(S_S.getstringlist(key));
        }
    }
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
void settingswindow::ok() {
    S_S.transaction();
    QObject *o;
    foreach(QString s,objectnames) {
        o =  findChild<QObject*> (s);
        Q_ASSERT(o!=0);
        if (qobject_cast<QLineEdit*> (o) != 0)
            S_S.set(s, qobject_cast<QLineEdit*> (o)->text());
        else if (qobject_cast<QCheckBox*> (o) != 0)
            S_S.set(s, qobject_cast<QCheckBox*> (o)->isChecked());
        else if (qobject_cast<QSpinBox*> (o))
            S_S.set(s, qobject_cast<QSpinBox*> (o)->value());
        else if (qobject_cast<combobox_wrapper*> (o))
            S_S.set(s, qobject_cast<combobox_wrapper*> (o)->get());
    }
    close();
    singleton<netcoupler>().settingswindowemitfunktion();
    S_S.commit();
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
void settingswindow::set(const QString &s, const QVariant &v){
    S_S.set(s, v);
    if(s=="cbcostumword")
        ui->cbcostumword->setChecked(v.toBool());
    else if(s=="chbsmileysinchatwindows")
        ui->chbsmileysinchatwindows->setChecked(v.toBool());
    else if(s=="showsmileysinchannels")
        ui->showsmileysinchannels->setChecked(v.toBool());
    else if(s=="cbdontplaysound")
        ui->cbdontplaysound->setChecked(v.toBool());
    else if(s=="cbshowballoons")
        ui->cbshowballoons->setChecked(v.toBool());    
    else
        myDebug()<<QString() + "\nvoid settingswindow::to_map(const QString &s, const QVariant &v)\n";
}
void settingswindow::on_pbloadsoundpack_clicked(){
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
void settingswindow::on_pbloaddefaultsounds_clicked(){
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
