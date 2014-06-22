#include "progressdialog.h"
#include "ui_progressdialog.h"

progressdialog::progressdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressdialog){
    ui->setupUi(this);
    setModal(false);
    setAttribute(Qt::WA_DeleteOnClose);

}

progressdialog::~progressdialog(){
    delete ui;
}
