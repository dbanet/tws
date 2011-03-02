#include"database_base.h"
#include"about.h"
#include"global_functions.h"

#include<QStringList>
#include<QtSql>
#include<stdexcept>

const char *TABLENAME="settings";
database_base::database_base(){
}
QStringList database_base::getstringlist(QString key) const{
    before_get(key);
    QStringList sl;
    if(hash[key].isEmpty())
        return sl;
    for(int i=0;i<hash[key].size();i++)
        sl<<hash[key].value(i).toString();    
    return sl;
}
QVariantList database_base::getlist(QString key) const{
    before_get(key);
    return hash[key];
}
QString database_base::getstring(QString key) const{
    before_get(key);
    if(hash[key].isEmpty())
        return QString();
    return hash[key].first().toString();
}
bool database_base::getbool(QString key) const{
    before_get(key);
    if(hash[key].isEmpty())
        return bool();
    return hash[key].first().toBool();
}
int database_base::getint(QString key) const{
    before_get(key);
    if(hash[key].isEmpty())
        return int();
    return hash[key].first().toInt();
}
QByteArray database_base::getbytearray(QString key) const{
    before_get(key);
    if(hash[key].isEmpty())
        return QByteArray();
    return hash[key].first().toByteArray();
}
QVariant database_base::get(QString key) const{
    before_get(key);
    QVariantList list;
    QVariant v;
    if(hash[key].isEmpty())
        return v;
    foreach(QVariant v, hash[key]){
        if(v.isNull())
            break;
        list<<v;
    }
    if(list.size()==1)
        return list.first();
    else
        return list;
}
template<typename T>
bool database_base::appendList(QString key, QVariant value, QSqlQuery &query){    
    if(value.value<T>().isEmpty()){
        query.exec(QString("update %1 set '%2'=NULL where prime=1;").arg(TABLENAME).arg(key));
        return true;
    }
    int size=value.value<T>().size()-model.rowCount();
    for(int i=0;i<size;i++)
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    int counter = 1;
    typedef typename T::value_type type;
    foreach(type v,value.value<T>()){
        query.prepare(QString("update %1 set '%2'=? where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
        query.addBindValue(v);
        query.exec();
    }    
    if(query.lastError().isValid())
        qDebug()<<query.lastQuery()<<": \n"<<query.lastError().text()<<"\n#####################";
    query.exec(QString("update %1 set '%2'=NULL where prime=%3;").arg(TABLENAME).arg(key).arg(counter));
    return true;
}
void database_base::append(QString key, QString s){
    if(getstringlist(key).contains(s))
        return;
    set(key,QStringList(getstringlist(key))<<s);
}
void database_base::remove(QString key, QString s){
    QStringList sl=getstringlist(key);
    sl.removeAll(s);
    set(key,sl);
}
void database_base::set(QString key, QVariantList list){    
    if(list.isEmpty())
        set(key,QVariant());
    else
        set(key,QVariant(list));
}
void database_base::set(QString key, QStringList sl){
    if(sl.isEmpty())
        set(key,QVariant());
    else
        set(key,QVariant(sl));
}
void database_base::set(QString key, QVariant value){
    if(get(key)==value || key.isEmpty())
        return;
    before_set(key, value);
    QSqlQuery query(db());
    if(model.record(0).field(key).isNull())
        query.exec(QString("alter table %1 add '%2' BLOB;").arg(TABLENAME).arg(key));
    if(value.type()==QVariant::List)
        appendList<QVariantList>(key,value,query);
    else if(value.type()==QVariant::StringList)
        appendList<QStringList>(key,value,query);
    else if(value.type()== QVariant::Int || value.type()== QVariant::Bool || value.type()== QVariant::ByteArray || value.type()== QVariant::Double || value.type()== QVariant::String){
        query.prepare(QString("update %1 set '%2'=? where prime=1;").arg(TABLENAME).arg(key));
        query.addBindValue(value);
        query.exec();
    } else if(value.isNull()) {
        query.exec(QString("update %1 set '%2'=NULL where prime=1;").arg(TABLENAME).arg(key));
    } else {
        qDebug()<<"void database_base::set(QString key, QVariant value)";
        qDebug()<<QString("type: ") + value.type()+" key: "+ key + "value: "+value.toString();
        throw std::runtime_error("void database_base::set(QString key, QVariant value)");
    }
    if(query.lastError().isValid())
        qDebug()<<query.lastQuery()<<": \n"<<query.lastError().text()<<"\n#####################";
    sethash();    
}
bool database_base::contains(QString key) const {
    return hash.keys().contains(key);
}
void database_base::before_get(QString &key) const{
    //key=makeValidColumnName(key);
//        qDebug()<<"get";
//        qDebug()<<key;
//        qDebug()<<"#########";
}
void database_base::before_set(QString &key, QVariant value) const{
    //key=makeValidColumnName(key);
//    qDebug()<<"set";
//    qDebug()<<key<<value<<get(key);
//    qDebug()<<"#########";
}
void database_base::open(){
    if(isOpen())
        return;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",databasename);
    db.setDatabaseName(databasename);
    db.open();
    QSqlQuery query(db);
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("select d_u_m_m_y from %1").arg(TABLENAME));
    if(!query.next())
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    sethash();
}
QSqlDatabase database_base::db(){
    if(!isOpen())
        open();
    return QSqlDatabase::database(databasename);
}
void database_base::sethash(){
    if(!isOpen()){
        qDebug()<<"void database_base::sethash(){";
        return;
    }
    hash.clear();
    model.setQuery(QString("select * from %1").arg(TABLENAME),(QSqlDatabase::database(databasename)));
    int columnCount=model.record(0).count();
    for(int j=0;j<columnCount;j++){
        QString field=model.record(0).fieldName(j);
        for(int i=0;i<model.rowCount();i++){
            if(model.record(i).value(j).isNull())
                break;
            hash[field]<<model.record(i).value(j);
        }
    }
    update();
}
bool database_base::isOpen(){
    return QSqlDatabase::database(databasename).isOpen();
}

void database_base::variantListToStringList(QStringList &sl,const QString key){
    sl.clear();
    QVariantList list=hash[key];
    if(list.isEmpty())
        return;
    foreach(QVariant v, list)
        sl<<v.toString();
}
void database_base::variantListToString(QString &s,const QString key){
    QVariantList list=hash[key];
    if(list.isEmpty())
        return;
    s=list.first().toString();
}
void database_base::variantListToBool(bool &b,const QString key){
    QVariantList list=hash[key];
    if(list.isEmpty())
        return;
    b=list.first().toBool();
}
void database_base::variantListToInt(int &i,const QString key){
    QVariantList list=hash[key];
    if(list.isEmpty())
        return;
    i=list.first().toInt();
}
void database_base::update(){
    variantListToStringList(dissallowedclannames, "dissallowedclannames");
    variantListToStringList(buddylist, "buddylist");
    variantListToStringList(ignorelist, "ignorelist");        
    variantListToStringList(combobox_wrapper, "combobox_wrapper");    
    variantListToBool(showinformation,"showinformation");    
    variantListToBool(chbjoininfo,"chbjoininfo");
    variantListToBool(chbpartinfo,"chbpartinfo");
    variantListToBool(chbquitinfo,"chbquitinfo");   
    variantListToBool(cbdontshowagradientonverifiedusers,"cbdontshowagradientonverifiedusers");
    variantListToBool(cbunderlinelinksandclans,"cbunderlinelinksandclans");
    variantListToBool(cbunderlineverifiedusers,"cbunderlineverifiedusers");
    variantListToBool(cbonlyshowranksfromverifiedusers,"cbonlyshowranksfromverifiedusers");
    variantListToBool(cbshowranksonlyfromsecureloggedusers,"cbshowranksonlyfromsecureloggedusers");    
    variantListToBool(cbshowipofhosts,"cbshowipofhosts");
    variantListToBool(cbsafequerys,"cbsafequerys");    
    variantListToBool(cbonlyshowflagsfromverifiedusers,"cbonlyshowflagsfromverifiedusers");    
    variantListToBool(spectateleagueserver,"spectateleagueserver");
    variantListToBool(chbshowbaloonwhenbuddyhosts,"chbshowbaloonwhenbuddyhosts");
    variantListToInt(sbmaximumoftextblocksinlog,"sbmaximumoftextblocksinlog");    
    variantListToInt(sbsecureloggingrepeatdelay,"sbsecureloggingrepeatdelay");
    variantListToBool(cbdontsortinchannels,"cbdontsortinchannels");
    variantListToBool(showsmileysinchannels,"showsmileysinchannels");
    variantListToBool(chbshowchannelchatinchatwindows,"chbshowchannelchatinchatwindows");
    variantListToBool(cbservermessageinchannelwindows,"cbservermessageinchannelwindows");
}
void database_base::validate(){
    checkifexistsinstringlist("leagueservers","http://www.tus-wa.com/");
    checkifexistsinstringlist("leagueservers","http://www.normalnonoobs.org/");
    checkifexistsinstringlist("leagueservers","http://lookias.worms2d.info/securelogging/");

    checkifexistsinstringlist("wormnetserverlist","http://wormnet1.team17.com");
    checkifexistsinstringlist("wormnetserverlist","http://itakagames.spb.ru");
    checkifexistsinstringlist("wormnetserverlist","http://212.240.191.125");
    checkifexistsinstringlist("wormnetserverlist","http://worms.tom.ru");
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
    if(getint("whichuitype")<1 || getint("whichuitype")>3)
        set("whichuitype",1);
    if(!contains("showsmileysinchannels"))
        set("showsmileysinchannels",true);
    if(!contains("chbsmileysinchatwindows"))
        set("chbsmileysinchatwindows",true);
    if(!contains("cbshowballoons"))
        set("cbshowballoons", true);
}
void database_base::checkifexistsinstringlist(QString key,QString value){
    QStringList sl=getstringlist(key);
    if(sl.contains(value,Qt::CaseInsensitive))
        return;
    sl<<value;
    set(key, sl);
}
void database_base::transaction(){
    if(!db().transaction())
        myDebug()<<"################void database_base::transaction(){";
}
void database_base::commit(){
    if(!db().commit())
        myDebug()<<"#################void database_base::commit(){";
}
