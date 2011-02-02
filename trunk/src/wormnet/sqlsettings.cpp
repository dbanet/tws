#include"sqlsettings.h"
#include"myDebug.h"
#include"global_functions.h"
#include"about.h"

#include<QtSql>
#include<stdexcept>
#include<QMessageBox>
#include<QFileDialog>
#include<QApplication>

const char *TABLENAME="settings";
sqlsettings::sqlsettings(){    
}
void sqlsettings::start(QString db){
    databasename=db;
    if(!QDir().exists("query"))
        QDir().mkdir("query");
    if(!QDir().exists("snpini"))
        QDir().mkdir("snpini");    
    bool b=databasexists();
    open();
    if(!language_file.isEmpty())
        set("language_file",language_file);
    if(!b)
        loadsnpiniDefaults();
    model.setQuery(QString("select * from %1").arg(TABLENAME),(QSqlDatabase::database(databasename)));
    QTranslator *trans=new QTranslator;
    QString file=getstring("language_file").remove(".qm");
    if (trans->load(file,"translations/"))
        qApp->installTranslator(trans);
    else
        myDebug() << QObject::tr("The translationfile cannot be loaded! it might be corrupt.")+"  "+file;
    validate();
}
sqlsettings::~sqlsettings(){
}
bool sqlsettings::databasexists(){
    installTranslationBySystemLocale();
    if(QFile::exists(databasename))
        return true;
    if(QFile::exists("snpini/snpini") && !QFile::exists(databasename))
        return importOldini(""),true;
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
        set(s,map[s]);
    return true;
}
void sqlsettings::importOldini(QString folder){
    open();
    QSqlQuery query(QSqlDatabase::database(databasename));
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));    
    if(!loadOldFile(folder+"snpini/snpini"))
        loadsnpiniDefaults();
    if(!loadOldFile(folder+"snpini/settingswindowini"))
        loadsettingswindowDefaults();
}
void sqlsettings::open(){
    if(QSqlDatabase::database(databasename).isOpen())
        return;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",databasename);
    db.setDatabaseName(databasename);
    db.open();
}
void sqlsettings::close(){
    QSqlDatabase::database().close();
}
void sqlsettings::loadsnpiniDefaults(){
    QSqlQuery query(QSqlDatabase::database(databasename));
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    set("volumeslidervalue",5);
    set("chbminimized", 0);
    set("dissallowedclannames", QStringList()<<"Username"<<"cybershadow"<<"WebSnoop"<<"HostingBuddy"<<"SheriffBot"<<"muzer"<<"Help"<<"Miranda"<<"Mirc"<<"wormatty"<<"simon"<<"darkone"<<"noclan"<<"baleegames");
    set("charformatfile", "comic by lookias.textscheme");
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
}
void sqlsettings::validate(){
    checkifexistsinstringlist("leagueservers","http://www.tus-wa.com/");
    checkifexistsinstringlist("leagueservers","http://www.normalnonoobs.org/");
    checkifexistsinstringlist("leagueservers","http://lookias.worms2d.info/securelogging/");
    checkifexistsinstringlist("wormnetserverlist","wormnet1.team17.com");
    checkifexistsinstringlist("wormnetserverlist","itakagames.spb.ru");
    checkifexistsinstringlist("wormnetserverlist","212.240.191.125");
    checkifexistsinstringlist("wormnetserverlist","worms.tom.ru");
    checkifexistsinstringlist("wormnetserverlist","http://steps.servegame.com");

    if(getstring("rank").isEmpty())
        set("rank", "13");
    if(getstring("countrycode").isEmpty())
        set("countrycode", "49");
    if(getstring("qss_file").isEmpty())
        set("qss_file", QString("black by lookias.qss"));
    if(getstring("information").isEmpty() || getstring("information").startsWith("The Wheat Snooper"))
        set("information", "The Wheat Snooper "+about::version);
    if(!contains("showinformation"))
        set("showinformation", true);
    if(!contains("spectatingneversettedoff"))
        set("spectatingneversettedoff", true);

    if(getstring("lestartup").startsWith("wav/"))
        set("lestartup", "wav/startup.mp3");
    if(getstring("lebuddyarrives").startsWith("wav/"))
        set("lebuddyarrives", "wav/buddyarrives.mp3");
    if(getstring("lebuddyleaves").startsWith("wav/"))
        set("lebuddyleaves", "wav/buddyleaves.mp3");
    if(getstring("lebuddychatmessage").startsWith("wav/"))
        set("lebuddychatmessage", "wav/buddymessage.mp3");
    if(getstring("lebuddychatwindowsopened").startsWith("wav/"))
        set("lebuddychatwindowsopened", "wav/buddychatwindowopened.mp3");
    if(getstring("lenormalchatmessage").startsWith("wav/"))
        set("lenormalchatmessage", "wav/normalprivmsg.mp3");
    if(getstring("lehighlightning").startsWith("wav/"))
        set("lehighlightning", "wav/highlightningsound.mp3");
    if(getstring("lecostumword").startsWith("wav/"))
        set("lecostumword", "wav/costumword.mp3");
    if(getstring("lehostsound").startsWith("wav/"))
        set("lehostsound", "wav/buddyhosts.mp3");
    if(getint("sbsecureloggingrepeatdelay")==0)
        set("sbsecureloggingrepeatdelay", 10*1000);
    if(!contains("cbonlyshowranksfromverifiedusers"))
        set("cbonlyshowranksfromverifiedusers", true);
}
void sqlsettings::checkifexistsinstringlist(QString key,QString value){
    QStringList sl=getstringlist(key);
    if(sl.contains(value,Qt::CaseInsensitive))
        return;    
    sl<<value;
    set(key, sl);
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
QStringList sqlsettings::getstringlist(QString key) const{
    before_get(key);    
    QStringList sl;
    for(int i=0;i<model.rowCount();i++){
        if(model.record(i).value(key).isNull())
            break;
        sl<<model.record(i).value(key).toString();
    }
    return sl;
}
QVariantList sqlsettings::getlist(QString key) const{
    before_get(key);   
    QVariantList list;
    for(int i=0;i<model.rowCount();i++){
        if(model.record(i).value(key).isNull())
            break;
        list<<model.record(i).value(key);
    }
    return list;
}
QString sqlsettings::getstring(QString key) const{
    before_get(key);
    return model.record(0).value(key).toString();
}
bool sqlsettings::getbool(QString key) const{
    before_get(key);    
    return model.record(0).value(key).toBool();
}
int sqlsettings::getint(QString key) const{
    before_get(key);    
    return model.record(0).value(key).toInt();
}
QByteArray sqlsettings::getbytearray(QString key) const{
    before_get(key);    
    return model.record(0).value(key).toByteArray();
}
QVariant sqlsettings::get(QString key) const{
    before_get(key);
    return model.record(0).value(key);
}

void sqlsettings::set(QString key, QVariant value){
    if(get(key)==value)
        return;
    before_set(key, value);
    QSqlQuery query(QSqlDatabase::database(databasename));    
    query.exec(QString("alter table %1 add %2 BLOB;").arg(TABLENAME).arg(key));
    if(value.type()==QVariant::List || value.type()==QVariant::StringList){        
        int size=0;
        if(value.type()==QVariant::List)
            size=value.toList().size();
        else if(value.type()==QVariant::StringList)
            size=value.toStringList().size();
        else
            throw std::runtime_error("sqlsettings::set(QString key, QVariant value)");

        query.exec(QString("select * from %1;").arg(TABLENAME));
        while(query.next())
            --size;
        for(int i=0;i<size;i++)
            query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
        int counter = 1;
        if(value.type()==QVariant::List)
            foreach(QVariant v,value.toList()){
            query.prepare(QString("update %1 set %2=:value where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
            query.bindValue("value",v);
            query.exec();
        }
        else if(value.type()==QVariant::StringList)
            foreach(QString v,value.toStringList()){
            query.prepare(QString("update %1 set %2=:value where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
            query.bindValue("value",v);
            query.exec();
        }
        query.exec(QString("update %1 set %2=NULL where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
    } else if(value.type()== QVariant::Int || value.type()== QVariant::Bool || value.type()== QVariant::ByteArray || value.type()== QVariant::Double || value.type()== QVariant::String){
        query.prepare(QString("update %1 set %2=:value where prime=1;").arg(TABLENAME).arg(key));
        query.bindValue("value",value);
        query.exec();
    } else {
        qDebug()<<"void sqlsettings::set(QString key, QVariant value)";
        qDebug()<<QString("type: ") + value.type()+" key: "+ key + "value: "+value.toString();
        //throw std::runtime_error("void sqlsettings::set(QString key, QVariant value)");
    }
    model.setQuery(QString("select * from %1").arg(TABLENAME),(QSqlDatabase::database(databasename)));
}
bool sqlsettings::contains(QString key) const {
    return !model.record(0).field(key).isNull();
}
void sqlsettings::before_get(QString &key) const{
    //    qDebug()<<"get";
    //    qDebug()<<key;
}
void sqlsettings::before_set(QString &key, QVariant value) const{    
    key.replace(" ","_");
    //    qDebug()<<"set";
    //    qDebug()<<key<<"    "<<value;
}
//###########################################################################################################
//########################                       ############################################################
//########################    SETTINGSWINDOW     ############################################################
//########################                       ############################################################
//###########################################################################################################
//###########################################################################################################
void sqlsettings::to_map(const QString &key, const QVariant &value){
    set(key,value);
}
QVariant sqlsettings::from_map(const QString &key) const{
    return get(key);
}
