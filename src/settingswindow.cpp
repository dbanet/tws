#include"settingswindow.h"
#include"netcoupler.h"
#include"combobox_wrapper.h"
#include<QLineEdit>
#include<QCheckBox>
#include<QFile>
#include<QDir>
#include<QSpinBox>
#include<QFileDialog>
#include<QDebug>
#include<QPointer>
#include<stdexcept>
extern QPointer<netcoupler> net;
settingswindow::settingswindow(){
    this->setObjectName("normalwidget");
    ui.setupUi(this);
    ui.tabWidget->setObjectName("bgchatwindowwidget");
    ui.tab->setObjectName("bgchatwindowwidget");
    ui.tab_2->setObjectName("bgchatwindowwidget");
    ui.tab_3->setObjectName("bgchatwindowwidget");
    ui.lebuddychatmessage->setText(QApplication::applicationDirPath()
                                   + "/wav/buddymessage.wav");
    ui.lebuddychatwindowsopened->setText(QApplication::applicationDirPath()
                                         + "/wav/buddychatwindowopened.wav");
    ui.lenormalchatmessage->setText(QApplication::applicationDirPath()
                                    + "/wav/normalprivmsg.wav");
    ui.lecostumword->setText(QApplication::applicationDirPath()
                                    + "/wav/costumword.mp3");
    combobox_wrapper *cw=new combobox_wrapper(this);
    cw->setObjectName("combobox_wrapper");
    ui.costum_combobox_layout->addWidget(cw);
    QString s;
    foreach(QObject *o,this->findChildren<QObject*>()) { //first loaded with default values then initialized with settingswindowini
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
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(ok()));
    connect(ui.cancel, SIGNAL(clicked()),this, SLOT(cancel()));
    connect(ui.pbbuddychatmessage, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbbuddychatwindowopened, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbnormalchatmessage, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbstartup, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbbuddyarrives, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbbuddyleaves, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbhighlightning, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbbuddyhosts, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    connect(ui.pbcostumword, SIGNAL(clicked()),this, SLOT(soundoptionbuttonslot()));
    ui.tabWidget->setCurrentIndex(0);
}
void settingswindow::load() {
    QFile f(QApplication::applicationDirPath() + QDir::separator() + "snpini"
            + QDir::separator() + "settingswindowini");
    if (f.open(QFile::ReadOnly)) {
        QDataStream ds(&f);
        ds.setVersion(QDataStream::Qt_4_3);
        ds >> map;
        QObject *o;
        foreach(QString s,objectnames) {
            o = this->findChild<QObject*> (s);
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
}
void settingswindow::safe() {
    QFile f(QApplication::applicationDirPath() + QDir::separator() + "snpini"
            + QDir::separator() + "settingswindowini");
    if (f.open(QFile::WriteOnly | QFile::Truncate)) {
        QDataStream ds(&f);
        ds.setVersion(QDataStream::Qt_4_3);
        ds << map;
    }
}
void settingswindow::ok() {
    QObject *o;
    foreach(QString s,objectnames) {
        o = this->findChild<QObject*> (s);
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
    if (net != 0)
        net->settingswindowemitfunktion();
}
void settingswindow::cancel() {
    this->close();
}
void settingswindow::soundoptionbuttonslot() {
    QPushButton *pb = qobject_cast<QPushButton*> (sender());
    Q_ASSERT(pb!=0);
    QString file = QFileDialog::getOpenFileName(this,
                                                "Choose a wav file for this event",
                                                QApplication::applicationDirPath() + "/wav");
    if (file == QString())
        return;
    if (pb->objectName() == "pbbuddychatmessage") {
        ui.lebuddychatmessage->setText(file);
    } else if (pb->objectName() == "pbbuddychatwindowopened") {
        ui.lebuddychatwindowsopened->setText(file);
    } else if (pb->objectName() == "pbnormalchatmessage") {
        ui.lenormalchatmessage->setText(file);
    } else if (pb->objectName() == "pbstartup") {
        ui.lestartup->setText(file);
    } else if (pb->objectName() == "pbbuddyarrives") {
        ui.lebuddyarrives->setText(file);
    } else if (pb->objectName() == "pbbuddyleaves") {
        ui.lebuddyleaves->setText(file);
    } else if (pb->objectName() == "pbhighlightning") {
        ui.lehighlightning->setText(file);
    } else if (pb->objectName() == "pbbuddyhosts")
        ui.lehostsound->setText(file);
     else if (pb->objectName() == "pbcostumword")
        ui.lecostumword->setText(file);
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
        ui.cbcostumword->setChecked(v.toBool());
    else if(s=="chbsmileys")
        ui.chbsmileys->setChecked(v.toBool());
    else if(s=="cbdontplaysound")
        ui.cbdontplaysound->setChecked(v.toBool());
    else if(s=="cbdontshowballoons")
        ui.cbdontshowballoons->setChecked(v.toBool());
    else if(s=="cbdontsortinchannels")
        ui.cbdontsortinchannels->setChecked(v.toBool());
    else
        throw std::runtime_error("\nvoid settingswindow::to_map(const QString &s, const QVariant &v)\n");
    safe();
}
