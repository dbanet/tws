#include "buttonlayout.h"
#include"netcoupler.h"
#include"snpsettings.h"
#include"settingswindow.h"
#include"leagueserverhandler.h"
#include<QMenu>
#include<QPointer>
buttonlayout::buttonlayout(QWidget *parent) :
	QWidget(parent) {
    ui.setupUi(this);    
    leaguemenu=new QMenu;
    ui.horizontalLayout->setAlignment(Qt::AlignLeft);
    this->setObjectName("buttoenlayout");
    connect(ui.pbrefresh, SIGNAL(clicked()),&singleton<netcoupler>(), SLOT(refreshhostlist()));
    connect(ui.pbhost, SIGNAL(clicked()),this, SIGNAL(pbhostclicked()));
    connect(ui.pbminimize, SIGNAL(clicked()),this, SIGNAL(pbminimizedclicked()));
    connect(ui.slideralpha,SIGNAL(valueChanged ( int )),this,SIGNAL(sigchangealpha(int)));
    connect(leaguemenu,SIGNAL(triggered(QAction*)),this,SLOT(leaguemenutriggered(QAction*)));
    if(singleton<snpsettings>().map["channeltransparency"].toInt()>=20){
        ui.slideralpha->setValue(singleton<snpsettings>().map["channeltransparency"].toInt());
    }
    else
        ui.slideralpha->setValue(100);
    ui.chatwindowbuttonscrollArea->installEventFilter(this);
    this->setMaximumHeight(23);   
    if(singleton<settingswindow>().from_map("cbcostumword").toBool())
        ui.pbcostumwords->setText(QObject::tr("Costum words")+" "+QObject::tr("on"));
    else
        ui.pbcostumwords->setText(QObject::tr("Costum words") +" "+QObject::tr("off"));

    if(singleton<settingswindow>().from_map("cbdontplaysound").toBool())
        ui.pbsound->setText(QObject::tr("Sound")+" "+QObject::tr("off"));
    else
        ui.pbsound->setText(QObject::tr("Sound")+" "+QObject::tr("on"));

    if(singleton<settingswindow>().from_map("cbdontshowballoons").toBool())
        ui.pbballoon->setText(QObject::tr("Balloons")+" "+QObject::tr("off"));
    else
        ui.pbballoon->setText(QObject::tr("Balloons")+" "+QObject::tr("on"));

    if(singleton<settingswindow>().from_map("cbdontsortinchannels").toBool())
        ui.pbsort->setText(QObject::tr("Sorting")+" "+QObject::tr("off"));
    else
        ui.pbsort->setText(QObject::tr("Sorting")+" "+QObject::tr("on"));
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
        ui.pbcostumwords->setText(QObject::tr("Costum words")+" "+QObject::tr("on"));
    else
        ui.pbcostumwords->setText(QObject::tr("Costum words")+" "+QObject::tr("off"));
}
void buttonlayout::on_pbsound_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontplaysound").toBool();
    singleton<settingswindow>().to_map("cbdontplaysound",!b);
    if(!b)
        ui.pbsound->setText(QObject::tr("Sound")+" "+QObject::tr("off"));
    else
        ui.pbsound->setText(QObject::tr("Sound")+" "+QObject::tr("on"));

}
void buttonlayout::on_pbballoon_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontshowballoons").toBool();
    singleton<settingswindow>().to_map("cbdontshowballoons",!b);
    if(!b)
        ui.pbballoon->setText(QObject::tr("Balloons")+" "+QObject::tr("off"));
    else
        ui.pbballoon->setText(QObject::tr("Balloons")+" "+QObject::tr("on"));
}
void buttonlayout::on_pbsort_clicked()
{
    bool b=singleton<settingswindow>().from_map("cbdontsortinchannels").toBool();
    singleton<settingswindow>().to_map("cbdontsortinchannels",!b);
    if(!b)
        ui.pbsort->setText(QObject::tr("Sorting")+" "+QObject::tr("off"));
    else
        ui.pbsort->setText(QObject::tr("Sorting")+" "+QObject::tr("on"));
}
void buttonlayout::leaguemenutriggered(QAction *action){
    if(action==NULL)
        return;
    if(action->text()==tr("Off")){
        singleton<snpsettings>().map["spectatingneversettedoff"]=false;
        singleton<leagueserverhandler>().stoprefresh();
        singleton<snpsettings>().map["spectateleagueserver"]=false;
        return;
    }
    QString s=action->text();
    singleton<snpsettings>().map["spectateleagueserver"]=true;
    singleton<leagueserverhandler>().setleague(s,s);
    singleton<leagueserverhandler>().startrefresh();
}
void buttonlayout::fillleaguemenu(){
    leaguemenu->addAction(tr("Off"));
    foreach(QString s,singleton<snpsettings>().map["leagueservers"].value<QStringList>()){
        leaguemenu->addAction(s);
    }
    ui.pbspectate->setMenu(leaguemenu);
}
