#include<QComboBox>
#include<QTextStream>
#include<QProcess>
#include<QFile>
#include<QApplication>
#include<QMessageBox>
#include"global_functions.h"
//----------------------------------------------------------------------------------------------
QStringList refreshcombobox(QComboBox *cb){
    QStringList sl;
    sl<<cb->currentText();
    for(int i=0 ; i<cb->count() ; i++){
        if(!sl.contains(cb->itemText(i)))
            sl<<cb->itemText(i);
    }
    cb->clear();
    cb->addItems(sl);
    return sl;
}
//----------------------------------------------------------------------------------------------
QString globalport;
//----------------------------------------------------------------------------------------------
void sethostport(const QString &port) {
    if(port.isEmpty())
        return;
    globalport=port;


#ifdef Q_WS_WIN
    QStringList env = QProcess::systemEnvironment();
    QString systemroot;
    foreach(QString s,env) {
        if (s.contains("SystemRoot=")) {
            systemroot = s;
            systemroot = systemroot.split("=").last();
        }
    }
    QFile winini(systemroot + "/win.ini");
    QStringList sl;
    bool b=0;
    if (winini.exists() && winini.open(QIODevice::ReadOnly)) {
        sl = QString(winini.readAll()).split("\n");
        int i = 0;
        foreach(QString s,sl) {
            if (s.contains("HostingPort=")) {
                b=1;
                sl[i] = "HostingPort=" + port;
            }
            i++;
        }
        if(b==0)
            sl.push_back("HostingPort=" + port);
    }
    winini.close();
    if(!winini.open(QFile::WriteOnly | QFile::Truncate))
        return;
    QTextStream ts(&winini);
    ts << sl.join("\n");
    winini.close();
#endif
}
QString gethostport() {
    QString port;
    port=globalport;
#ifdef Q_WS_WIN
    QStringList env=QProcess::systemEnvironment();
    QString systemroot;
    foreach(QString s,env) {
        if(s.contains("SystemRoot=")) {
            systemroot=s;
            systemroot=systemroot.split("=").last();
        }
    }
    QFile winini(systemroot+"/win.ini");
    if(winini.exists() && winini.open(QIODevice::ReadOnly)) {
        QStringList sl=QString(winini.readAll()).split("\n");
        foreach(QString s,sl) {
            if(s.contains("HostingPort=")) {
                if(s.split("=",QString::SkipEmptyParts).size()==2) {
                    port=s.split("=",QString::SkipEmptyParts).last();
                    port=port.simplified();
                }
            }
        }
        winini.close();
    } else
        myDebug()<<"%systemroot%/win.ini "+QObject::tr("is missing or locked, please run worms, one time, without the snooper. it will then create the file!");
#endif
    return port;
}
//----------------------------------------------------------------------------------------------
void safeusergarbage() {
    QFile f(QApplication::applicationDirPath() + "/query/log");
    if (!f.open(QFile::WriteOnly | QFile::Truncate))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    QMap<QString, QStringList>::iterator i;
    for (i = usergarbagemap.begin(); i != usergarbagemap.end(); ++i) {
        ds << i.key() << i.value();
    }
}
//----------------------------------------------------------------------------------------------
void loadusergarbage() {
    QFile f(QApplication::applicationDirPath() + "/query/log");
    QString s;
    QStringList sl;
    if (!f.open(QFile::ReadOnly))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    while (!ds.atEnd()) {
        ds >> s >> sl;
        usergarbagemap[s] = sl;
    }
}
//----------------------------------------------------------------------------------------------
void safequerylist() {
    QFile f(QApplication::applicationDirPath() + "/query/querylist");
    if (!f.open(QFile::WriteOnly | QFile::Truncate))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds << querylist;
}
//----------------------------------------------------------------------------------------------
void loadquerylist() {
    QFile f(QApplication::applicationDirPath() + "/query/querylist");
    if (!f.open(QFile::ReadOnly))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds >> querylist;
}
//----------------------------------------------------------------------------------------------
void info(const QString &msg){
    QMessageBox::information(0,"Debug info",msg);
}
//----------------------------------------------------------------------------------------------
void info(int i){
    QMessageBox::information(0,"Debug info",QString::number(i));
}
