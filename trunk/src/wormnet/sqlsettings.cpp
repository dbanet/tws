#include"sqlsettings.h"
#include"myDebug.h"
#include"global_functions.h"
#include"progressdialog.h"

#include<QtSql>
#include<stdexcept>
#include<QMessageBox>
#include<QFileDialog>
#include<QApplication>
extern const char *TABLENAME;
const QString cacheddbname=":memory:";
sqlsettings::sqlsettings(): database_base(""){
    existingSettingsValid=true;
}
void sqlsettings::start(QString db){
    databasename=db;
    if(!QDir().exists("query"))
        QDir().mkdir("query");
    if(!QDir().exists("snpini"))
        QDir().mkdir("snpini");    
    open();
    bool b=databasexists();
    database_base::open();
    if(!language_file.isEmpty())
        prepare("language_file",language_file);
    progressdialog *progress=NULL;
    if(!b){
        progress=new progressdialog;
        progress->show();
        progress->raise();
        qApp->processEvents();
        existingSettingsValid=false;
        loadsnpiniDefaults();
        loadsettingswindowDefaults();
        exec();
    }        
    close();    
    QTranslator *trans=new QTranslator;
    QString file=getstring("language_file").remove(".qm");
    if (trans->load(file,"translations/"))
        qApp->installTranslator(trans);
    else
        myDebug() << QObject::tr("The translationfile cannot be loaded! it might be corrupt.")+"  "+file;                
    validate();        
    if(progress)
        progress->close();
}
sqlsettings::~sqlsettings(){
}
void sqlsettings::open(){
    if(QSqlDatabase::database(cacheddbname).isOpen())
        return;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(cacheddbname);
    if(!db.open())
        qDebug()<<"cant open database: "<<cacheddbname;
    QSqlQuery query(QSqlDatabase::database(cacheddbname));
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("select d_u_m_m_y from %1").arg(TABLENAME));
    if(!query.next())
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
}
void sqlsettings::close(){
    QSqlDatabase::database(cacheddbname).close();
}
bool sqlsettings::databasexists(){
    installTranslationBySystemLocale();
    if(QFile::exists(databasename))
        return true;
    if(QFile::exists("snpini/snpini") && !QFile::exists(databasename))
        return importOldini(""), existingSettingsValid=false, true;
    int button=QMessageBox::question(0,QObject::tr("Question")
                                     ,QApplication::tr("If you like to keep the settings from an older Snooper installation click yes.\n"
                                                       "If you use The Wheat Snooper at the first time just click No.")
                                     ,QMessageBox::Yes | QMessageBox::No);
    if(button==QMessageBox::Yes) {
        QString folder;
        while(true) {
            folder=QFileDialog::getExistingDirectory(0,QApplication::tr("Please choose the folder from the old Snooper.")
                                                     ,QApplication::applicationDirPath());
            folder += "/";
            if(QFile::exists(folder+databasename) || QFile::exists("snpini/snpini")){
                break;
            } else {
                int button=QMessageBox::warning(0,QObject::tr("Warning!"),QApplication::tr("This folder doesnt seem to hold a valid installation of The Wheat Snooper. Do you want to keep searching?"),QMessageBox::Yes | QMessageBox::No);
                if(button==QMessageBox::Yes)
                    continue;
                else
                    return false;
            }
        }
        if(QFile::exists(folder + "snpini/snpini") && !QFile::exists(folder + databasename))
            importOldini(folder);
        else
            QFile::copy(folder+databasename,databasename);        
        QFile::copy(folder+"snpini/ctcp.ini","snpini/ctcp.ini");
        QFile::copy(folder+"snpini/clanpages","snpini/clanpages");
        QFile::copy(folder+"query/log","query/log");
        QFile::copy(folder+"query/querylist","query/querylist");
    } else
        return false;
    return true;
}
bool sqlsettings::loadOldFile(QString file){
    QFile f(file);
    if(!f.open(QFile::ReadOnly)){
        myDebug()<<"Cant load: "<<file;
        return false;
    }
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    QMap<QString, QVariant> map;
    ds>>map;
    foreach(QString s, map.keys())
        prepare(s,map[s]);
    return true;
}
void sqlsettings::importOldini(QString folder){
    QSqlQuery query(QSqlDatabase::database(cacheddbname));
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    if(!loadOldFile(folder+"snpini/snpini"))
        loadsnpiniDefaults();
    if(!loadOldFile(folder+"snpini/settingswindowini"))
        loadsettingswindowDefaults();
}
void sqlsettings::loadsnpiniDefaults(){
    QSqlQuery query(QSqlDatabase::database(cacheddbname));
    query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    prepare("volumeslidervalue",5);
    prepare("chbminimized", 0);
    prepare("dissallowedclannames", QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
    prepare("charformatfile", "comic by lookias.textscheme");
    prepare("chbsendhostinfotochan", true);
    prepare("language_file", "_en.Standard.qm");
    installTranslationBySystemLocale();
}
void sqlsettings::loadsettingswindowDefaults(){
    prepare("leawaystring", "I'm in a Game!");
    prepare("lebackmessage", "I'm back.");

    prepare("lestartup", "wav/startup.mp3");
    prepare("lebuddyarrives", "wav/buddyarrives.mp3");
    prepare("lebuddyleaves", "wav/buddyleaves.mp3");
    prepare("lebuddychatmessage", "wav/buddymessage.mp3");
    prepare("lebuddychatwindowsopened", "wav/buddychatwindowopened.mp3");
    prepare("lenormalchatmessage", "wav/normalprivmsg.mp3");
    prepare("lehighlightning", "wav/highlightningsound.mp3");
    prepare("lecostumword", "wav/costumword.mp3");
    prepare("lehostsound", "wav/buddyhosts.mp3");

    prepare("cbalertmeonnotice", true);
    prepare("cbalertfromnormal", true);
    prepare("chbactionwhenjoining", true);
    prepare("cbsetawaywhilegaming", true);
    prepare("chbbacktonormals", true);
    prepare("chbbacktobuddys", true);
    prepare("cbignorysappearinchannel", true);
    prepare("cbsafequerys", true);
    prepare("cbopenbuddylistonstartup", true);
    prepare("chbshowchannelchatinchatwindows", true);
    prepare("cbservermessageinchannelwindows", true);
    prepare("chbjoininfo", true);
    prepare("chbpartinfo", true);
    prepare("chbquitinfo", true);
    prepare("chbbuddyballoonarives", true);
    prepare("chbbuddyballoonleaves", true);
    prepare("chbballoonprivmsg", true);
    prepare("chbshowbaloonwhenbuddyhosts", true);
    prepare("cbstartup", true);
    prepare("cbbuddyarrives", true);
    prepare("cbbuddyleaves", true);
    prepare("cbplaybuddychatmessage", true);
    prepare("cbplaybuddychatwindowopened", true);
    prepare("cbplaynormalchatmessage", true);
    prepare("cbhighlightning", true);
    prepare("cbcostumword", true);
    prepare("chbhostsound", true);

    prepare("sbmaximumoftextblocksinlog", 60);
    prepare("sbmaximumballonmessages", 3);
    prepare("sbwhorepead", 3000);
    prepare("sbhostrepead", 15000);
    prepare("sbmaximumoftextblocks", 500);
    prepare("sbsecureloggingrepeatdelay", 10*100);
    prepare("cbonlyshowranksfromverifiedusers", true);
}
void sqlsettings::installTranslationBySystemLocale(){
    QTranslator *trans=new QTranslator;
    QString language=QLocale::system().name().left(2);
    QDir dir("translations");
    foreach(QString s,dir.entryList()){
        if(s.startsWith("_") && s.mid(1,2)==language){
            trans->load(s,"translations/");
            qApp->installTranslator(trans);
            language_file=s;
            return;
        }
    }
    if(trans->load("_en.Standard.qm","translations/")){
        qApp->installTranslator(trans);
        language_file="_en.Standard.qm";
    }
    return;
}
void sqlsettings::prepare(QString key,QVariant value){
    key.replace(" ","_");
    QSqlQuery query(QSqlDatabase::database(cacheddbname));
    query.exec(QString("alter table %1 add %2 BLOB;").arg(TABLENAME).arg(key));
    if(value.isNull() || key.isEmpty())
        return;
    if(value.type()==QVariant::List || value.type()==QVariant::StringList){
        int size=0;
        if(value.type()==QVariant::List)
            size=value.toList().size();
        else if(value.type()==QVariant::StringList)
            size=value.toStringList().size();
        query.exec(QString("select * from %1;").arg(TABLENAME));
        while(query.next())
            --size;
        for(int i=0;i<size;i++)
            query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
        int counter = 1;
        if(value.type()==QVariant::List){
            foreach(QVariant v,value.toList()){
                query.prepare(QString("update %1 set %2=? where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
                query.addBindValue(v);
                query.exec();
            }
        } else if(value.type()==QVariant::StringList){
            foreach(QString v,value.toStringList()){                
                query.prepare(QString("update %1 set %2=? where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
                query.addBindValue(v);
                query.exec();
            }
        }       
        query.exec(QString("update %1 set %2=NULL where prime=%3;").arg(TABLENAME).arg(key).arg(counter));
    } else if(value.type()== QVariant::Int || value.type()== QVariant::Bool || value.type()== QVariant::ByteArray || value.type()== QVariant::Double || value.type()== QVariant::String){
        query.prepare(QString("update %1 set %2=:value where prime=1;").arg(TABLENAME).arg(key));
        query.bindValue("value",value);
        query.exec();
    } else {
        qDebug()<<"void sqlsettings::prepare(QString key, QVariant value)";
        qDebug()<<QString("type: ") + value.type();
        qDebug()<<" key: "+ key;
        qDebug()<<"value: "+value.toString();
        qDebug()<<value.isNull();
        throw std::runtime_error("void database_base::prepare(QString key, QVariant value)");
    }
    if(query.lastError().isValid())
        qDebug()<<query.lastQuery()<<":\n"<<query.lastError().text()<<"\n#####################";
}
void sqlsettings::exec(){
    if(existingSettingsValid)
        return;
    if(!isOpen()){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",databasename);
        db.setDatabaseName(databasename);
        db.open();
    }
    QSqlQueryModel model;
    model.setQuery(QString("select * from %1;").arg(TABLENAME),QSqlDatabase::database(cacheddbname));
    QSqlRecord record=model.record(0);
    QSqlQuery query(QSqlDatabase::database(databasename));
    QStringList sl;    
    for(int i=0;i<record.count();i++){
        sl<<makeValidColumnName(record.field(i).name());
        query.exec(QString("alter table %1 add %2 BLOB;").arg(TABLENAME).arg(sl.last()));
        qApp->processEvents();
    }
    sl.removeDuplicates();
    sl.removeAll("d_u_m_m_y");
    sl.removeAll("prime");
    sl.replaceInStrings("BLOB","d_u_m_m_y_2");        

    for(int i=0;i<model.rowCount();i++){
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
        QString s=QString("update %1 set %2 where prime=%3;").arg(TABLENAME).arg(sl.join("=?, ")+"=?").arg(i+1);
        query.prepare(s);
        for(int j=0;j<sl.size();j++){
            query.addBindValue(model.record(i).value(j+2));
        }
        query.exec();        
        qApp->processEvents();
        if(query.lastError().isValid())
            qDebug()<<query.lastQuery()<<":\n"<<query.lastError().text()<<"\n#####################";
    }             
    sethash();
}
