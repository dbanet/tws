#include "ircjoindia.h"
#include "ui_ircjoindia.h"

ircJoinDia::ircJoinDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ircJoinDia)
{
    ui->setupUi(this);
}

ircJoinDia::~ircJoinDia()
{
    delete ui;
}
QString ircJoinDia::getChannel(){
    return ui->leChannel->text();
}
