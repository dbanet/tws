#include"sqlsettings.h"
#include"myDebug.h"
#include"global_functions.h"

#include<QtSql>
#include<stdexcept>
#include<QMessageBox>
#include<QFileDialog>
#include<QApplication>
extern const char *TABLENAME;
sqlsettings::sqlsettings(){
}
void sqlsettings::start(QString arg){
    databasename=arg;
    if(!QDir().exists("query"))
        QDir().mkdir("query");
    if(!QDir().exists("snpini"))
        QDir().mkdir("snpini");
    bool b=databasexists();    
    db().transaction();
    if(!b){
        loadsnpiniDefaults();
        loadsettingswindowDefaults();
    }         
    QTranslator *trans=new QTranslator;
    QString file=getstring("language_file").remove(".qm");
    if (trans->load(file,"translations/"))
        qApp->installTranslator(trans);
    else
        myDebug() << QObject::tr("The translationfile cannot be loaded! it might be corrupt.")+"  "+file;                
    validate();            
    db().commit();
}
sqlsettings::~sqlsettings(){
}
bool sqlsettings::databasexists(){    
    if(QFile::exists(databasename))
        return true;
    installTranslationBySystemLocale();
    db().transaction();    
    if(QFile::exists("snpini/snpini"))
        return importOldini(), true;
    int button=QMessageBox::question(0,QObject::tr("Question")
                                     ,QApplication::tr("If you like to keep the settings from an older Snooper installation click yes.\n"
                                                       "If you use The Wheat Snooper at the first time just click No.")
                                     ,QMessageBox::Yes | QMessageBox::No);
    if(button==QMessageBox::Yes)
        keepSettings();
    else
        return false;
    return true;
}
void sqlsettings::keepSettings(){
    QString folder;
    while(true) {
        folder=QFileDialog::getExistingDirectory(0,QApplication::tr("Please choose the folder from the old Snooper.")
                                                 ,QApplication::applicationDirPath());
        folder += "/";
        if(QFile::exists(folder+databasename) || QFile::exists("snpini/snpini"))
            break;
        else {
            int button=QMessageBox::warning(0,QObject::tr("Warning!"),QApplication::tr("This folder doesnt seem to hold a valid installation of The Wheat Snooper. Do you want to keep searching?"),QMessageBox::Yes | QMessageBox::No);
            if(button==QMessageBox::Yes)
                continue;
            else
                return;
        }
    }
    if(QFile::exists(folder + "snpini/snpini") && !QFile::exists(folder + databasename))
        importOldini(folder);
    else
        QFile::copy(folder+databasename,databasename);
    QFile::copy(folder+"snpini/ctcp.ini","snpini/ctcp.ini");
    QFile::copy(folder+"query/log","query/log2");
    QFile::copy(folder+"query/querylist","query/querylist");
}
bool sqlsettings::loadOldFile(QString file){
    QFile f(file);
    if(!f.open(QFile::ReadOnly)){
        myDebug()<<"Cant load: "<<file;
        return false;
    }
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    QHash<QString, QVariant> hash;
    ds>>hash;
    foreach(QString s, hash.keys())
        set(s,hash[s]);
    return true;
}
void sqlsettings::importOldini(QString folder){    
    if(!loadOldFile(folder+"snpini/snpini"))
        loadsnpiniDefaults();
    if(!loadOldFile(folder+"snpini/settingswindowini"))
        loadsettingswindowDefaults();
}
void sqlsettings::loadsnpiniDefaults(){    
    set("volumeslidervalue",5);
    set("chbminimized", 0);
    set("dissallowedclannames", QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
    set("textscheme", "comic by lookias.textscheme");
    set("chbsendhostinfotochan", true);
    set("language_file", "_en.Standard.qm");
    installTranslationBySystemLocale();
}
void sqlsettings::loadsettingswindowDefaults(){
    set("leawaystring", "I'm in a Game!");
    set("lebackmessage", "I'm back.");

    set("lestartup", "wav/startup.mp3");
    set("lebuddyarrives", "wav/buddyarrives.mp3");
    set("lebuddyleaves", "wav/buddyleaves.mp3");
    set("lebuddychatmessage", "wav/buddymessage.mp3");
    set("lebuddychatwindowsopened", "wav/buddychatwindowopened.mp3");
    set("lenormalchatmessage", "wav/normalprivmsg.mp3");
    set("lehighlightning", "wav/highlightningsound.mp3");
    set("lecostumword", "wav/costumword.mp3");
    set("lehostsound", "wav/buddyhosts.mp3");

    set("cbalertmeonnotice", true);
    set("cbalertfromnormal", true);
    set("chbactionwhenjoining", true);
    set("cbsetawaywhilegaming", true);
    set("chbbacktonormals", true);
    set("chbbacktobuddys", true);
    set("cbignorysappearinchannel", true);
    set("cbsafequerys", true);
    set("cbopenbuddylistonstartup", true);
    set("chbshowchannelchatinchatwindows", true);
    set("cbservermessageinchannelwindows", true);
    set("chbjoininfo", true);
    set("chbpartinfo", true);
    set("chbquitinfo", true);
    set("chbbuddyballoonarives", true);
    set("chbbuddyballoonleaves", true);
    set("chbballoonprivmsg", true);
    set("chbshowbaloonwhenbuddyhosts", true);
    set("cbstartup", true);
    set("cbbuddyarrives", true);
    set("cbbuddyleaves", true);
    set("cbplaybuddychatmessage", true);
    set("cbplaybuddychatwindowopened", true);
    set("cbplaynormalchatmessage", true);
    set("cbhighlightning", true);
    set("cbcostumword", true);
    set("chbhostsound", true);

    set("sbmaximumoftextblocksinlog", 60);
    set("sbmaximumballonmessages", 3);
    set("sbwhorepead", 3000);
    set("sbhostrepead", 15000);
    set("sbmaximumoftextblocks", 500);
    set("sbsecureloggingrepeatdelay", 10*100);
    set("cbonlyshowranksfromverifiedusers", true);
    set("showsmileysinchannels", true);
}
void sqlsettings::installTranslationBySystemLocale(){
    QTranslator *trans=new QTranslator;
    QString language=QLocale::system().name().left(2);
    QDir dir("translations");
    foreach(QString s,dir.entryList()){
        if(s.startsWith("_") && s.mid(1,2)==language){
            trans->load(s,"translations/");
            qApp->installTranslator(trans);
            set("language_file", s);
            return;
        }
    }
    if(trans->load("_en.Standard.qm","translations/")){
        qApp->installTranslator(trans);
        set("language_file", "_en.Standard.qm");
    }
    return;
}
