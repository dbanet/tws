#include "combobox_wrapper.h"
#include "ui_combobox_wrapper.h"

combobox_wrapper::combobox_wrapper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::combobox_wrapper)
{
    ui->setupUi(this);
}

combobox_wrapper::~combobox_wrapper()
{
    delete ui;
}

void combobox_wrapper::on_plus_2_clicked()
{
    ui->comboBox->addItem("");
    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
}

void combobox_wrapper::on_minus_clicked()
{
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}
void combobox_wrapper::set(QStringList sl){
    ui->comboBox->clear();
    ui->comboBox->addItems(sl);
}
QStringList combobox_wrapper::get(){
    QStringList sl;
    for(int i=0;i<ui->comboBox->count();i++){
        QString s=ui->comboBox->itemText(i);
        if(s!=QString())
            sl<<ui->comboBox->itemText(i);
    }
    return sl;
}

void combobox_wrapper::on_up_clicked()
{
    if(ui->comboBox->currentIndex()>0)
        ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex()-1);

}

void combobox_wrapper::on_down_clicked()
{
    if(ui->comboBox->currentIndex()<ui->comboBox->count()-1)
        ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex()+1);

}
