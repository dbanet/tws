#include <QTextCodec>

#include "codecselectdia.h"
#include "ui_codecselectdia.h"
#include "settings.h"

QTextCodec *CodecSelectDia::codec=0;
CodecSelectDia::CodecSelectDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodecSelectDia)
{
    ui->setupUi(this);
    foreach(QByteArray b, QTextCodec::availableCodecs()){
        ui->comboBox->addItem(b);
    }
    ui->cbrighToLeft->setChecked(S_S.getbool("righttoleftwriting"));
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(S_S.getString("textcodecsince263")));
}

CodecSelectDia::~CodecSelectDia()
{
    delete ui;
}

void CodecSelectDia::on_pbOk_clicked()
{
    codec=QTextCodec::codecForName(qPrintable(ui->comboBox->currentText()));
    S_S.set("textcodecsince263", ui->comboBox->currentText());
    bool b=ui->cbrighToLeft->isChecked();
    S_S.set("righttoleftwriting", b);
    if(b)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
    reject();
}
bool CodecSelectDia::contains(QString s){
    return QTextCodec::availableCodecs().contains (s.simplified ().toUtf8 ());
}
void CodecSelectDia::setcodec(QString s){
    if(!contains (s))
        return;
    codec=QTextCodec::codecForName(s.simplified ().toUtf8 ());
    S_S.set("textcodecsince263", s.simplified ());
}
