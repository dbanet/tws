#include "awaybox.h"
#include "settings.h"
#include "netcoupler.h"
#include "qobjectwrapper.h"
#include "awayhandler.h"
#include "global_functions.h"
#include <QPointer>
#include <QKeyEvent>
#include <QEvent>
bool awaybox::ison=0;
awaybox::awaybox(QWidget *parent):QWidget(parent) {
    setObjectName("awaybox");
    ui.setupUi(this);
    setWindowTitle(tr("Type in your awaymessage."));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon("snppictures/tray.png"));
    sl = S_S.getstringlist("awaymessage");
    counter=sl.size();
    i=counter-1;
    if(!S_S.getstringlist("awaymessage").isEmpty()){
        ui.textEdit->setText(sl[i]);
    }
    connect(ui.ok, SIGNAL(clicked()),this, SLOT(okclicked()));
    connect(ui.back, SIGNAL(clicked()),this, SLOT(backclicked()));
    connect(ui.forward, SIGNAL(clicked()),this, SLOT(forwardclicked()));
    setWindowFlags(windowFlags() ^= Qt::WindowSystemMenuHint);
    ison=1;
    ui.textEdit->installEventFilter(this);
    //connect(ui.cancel,SIGNAL(clicked()),this,SLOT(cancelclicked()));
}
void awaybox::okclicked() {
    qobjectwrapper<awayhandler>::ref().setaway(ui.textEdit->toPlainText());
    if(!sl.contains(ui.textEdit->toPlainText()))
        sl<<ui.textEdit->toPlainText();
    else
        sl.move(sl.indexOf(ui.textEdit->toPlainText()),sl.size()-1);
    S_S.set("awaymessage", sl);    
    emit sigok();    
    close();    
}
void awaybox::cancelclicked() {
    close();
}
void awaybox::backclicked() {
    if (i > 0)
        ui.textEdit->setText(sl[--i]);
}
void awaybox::forwardclicked() {
    if (i < counter-1)
        ui.textEdit->setText(sl[++i]);
}
void awaybox::closeEvent(QCloseEvent *) {
    ison=0;
    qobjectwrapper<awayhandler>::ref().setaway(ui.textEdit->toPlainText());
}
awaybox::~awaybox() {
}
bool awaybox::eventFilter(QObject * o, QEvent *e){
    if(o != ui.textEdit)
        return false;
    if(e->type()!=QEvent::KeyPress)
        return false;
    QKeyEvent *ke=static_cast<QKeyEvent*>(e);
    switch(ke->key()){
    case Qt::Key_Return:
        ui.ok->click();
        break;
    case Qt::Key_PageUp:
        ui.forward->click();
        break;
    case Qt::Key_PageDown:
        ui.back->click();
        break;
    default:
        return false;
    }
    return true;
}
