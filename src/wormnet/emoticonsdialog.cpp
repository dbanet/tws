#include"emoticonsdialog.h"
#include"ui_emoticonsdialog.h"
#include"chathandler.h"
#include"emoticonhandler.h"
#include"math.h"

#include<QDebug>
#include<QListWidgetItem>

emoticonsdialog::emoticonsdialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emoticonsdialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint);
    resize(100,height());
    QMap<QString,QImage> map = chathandler::emot->map;
    foreach(QString s, map.keys()){
        QListWidgetItem *item = new QListWidgetItem(QPixmap::fromImage(map[s]),s);
        ui->listWidget->addItem(item);
    }
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)), this,SLOT(smileyclicked(QListWidgetItem*)));
}
void emoticonsdialog::smileyclicked(QListWidgetItem *w){
    emit sigemotchoosed(w->text());
    close();
}
emoticonsdialog::~emoticonsdialog(){
    delete ui;
}
void emoticonsdialog::focusOutEvent( QFocusEvent * event ){
    if(ui->listWidget->hasFocus())
        return;
    close();
}
