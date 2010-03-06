#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H
#include<QStringList>
#include<QString>
#include<QMap>
#include<QFile>
#include<QProcess>
#include<QTextStream>
extern QMap<QString, QStringList> usergarbagemap;
extern QStringList querylist;

// #include"global_functions.h"
namespace {
    bool compareCI(const QString &s1, const QString &s2) {
        return s1.toLower() == s2.toLower();
    }
    //----------------------------------------------------------------------------------------------
    bool startswithCI(const QString &s1, const QString &s2) {
        return s1.startsWith(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsCI(const QString &s1, const QString &s2) {
        return s1.contains(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsCI(const QStringList &sl, const QString &s2) {
        return sl.contains(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsOneCI(const QString &s, const QStringList &sl, QString *arg) {
        foreach(QString str,sl){
            if(s.contains(str, Qt::CaseInsensitive)){
                if(arg!=NULL)
                    *arg=str;
                return true;
            }
        }
        return false;
    }
    //----------------------------------------------------------------------------------------------
    QStringList removeCI(QStringList &sl, const QString &s) {
        foreach(QString str,sl) {
            if (compareCI(s, str))
                sl.removeAt(sl.indexOf(str));
        }
        return sl;
    }
    //----------------------------------------------------------------------------------------------
    void safeusergarbage() {
        QFile f(QApplication::applicationDirPath() + "/query/log");
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            QMap<QString, QStringList>::iterator i;
            for (i = usergarbagemap.begin(); i != usergarbagemap.end(); ++i) {
                ds << i.key() << i.value();
            }
        }
    }
    //----------------------------------------------------------------------------------------------
    void loadusergarbage() {
        QFile f(QApplication::applicationDirPath() + "/query/log");
        QString s;
        QStringList sl;
        if (f.open(QFile::ReadOnly)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            while (!ds.atEnd()) {
                ds >> s >> sl;
                usergarbagemap[s] = sl;
            }
        }
    }
    //----------------------------------------------------------------------------------------------
    void safequerylist() {
        QFile f(QApplication::applicationDirPath() + "/query/querylist");
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            ds << querylist;
        }
    }
    //----------------------------------------------------------------------------------------------
    void loadquerylist() {
        QFile f(QApplication::applicationDirPath() + "/query/querylist");
        if (f.open(QFile::ReadOnly)) {
            QDataStream ds(&f);
            ds.setVersion(QDataStream::Qt_4_3);
            ds >> querylist;
        }
    }
    //----------------------------------------------------------------------------------------------
    QString globalport;

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
            qDebug()<<"%systemroot%/win.ini "+QObject::tr("is missing or locked, please run worms, one time, without the snooper. it will then create the file!");
#endif
        return port;
    }
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
            ;
        QTextStream ts(&winini);
        ts << sl.join("\n");
        winini.close();
#endif
    }
};
//----------------------------------------------------------------------------------------------
#endif // GLOBAL_FUNCTIONS_H
