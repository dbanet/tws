#ifndef DATABASE_BASE_H
#define DATABASE_BASE_H
#include<QString>
#include<QVariant>
#include<QSqlQueryModel>
#include<QStringList>

class database_base
{
public:
    database_base();

    QString getstring(QString key) const;
    QStringList getstringlist(QString key) const;
    bool getbool(QString key) const;
    int getint(QString key) const;
    QVariantList getlist(QString key) const;
    QByteArray getbytearray(QString key) const;

    void set(QString key, QVariant value);
    void set(QString key, QStringList sl);
    void set(QString key, QVariantList list);
    void remove(QString key, QString s);
    void append(QString key, QString s);

    bool contains(QString key) const;

    bool isOpen();

    bool transaction();
    void commit();

    QStringList dissallowedclannames;
    QStringList buddylist;
    QStringList ignorelist;     
    QStringList combobox_wrapper;                  

    int sbmaximumoftextblocksinlog;    
    int sbsecureloggingrepeatdelay;    

    bool showinformation;
    bool chbjoininfo;
    bool chbpartinfo;
    bool chbquitinfo;
    bool spectateleagueserver;
    bool cbdontshowagradientonverifiedusers;
    bool cbunderlinelinksandclans;
    bool cbunderlineverifiedusers;
    bool cbonlyshowranksfromverifiedusers;
    bool cbshowranksonlyfromsecureloggedusers;    
    bool cbshowipofhosts;
    bool cbsafequerys;        
    bool cbonlyshowflagsfromverifiedusers;        
    bool chbshowbaloonwhenbuddyhosts;
    bool cbdontsortinchannels;
    bool showsmileysinchannels;
    bool chbshowchannelchatinchatwindows;
    bool cbservermessageinchannelwindows;
    bool cbonlywhitegradient;
//##########            

protected:    
    void open();    
    QString databasename;
    void validate();    
    QSqlDatabase db();
private:    
    template<typename T>
    bool appendList(QString key, QVariant value, QSqlQuery &query);
    void checkifexistsinstringlist(QString key,QString value);
    void before_get(QString &key) const;
    void before_set(QString &key, QVariant value) const;
    QVariant get(QString key) const;        
    QHash<QString,QVariantList> hash;    
    void sethash();
    QSqlQueryModel model;

    friend class settingswindow;    
    void update();
    void variantListToStringList(QStringList &sl,const QString key);
    void variantListToString(QString &s,const QString key);
    void variantListToBool(bool &b,const QString key);
    void variantListToInt(int &i,const QString key);
    bool ontransaction;
};
#endif // DATABASE_BASE_H
