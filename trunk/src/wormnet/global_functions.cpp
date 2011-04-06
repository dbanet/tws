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
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

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
QString gethostportbyini() {
    QString port=globalport;
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
SOCKET ControlSocket;
QString getwormnat2commandline(){
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0,sizeof(si));
    si.cb = sizeof(si);
    memset( &pi,0, sizeof(pi));
    SECURITY_ATTRIBUTES SecAttr;
    SecAttr.nLength=sizeof(SecAttr);
    SecAttr.bInheritHandle=TRUE;
    SecAttr.lpSecurityDescriptor=0;
    HANDLE WaitEvent=CreateEvent(&SecAttr,0,0,0);
    QString s=QString(" /wnat2 %1-%2-%3").arg(GetCurrentProcessId()).arg(ControlSocket).arg((u_int)WaitEvent);
    char str1[1024];
    sprintf(str1,"wa.exe /wnat2 %u-%u-%u",(u_int)GetCurrentProcessId(),(u_int)ControlSocket,(u_int)WaitEvent);
    CreateProcessA(0,str1,0,0,0,0,0,0,&si,&pi);
    return s;
}
//----------------------------------------------------------------------------------------------
QString wormnatport;
QString getwormnatport(){
    closesocket(ControlSocket);
    sockaddr_in ControlAddr;
    hostent *ControlHost;
    WORD Input;
    WSADATA wsaData;
    WORD ExternalPort=0;
    WORD ControlPort=17018;
    WORD PortError=0xFFFF;

    if (WSAStartup(MAKEWORD(2,2),&wsaData))
        myDebug ()<<QObject::tr("Connection WSAStartup failed %1 ").arg(S_S.getstring ("wormnat2address")) + WSAGetLastError();
    ControlSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    ControlHost=gethostbyname(S_S.getstring ("wormnat2address").toAscii ());
    if(!ControlHost)
    {
        myDebug ()<<QObject::tr("Connection Failed to resolve %1").arg(S_S.getstring ("wormnat2address"))+WSAGetLastError();
        ExternalPort=PortError;
        closesocket(ControlSocket);
        getch();
        return QString();
    }
    ControlAddr.sin_family=AF_INET;
    ControlAddr.sin_port=htons(ControlPort);
    ControlAddr.sin_addr.s_addr=*(DWORD*)ControlHost->h_addr;
    if(connect(ControlSocket,(sockaddr*)&ControlAddr,sizeof(ControlAddr))==SOCKET_ERROR)
    {
        myDebug ()<<QObject::tr("Connection Failed to connect (Error %1)").arg(WSAGetLastError());
        ExternalPort=PortError;
        closesocket(ControlSocket);
        getch();
        return QString();
    }
    if(recv(ControlSocket,(char*)&Input,2,0)!=2)
    {
        myDebug ()<<QObject::tr("Connection Failed to read initial port (Error %1)").arg(WSAGetLastError());
        ExternalPort=PortError;
        closesocket(ControlSocket);
        getch();
        return QString();
    }
    ExternalPort=Input;
    wormnatport=QString::number (ExternalPort);
    return wormnatport;
}
//----------------------------------------------------------------------------------------------
QString lastwormnatport(){
    return wormnatport;

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
void info(char *msg){
    QMessageBox::information(0,"Debug info",msg);
    myDebug()<<msg;
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
void info(const QByteArray &msg){
    QMessageBox::information(0,"Debug info",msg);
    myDebug()<<msg;
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
