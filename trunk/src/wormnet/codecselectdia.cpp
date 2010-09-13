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
}

CodecSelectDia::~CodecSelectDia()
{
    delete ui;
}

void CodecSelectDia::on_pblatin_clicked()
{
    QTextCodec *c=QTextCodec::codecForName("Windows-1252");     //latin
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]="Windows-1252";
    accept();
}

void CodecSelectDia::on_pbcyrillic_clicked()
{
    QTextCodec *c=QTextCodec::codecForName("Windows-1251");     //cyrillic
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]="Windows-1251";
    accept();
}

void CodecSelectDia::on_pbOk_clicked()
{
    QTextCodec *c=QTextCodec::codecForName(qPrintable(ui->comboBox->currentText()));
    QTextCodec::setCodecForCStrings(c);
    singleton<snpsettings>().map["textcodec"]=ui->comboBox->currentText();
    reject();
}
