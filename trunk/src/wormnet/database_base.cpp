#include"database_base.h"
#include"about.h"
#include"global_functions.h"

#include<QStringList>
#include<QtSql>
#include<stdexcept>
const char *TABLENAME="settings";
database_base::database_base(QString db): databasename(db){
}
QStringList database_base::getstringlist(QString key) const{
    before_get(key);
    QStringList sl;
    for(int i=0;i<hash[key].size();i++){
        if(hash[key].isEmpty())
            break;
        sl<<hash[key].value(i).toString();
    }
    return sl;
}
QVariantList database_base::getlist(QString key) const{
    before_get(key);
    return hash[key];
}
QString database_base::getstring(QString key) const{
    before_get(key);
    return hash[key].first().toString();
}
bool database_base::getbool(QString key) const{
    before_get(key);
    return hash[key].first().toBool();
}
int database_base::getint(QString key) const{
    before_get(key);
    return hash[key].first().toInt();
}
QByteArray database_base::getbytearray(QString key) const{
    before_get(key);
    return hash[key].first().toByteArray();
}
QVariant database_base::get(QString key) const{
    before_get(key);
    QVariantList list;
    QVariant v;
    for(int i=0;i<hash[key].size();i++){
        QVariant v=hash[key][i];
        if(v.isNull())
            break;
        list<<v;
    }
    if(list.size()==1)
        return list[0];
    else if(list.size()>1){
        QStringList sl;
        foreach(QVariant v,list)
            sl<<v.toString();
        return sl;
    }
    else
        return QVariant();
}
template<typename T>
bool database_base::appendList(QString key, QVariant value, QSqlQuery &query){
    if(value.value<T>().isEmpty())
        return false;
    int size=0;
    size=value.value<T>().size();
    size=size-hash[key].size();
    for(int i=0;i<size;i++)
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    int counter = 1;
    typedef typename T::value_type type;
    foreach(type v,value.value<T>()){
        query.prepare(QString("update %1 set %2=? where prime=%3;").arg(TABLENAME).arg(key).arg(counter++));
        query.addBindValue(v);
        query.exec();
    }
    qDebug()<<key<<"# "<<value.value<T>()<<"# "<<get(key);
    return true;
}
void database_base::set(QString key, QVariant value){
    before_set(key, value);
    if(get(key)==value || key.isEmpty())
        return;
    QSqlQuery query(QSqlDatabase::database(databasename));
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
    key=makeValidColumnName(key);
    qDebug()<<"get";
    qDebug()<<key;
    qDebug()<<"#########";
}
void database_base::before_set(QString &key, QVariant value) const{
    key=makeValidColumnName(key);
    //    qDebug()<<"set";
    //    qDebug()<<key<<"    "<<value;
}
void database_base::open(){
    if(QSqlDatabase::database(databasename).isOpen())
        return;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",databasename);
    db.setDatabaseName(databasename);
    db.open();
    QSqlQuery query(QSqlDatabase::database(databasename));
    query.exec(QString("CREATE TABLE if not exists %1 (prime INTEGER PRIMARY KEY ASC, d_u_m_m_y bool);").arg(TABLENAME));
    query.exec(QString("select d_u_m_m_y from %1").arg(TABLENAME));
    if(!query.next())
        query.exec(QString("insert into %1(d_u_m_m_y) values(1);").arg(TABLENAME));
    sethash();
}
void database_base::sethash(){
    model.setQuery(QString("select * from %1").arg(TABLENAME),(QSqlDatabase::database(databasename)));
    int columnCount=model.record(0).count();
    for(int j=0;j<columnCount;j++){
        QString field=model.record(0).fieldName(j);
        for(int i=0;i<model.rowCount();i++){
            if(model.record(i).value(j).isNull())
                continue;
            hash[field]<<model.record(i).value(j);
        }
    }
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
}
void database_base::checkifexistsinstringlist(QString key,QString value){
    QStringList sl=getstringlist(key);
    if(sl.contains(value,Qt::CaseInsensitive))
        return;
    sl<<value;
    set(key, sl);
}
