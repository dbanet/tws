#include "netcoupler.h"
#include "window.h"
#include "mainwindow.h"
#include "snpsettings.h"
#include "charformatsettings.h"
#include <QtGui>
#include <QApplication>
#include<QPlastiqueStyle>
#include<QTextStream>
#include<QTextCodec>
#include"settingswindow.h"
#include"volumeslider.h"
#include"playername.h"
#include "sound_handler.h"
#include"global_functions.h"
#include"codecselectdia.h"
#include"clantowebpagemapper.h"
#include"myDebug.h"

#ifdef Q_WS_WIN
    #include <dir.h>
#endif
volumeslider *volume;

QList<QPixmap*> flaglist;
QList<QPixmap*> ranklist;
int ranklistsize;
int flaglistsize;
QPixmap *locked;
QPixmap *unlocked;
QStringList querylist;
QMap<QString, QStringList> usergarbagemap;
static mainwindow *w = 0;
QStringList defaultServerList(QStringList()<<"wormnet1.team17.com"<<"itakagames.spb.ru"<<"212.240.191.125"<<"worms.tom.ru");

void search_for_game_executables();
void handle_prosnooper_buddys();
void handle_wini_ini();
void myMessageOutput(QtMsgType, const char *);
int main(int argc, char *argv[]) {    
    qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);    
    a.setApplicationName("The Wheat Snooper");
    chdir(qPrintable(QApplication::applicationDirPath()));
    singleton<snpsettings>().load();
    singleton<clantowebpagemapper>().load();
    if(singleton<snpsettings>().map["textcodec"].toString().isEmpty()){
        CodecSelectDia::codec=QTextCodec::codecForLocale();
        singleton<snpsettings>().map["textcodec"]=CodecSelectDia::codec->name();
    }
    else
        CodecSelectDia::codec=QTextCodec::codecForName(singleton<snpsettings>().map["textcodec"].toByteArray());    
    singleton<charformatsettings>().load();
    volume = new volumeslider;
    a.addLibraryPath(QApplication::applicationDirPath());
    a.setWindowIcon(QIcon(QApplication::applicationDirPath()
                          + QDir::separator() + "snppictures" + QDir::separator()
                          + "tray.png"));
    QString sep = QDir::separator();
    QDir dir("snppictures/flags");
    if (!dir.exists())
        myDebug() << QObject::tr("the flags in snppictures/flags are missing!");
    foreach(QString f,dir.entryList(QStringList()<<"*.png",QDir::Files,QDir::Name)) {
        flaglist.push_back(new QPixmap(dir.path() + sep + f));
    }
    dir.setPath(QApplication::applicationDirPath() + sep + "snppictures" + sep
                + "ranks");
    if (!dir.exists())
        myDebug() << QObject::tr("the ranks in snppictures/flags are missing!");
    foreach(QString f,dir.entryList(QStringList()<<"*.png",QDir::Files,QDir::Name)) {
        ranklist.push_back(new QPixmap(dir.path() + sep + f));
    }
    locked = new QPixmap;
    unlocked = new QPixmap;
    if (!locked->load(QApplication::applicationDirPath() + sep + "snppictures"
                      + sep + "locked.png"))
        myDebug() << QObject::tr("some pictures are missing!");
    if (!unlocked->load(QApplication::applicationDirPath() + sep
			+ "snppictures" + sep + "unlocked.png"))
        myDebug() << QObject::tr("some pictures are missing!");

    ranklistsize = ranklist.size();
    flaglistsize = flaglist.size();
    a.setStyle(new QPlastiqueStyle);
    a.setQuitOnLastWindowClosed(0);    

#ifdef Q_WS_WIN
    search_for_game_executables();
    handle_prosnooper_buddys();
    handle_wini_ini();
#endif
    singleton<snpsettings>();
    singleton<settingswindow>();
    singleton<sound_handler>().init();
    w = new mainwindow;
    myDebugClass::w=w;
    volume->setvalue(singleton<snpsettings>().map["volumeslidervalue"].value<int> ());
    if (!singleton<snpsettings>().map["chbminimized"].value<bool> ()){
        w->show();        
    }

    loadusergarbage();
    loadquerylist();       
    QTimer::singleShot(1000,&singleton<sound_handler>(),SLOT(play_startupsound()));
    return a.exec();
}
void myMessageOutput(QtMsgType type, const char *msg) {
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        if (w != 0
            && singleton<settingswindow>().from_map("cbservermessageinchannelwindows").value<bool> ())
            w->appenddebugmessage(QString(msg).simplified()+"\n");
        else
            mainwindow::debugmsg.append(QString(msg).simplified()+"\n");
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}
void handle_wini_ini(){
    //looking in win.ini for the playername, to avoid mistakes
    QStringList env = QProcess::systemEnvironment();
    QString systemroot;
    foreach(QString s,env) {
        if (s.contains("SystemRoot=")) {
            systemroot = s;
            systemroot = systemroot.split("=").last();
        }
    }
    QFile winini(systemroot + "/win.ini");
    QString name;
    QStringList sl;
    sl.clear();
    if (winini.open(QIODevice::ReadOnly)) {
        sl = QString(winini.readAll()).split("\n",
                                             QString::SkipEmptyParts);
        winini.close();
        int i = 0;
        bool b = 0;
        foreach(QString s,sl) {
            if(containsCI(s,"Playername")) {
                b=1;
                if (s.split("=", QString::SkipEmptyParts).size() != 2) {
                    s = "PlayerName=temporaryname\n";
                    playername::nick="temporaryname";
                    sl[i]=s;
                } else if (!s.contains("PlayerName=")) {
                    s = "PlayerName=temporaryname\n";
                    playername::nick="temporaryname";
                    sl[i]=s;
                } else if(s.split("=", QString::SkipEmptyParts).size() == 2) {
                    playername::nick=s.split("=", QString::SkipEmptyParts).last();
                }
            }
            i++;
        }
        if(b==0) {
            sl.insert(2,QString("PlayerName=temporaryname\n"));
        }
    }
    if (sl.isEmpty()) {
        sl << "[NetSettings]" << "PlayerName=temporaryname" << "UseProxy=0"
                << "ProxyPort=0" << "HostingPort=17011" << "UpdatePrompt=0"
                << "UpdateServerList=1" << "AutoPing=1" << "Location=0"
                << "FilterLanguage=1" << "AddressOverride=0" << "UseUPnP=1"
                << "Protocol=1" << "ServerLaunched=0";
    }
    if (winini.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream ts(&winini);
        foreach(QString s,sl) {
            s = s.simplified();
            ts << s + "\n";
        }
    }
    winini.close();
}
void search_for_game_executables(){
    if (!singleton<snpsettings>().map.contains("joinstrings")
        || singleton<snpsettings>().map["joinstrings"].toStringList().isEmpty()) {
        QSettings settings("Team17SoftwareLTD", "WormsArmageddon");
        QString gamedir(settings.value("PATH").toString());
        QFile wa;
        QFile wormkit;
        wa.setFileName(gamedir + "/wa.exe");
        wormkit.setFileName(gamedir + "/WormKit.exe");
        QString text;
        bool test = 0;
        if (wa.exists()) {
            text = wa.fileName();
            test = 1;
        }
        if (wormkit.exists()) {
            text = text + "\n" + wormkit.fileName();
        }
        if (test)
            singleton<snpsettings>().map["joinstrings"] = text.split("\n",QString::SkipEmptyParts);
    }
    singleton<snpsettings>().safe();
}

void handle_prosnooper_buddys(){
    QSettings settings("HKEY_CURRENT_USER\\Software\\ProSnooper",
                       QSettings::NativeFormat);
    QString buddies = settings.value("Buddies").toString();
    QStringList snpbuddies = singleton<snpsettings>().map["buddylist"].value<QStringList> ();
    QStringList buddielist;
    if (!buddies.isEmpty())
        buddielist = buddies.split(",", QString::SkipEmptyParts);
    singleton<snpsettings>().map["prosnooperbuddies"];
    QStringList sl = singleton<snpsettings>().map["prosnooperbuddies"].value<QStringList> ();
    foreach(QString s,buddielist) {
        if (!sl.contains(s, Qt::CaseInsensitive)) {
            sl << s;
            if (!snpbuddies.contains(s, Qt::CaseInsensitive))
                snpbuddies << s;
        }
    }
    singleton<snpsettings>().map["prosnooperbuddies"] = sl;
    singleton<snpsettings>().map["buddylist"] = snpbuddies;
}
