#include "buttonlayout.h"
#include"netcoupler.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include<QDebug>
#include<QPointer>
extern QPointer<netcoupler> net;;
buttonlayout::buttonlayout(QWidget *parent) :
	QWidget(parent) {
    ui.setupUi(this);
    ui.horizontalLayout->setAlignment(Qt::AlignLeft);
    this->setObjectName("buttoenlayout");
    connect(ui.pbrefresh, SIGNAL(clicked()),net, SLOT(refreshhostlist()));
    connect(ui.pbhost, SIGNAL(clicked()),this, SIGNAL(pbhostclicked()));
    connect(ui.pbminimize, SIGNAL(clicked()),this, SIGNAL(pbminimizedclicked()));
    connect(ui.slideralpha,SIGNAL(valueChanged ( int )),this,SIGNAL(sigchangealpha(int)));
    if(singleton<snpsettings>().map["channeltransparency"].toInt()>=20){
        ui.slideralpha->setValue(singleton<snpsettings>().map["channeltransparency"].toInt());
    }
    else
        ui.slideralpha->setValue(100);
    ui.chatwindowbuttonscrollArea->installEventFilter(this);
    this->setMaximumHeight(23);   
    if(singleton<settingswindow>().from_map("cbcostumword").toBool())
        ui.pbcostumwords->setText(tr("Costum words on"));
    else
        ui.pbcostumwords->setText(tr("Costum words off"));

    if(singleton<settingswindow>().from_map("cbdontplaysound").toBool())
        ui.pbsound->setText(tr("Sound off"));
    else
        ui.pbsound->setText(tr("Sound on"));

    if(singleton<settingswindow>().from_map("cbdontshowballoons").toBool())
        ui.pbballoon->setText(tr("Balloons off"));
    else
        ui.pbballoon->setText(tr("Balloons on"));

    if(singleton<settingswindow>().from_map("cbdontsortinchannels").toBool())
        ui.pbsort->setText(tr("Sorting off"));
    else
        ui.pbsort->setText(tr("Sorting on"));
}
bool buttonlayout::eventFilter(QObject *obj, QEvent *event) {
    if (qobject_cast<QScrollArea*> (obj) != 0 && qobject_cast<QScrollArea*> (
            obj)->objectName() == "chatwindowbuttonscrollArea") {
        if (event->type() == QEvent::Enter) {            
            if(ui.chatwindowbuttonscrollArea->width()<941){
                ui.chatwindowbuttonscrollArea->setMaximumHeight(50);
                this->setMaximumHeight(50);
                ui.chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            }
        } else if (event->type() == QEvent::Leave) {
            ui.chatwindowbuttonscrollArea->setMaximumHeight(23);
            this->setMaximumHeight(23);
            ui.chatwindowbuttonscrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    return QObject::eventFilter(obj, event);
}
void buttonlayout::hidebuttons() {
    this->show();
    emit sighideme();
}
void buttonlayout::showbuttons() {
    this->hide();
    emit sigshowme();
}
buttonlayout::~buttonlayout() {

}
void buttonlayout::on_pbcostumwords_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbcostumword").value<bool> ();
    singleton<settingswindow>().to_map("cbcostumword",!b);
    if(!b)
        ui.pbcostumwords->setText(tr("Costum words on"));
    else
        ui.pbcostumwords->setText(tr("Costum words off"));
}

void buttonlayout::on_pbsound_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontplaysound").toBool();
    singleton<settingswindow>().to_map("cbdontplaysound",!b);
    if(!b)
        ui.pbsound->setText(tr("Sound off"));
    else
        ui.pbsound->setText(tr("Sound on"));

}

void buttonlayout::on_pbballoon_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontshowballoons").toBool();
    singleton<settingswindow>().to_map("cbdontshowballoons",!b);
    if(!b)
        ui.pbballoon->setText(tr("Balloons off"));
    else
        ui.pbballoon->setText(tr("Balloons on"));

}

void buttonlayout::on_pbsort_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontsortinchannels").toBool();
    singleton<settingswindow>().to_map("cbdontsortinchannels",!b);
    if(!b)
        ui.pbsort->setText(tr("Sorting off"));
    else
        ui.pbsort->setText(tr("Sorting on"));
}
