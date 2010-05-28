#include "automatic_posts_dialog.h"
#include "ui_automatic_posts_dialog.h"
#include "charformatsettings.h"
#include "snpsettings.h"
automatic_posts_dialog::automatic_posts_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::automatic_posts_dialog)
{
    ui->setupUi(this);
    ui->messageedit->setPlainText(singleton<snpsettings>().map["automatic_posts"].toString());
    ui->timebox->setValue(singleton<snpsettings>().map["automatic_posts_delay"].toInt());
}

automatic_posts_dialog::~automatic_posts_dialog()
{
    delete ui;
}

void automatic_posts_dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void automatic_posts_dialog::on_pbstart_clicked()
{
    singleton<snpsettings>().map["automatic_posts"]=ui->messageedit->toPlainText();
    singleton<snpsettings>().map["automatic_posts_delay"]=ui->timebox->value();
    accept();
}

void automatic_posts_dialog::on_pbstop_clicked()
{
    reject();
}
