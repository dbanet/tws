#include"window.h"
#include"mainwindow.h"
#include"settings.h"
#include"charformatsettings.h"
#include"settingswindow.h"
#include"volumeslider.h"
#include"sound_handler.h"
#include"global_functions.h"
#include"codecselectdia.h"
#include"clantowebpagemapper.h"
#include"picturehandler.h"
#include"awayhandler.h"
#include"about.h"
#include"wa_textcodec.h"
#include"playername.h"

#include<QtGui>
#include<QApplication>
#include<QPlastiqueStyle>
#include<QTextStream>
#include<QTextCodec>
#include<QDir>

#ifdef Q_WS_WIN
#include <dir.h>
#endif
volumeslider *volume;
QStringList querylist;

void search_for_game_executables();
void handle_prosnooper_buddys();
void get_picutres();
int main(int argc, char *argv[]) {
    new WA_textcodec1252;
    new WA_textcodec1251;
    QApplication a(argc, argv);
    a.setApplicationVersion(about::version);
    a.setApplicationName("The Wheat Snooper");

    chdir(qPrintable(QApplication::applicationDirPath()));
    S_S.start("snpini/settings.sqlite3");
    singleton<settingswindow>();
    a.setQuitOnLastWindowClosed(false);
    volume = new volumeslider;
    a.setApplicationName("The Wheat Snooper");        
    singleton<clantowebpagemapper>().refresh();
    singleton<picturehandler>();
    if(S_S.getstring("textcodecsince263").isEmpty()){
        CodecSelectDia::codec=QTextCodec::codecForName("wa");
        S_S.set("textcodecsince263", "wa");
    }
    else
        CodecSelectDia::codec=QTextCodec::codecForName(S_S.getbytearray("textcodecsince263"));
    if(!CodecSelectDia::codec){
        CodecSelectDia::codec=QTextCodec::codecForName("wa");
        S_S.set("textcodecsince263", "wa");
    }
    singleton<charformatsettings>().load();           
    a.addLibraryPath(QApplication::applicationDirPath());
    a.setWindowIcon(QIcon(QApplication::applicationDirPath()+ "/snppictures/tray.png"));
    a.setStyle(new QPlastiqueStyle);
#ifdef Q_WS_WIN
    search_for_game_executables();
    handle_prosnooper_buddys();
#endif                   
    singleton<sound_handler>().init();
    qobjectwrapper<awayhandler>();
    qobjectwrapper<mainwindow> w;
    volume->setvalue(S_S.getint("volumeslidervalue"));
    if (!S_S.getbool("chbminimized"))
        w.ref().show();
    loadusergarbage();
    loadquerylist();       
    QTimer::singleShot(1000,&singleton<sound_handler>(),SLOT(play_startupsound()));    
    return a.exec();
}
void search_for_game_executables(){
    if (S_S.getstringlist("joinstrings").isEmpty()) {
        QSettings settings("Team17SoftwareLTD", "WormsArmageddon");
        QString gamedir(settings.value("PATH").toString());
        QFile wa;
        QFile wormkit;        
        wormkit.setFileName(gamedir + "/WormKit.exe");
        wa.setFileName(gamedir + "/wa.exe");
        QString text;
        bool test = 0;
        if (wormkit.exists())
            text = wormkit.fileName();
        if (wa.exists()) {
            text = text + "\n" + wa.fileName();
            test = 1;
        }        
        if (test)
            S_S.set("joinstrings", text.split("\n",QString::SkipEmptyParts));
    }
}
void handle_prosnooper_buddys(){
    QSettings settings("HKEY_CURRENT_USER\\Software\\ProSnooper",QSettings::NativeFormat);
    QString buddies = settings.value("Buddies").toString();
    QStringList snpbuddies = S_S.getstringlist("buddylist");
    QStringList buddielist;
    if (!buddies.isEmpty())
        buddielist = buddies.split(",", QString::SkipEmptyParts);
    QStringList sl = S_S.getstringlist("prosnooperbuddies");
    foreach(QString s,buddielist) {
        if (!sl.contains(s, Qt::CaseInsensitive)) {
            sl << s;
            if (!snpbuddies.contains(s, Qt::CaseInsensitive))
                snpbuddies << s;
        }
    }
    S_S.set("prosnooperbuddies", sl);
    S_S.set("buddylist", snpbuddies);
}
