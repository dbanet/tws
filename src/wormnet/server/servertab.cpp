#include "servertab.h"
#include "ui_servertab.h"
#include "../netcoupler.h"
#include "../global_macros.h"
#include "../sqlsettings.h"

serverTab::serverTab(MainWindow *mainWnd,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serverTab),
    mainWnd(mainWnd)
{
    ui->setupUi(this);
    connect(&singleton<netcoupler>(), SIGNAL(sigGotChanList(const QStringList &)),this, SLOT(gotChannelList(const QStringList &)));
}

serverTab::~serverTab()
{
    delete ui;
}

void serverTab::on_pbjoin_clicked()
{
        mainWnd->join(ui->cbchannels->currentText());
}

void serverTab::on_pbrememberjoin_clicked()
{
    S_S.set("joinonstartup", ui->cbchannels->currentText());
    ui->pbrememberjoin->setText(tr("Autojoin:") + "\n" + S_S.getString("joinonstartup"));
}

void serverTab::on_chbautojoin_clicked(bool checked)
{
    if(checked)
        on_pbrememberjoin_clicked();

}

void serverTab::gotChannelList(const QStringList &sl) {
    ui->cbchannels->clear();
    foreach(QString s,sl)
        ui->cbchannels->addItem(s);
    channelList = sl;
    ui->pbjoin->setEnabled(1);
    //joinmenu->clear();
    //foreach(QString s,channelList) {
    //    joinmenu->addAction(s);
    //}
#ifdef WITH_GAMESURGE_SUPPORT
    joinmenu->addAction(GamesourgeChannelName);
#endif
    if (mainWnd->joinonstartup == 0) {
        mainWnd->snpsetcontains("joinonstartup");
        mainWnd->joinonstartup = 1;
    }
}

void serverTab::fillSnpSettings(){
    S_S.set("chbminimized", ui->chbminimized->isChecked());
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
}

void serverTab::returnToLoginTab(){
    S_S.set("chbautojoin", ui->chbautojoin->isChecked());
    ui->cbchannels->clear();
    ui->pbjoin->setEnabled(0);
}

void serverTab::addToServInfo(QString debugmsg){
    ui->labeltraydescription->insertPlainText(debugmsg);
}

void serverTab::set_chbautojoin(bool state){
    ui->chbautojoin->setChecked(state);
}

void serverTab::set_chbminimized(bool state){
    ui->chbminimized->setChecked(state);
}
