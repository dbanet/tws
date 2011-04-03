#include<QComboBox>
#include<QTextStream>
#include<QProcess>
#include<QFile>
#include<QApplication>
#include<QMessageBox>
#include<QDir>
#include<QDate>
#include<QTextCodec>

#include<windows.h>

#include"global_functions.h"
#include"settings.h"
#include"usermessage.h"
#include"netcoupler.h"
#include"codecselectdia.h"

typedef QHash<QString, QList<usermessage> > history_type;
history_type my_history;
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
QString gethostport() {
    QString port;
    port=globalport;
#ifdef Q_WS_WIN
    port=get_winini_key ("HostingPort");
    if(port.isEmpty ())
        port=globalport;
#endif
    return port;
}
void sethostport(QString port) {
    if(port.isEmpty())
        port="17011";
    globalport=port;

#ifdef Q_WS_WIN
    set_winini_key ("HostingPort",port);
#endif
}
QString get_winini_key(QString key){
    char *p=new char[255];
    GetProfileStringA("NetSettings",key.toAscii (),"",p,255);    
    QString s=QTextCodec::codecForName ("wa")->toUnicode (p);
    delete[] p;
    return s;
}
bool set_winini_key(QString key, QString value){
    return WriteProfileStringA("NetSettings"
                               , key.toAscii ()
                               , QTextCodec::codecForName ("wa")->fromUnicode (value));
}
//----------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &ds, const usermessage &u){
    return ds<<u.msg()<<u.type()<<u.user()<<u.receiver()<<u.time();
}
//----------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &ds, usermessage &u){
    return ds>>u.my_msg>>u.my_type>>u.my_user>>u.my_receiver>>u.my_time;
}
//----------------------------------------------------------------------------------------------
void safeusergarbage() {
    if(!S_S.getbool("cbsafequerys"))
        return;
    QFile f(QApplication::applicationDirPath() + "/query/log2");
    if (!f.open(QFile::WriteOnly | QFile::Truncate))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);    
    ds << my_history;
}
//----------------------------------------------------------------------------------------------
void loadusergarbage() {
    if(!S_S.getbool("cbsafequerys"))
        return;
    QFile f(QApplication::applicationDirPath() + "/query/log2");
    QString s;
    history_type::mapped_type sl;
    if (!f.open(QFile::ReadOnly))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds >> my_history;
}
//----------------------------------------------------------------------------------------------
void appendhistory(usermessage u){
    if(u.receiver() != singleton<netcoupler>().nick && !S_S.chbshowchannelchatinchatwindows)
        return;
    QString user;
    if(u.user()==singleton<netcoupler>().nick)
        user=u.receiver();
    else
        user=u.user();
    user=user.toLower();

    QString s;

    if (my_history[user].size() > S_S.sbmaximumoftextblocksinlog)
        my_history[user] = my_history[user].mid(S_S.sbmaximumoftextblocksinlog * 2/ 3);;
    u.settime(time());
    u.add_type(e_GARBAGE);
    if (my_history[user].size() < S_S.sbmaximumoftextblocksinlog)
        my_history[user.toLower()] << u;
    else {
        my_history[user] = my_history[user].mid(S_S.sbmaximumoftextblocksinlog * 2/ 3);
        my_history[user] <<  u;
    }
}
//----------------------------------------------------------------------------------------------
QString time(){
    return QDate::currentDate().toString("dd.MM") + " "+ QTime::currentTime().toString("hh:mm");
}
//----------------------------------------------------------------------------------------------
const QHash<QString, QList<usermessage> > history(){
    return my_history;
}
//----------------------------------------------------------------------------------------------
const QList<usermessage> history(usermessage u){
    return my_history[u.user()];
}
//----------------------------------------------------------------------------------------------
const QList<usermessage> history(QString user){
    return my_history[user];
}
//----------------------------------------------------------------------------------------------
void safequerylist() {
    if(!S_S.getbool("cbsafequerys"))
        return;
    QFile f(QApplication::applicationDirPath() + "/query/querylist");
    if (!f.open(QFile::WriteOnly | QFile::Truncate))
        return;
    QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_3);
    ds << querylist;
}
//----------------------------------------------------------------------------------------------
void loadquerylist() {
    if(!S_S.getbool("cbsafequerys"))
        return;
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
    myDebug()<<msg;
}
//----------------------------------------------------------------------------------------------
void info(int i){
    QMessageBox::information(0,"Debug info",QString::number(i));
    myDebug()<<i;
}
//----------------------------------------------------------------------------------------------
void fillString(QString &s, QString ss, int length){
    s.clear();
    for(int i=0;i<length;i++)
        s+=ss;
}
//----------------------------------------------------------------------------------------------
void info(const QStringList &sl){
    info(sl.join(", "));
}
//----------------------------------------------------------------------------------------------
void appendtoquerylist(QString user){
    if(containsCI(S_S.ignorelist,user))
        return;
    if (!containsCI(querylist, user))
        querylist << user;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
