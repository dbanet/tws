#include "ctctphandlerwidget.h"
#include"ctcphandleratomic.h"
#include<QFile>
#include<QDataStream>
ctctphandlerwidget::ctctphandlerwidget(){
    this->setObjectName("ctcphandler");
    ui.setupUi(this);
    this->setWindowTitle(tr("ctcp handler."));
    QFile file("snpini/ctcp.ini");

    QString line;
    QString command;
    bool enabled=0;
    if (file.open(QFile::ReadOnly)) {
        QDataStream ds(&file);
        ds.setVersion(QDataStream::Qt_4_3);
        while(!ds.atEnd()){
            ds>>command;
            if(!ds.atEnd()){
                ds>>line;
                if(!ds.atEnd()){
                    ds>>enabled;
                    addcommand(command);
                    addsalary(command,line,enabled);                    
                }
            }
        }
    }
    foreach(QPointer<ctcphandleratomic> a,atomiclist) {
        atomicmap[a->ui.lineEdit->text().simplified()] = a;
    }
    connect(ui.safeclose, SIGNAL(clicked()),this, SLOT(safeandclose()));
    connect(ui.close_2, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui.add, SIGNAL(clicked()),this, SLOT(addcommandbyhand()));
    connect(ui.pbhowto,SIGNAL(clicked()),this,SLOT(pbhowtoclicked()));
}
void ctctphandlerwidget::addsalary(QString command, QString line,bool enabled) {
    foreach(QPointer<ctcphandleratomic> a,atomiclist) {
        if (a->ui.lineEdit->text() == command && line.simplified()!="")
        {
            a->ui.textEdit->append(line+"\n");
            a->ui.cbenable->setChecked(enabled);
        }
    }
}
void ctctphandlerwidget::addcommand(QString command) {
    QPointer<ctcphandleratomic> a = new ctcphandleratomic();
    ui.atomiclayout->addWidget(a);
    a->show();
    a->ui.lineEdit->setText(command);
    atomiclist << a;
}
void ctctphandlerwidget::addcommandbyhand() {
    QPointer<ctcphandleratomic> a = new ctcphandleratomic();
    ui.atomiclayout->addWidget(a);
    a->show();
    atomiclist << a;
}
void ctctphandlerwidget::safeandclose() {
    atomicmap.clear();
    QFile file("snpini/ctcp.ini");
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QDataStream ds(&file);
        ds.setVersion(QDataStream::Qt_4_3);
        foreach(QPointer<ctcphandleratomic> a,atomiclist) {
            if (a != 0 && a->ui.lineEdit->text()!="") {
                ds<<a->ui.lineEdit->text();
                ds<<a->ui.textEdit->toPlainText().simplified();
                ds<<a->ui.cbenable->isChecked();
                atomicmap[a->ui.lineEdit->text().remove("\n")] = a;
            }
        }
        atomiclist=atomicmap.values();
    }
    this->close();
}
void ctctphandlerwidget::pbhowtoclicked(){
    QTextEdit *te=new QTextEdit;
    te->setObjectName("ctcphowto");
    te->setAttribute(Qt::WA_DeleteOnClose);
    QString s=tr("CTCP commands are simple messages, that are bounded with ASCII 0x01 signs.\n"
                 "These signs cannot be written directly into a textline.\n"
                 "If you want to send a ctcp command you must prefix the message with >!\n"
                 "for example '>!VERSION' or '>!command'.\n"
                 "\n"
                 "CTCP means Client to Client Protocoll.\n"
                 "The commands representing messages that another can send you.\n"
                 "He will then receive the the text under the command.\n"
                 "It is a good idea to provide a deafult command,\n"
                 "this command should send a message that explains which commands\n"
                 "are available for the public.\n"
                 "For example the user writes '>!default' to you in a private message\n"
                 "he will then see your other commands and can simply browse your\n"
                 "CTCP commands.\n"
                 "\n"
                 "use CTCP commands for clansites, jokes, pictures, qfileupload links etc.");
    te->setText(s);
    te->resize(600,250);
    te->show();
}
ctctphandlerwidget::~ctctphandlerwidget() {

}
