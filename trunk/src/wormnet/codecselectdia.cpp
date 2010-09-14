#include "codecselectdia.h"
#include "ui_codecselectdia.h"
#include<QTextCodec>
#include"snpsettings.h"
CodecSelectDia::CodecSelectDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodecSelectDia)
{
    ui->setupUi(this);
    foreach(QByteArray b, QTextCodec::availableCodecs()){
        ui->comboBox->addItem(b);
    }
    ui->cbrighToLeft->setChecked(singleton<snpsettings>().map["righttoleftwriting"].toBool());
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(singleton<snpsettings>().map["textcodec"].toString()));
}

CodecSelectDia::~CodecSelectDia()
{
    delete ui;
}

void CodecSelectDia::on_pblatin_clicked()
{
    QTextCodec *c=QTextCodec::codecForName("windows-1252");     //latin
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]="windows-1252";
    qApp->setLayoutDirection(Qt::LeftToRight);
    singleton<snpsettings>().map["righttoleftwriting"]=false;
    accept();
}

void CodecSelectDia::on_pbcyrillic_clicked()
{
    QTextCodec *c=QTextCodec::codecForName("windows-1251");     //cyrillic
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]="windows-1251";
    singleton<snpsettings>().map["righttoleftwriting"]=false;
    qApp->setLayoutDirection(Qt::LeftToRight);
    accept();
}

void CodecSelectDia::on_pbOk_clicked()
{
    QTextCodec *c=QTextCodec::codecForName(qPrintable(ui->comboBox->currentText()));
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]=ui->comboBox->currentText();
    bool b=ui->cbrighToLeft->isChecked();
    singleton<snpsettings>().map["righttoleftwriting"]=b;
    if(b)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
    reject();
}
