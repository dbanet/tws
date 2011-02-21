#ifndef DATABASE_BASE_H
#define DATABASE_BASE_H
#include<QString>
#include<QVariant>
#include<QSqlQueryModel>
#include<QStringList>

class database_base
{
public:
    database_base(QString db);

    QString getstring(QString key) const;
    QStringList getstringlist(QString key) const;
    bool getbool(QString key) const;
    int getint(QString key) const;
    QVariantList getlist(QString key) const;
    QByteArray getbytearray(QString key) const;

    void set(QString key, QVariant value);

    bool contains(QString key) const;

    bool isOpen();

    QStringList dissallowedclannames;
    QStringList buddylist;
    QStringList ignorelist;
    QStringList joinstrings;
    QStringList wormnetserverlist;
    QStringList leagueservers;
    QStringList prosnooperbuddies;
    QStringList combobox_wrapper;
    QStringList mutedusers;

    QString language_file;
    QString qss_file;
    QString joinonstartup;
    QString nickname;
    QString tus_password;
    QString tus_login;
    QString countrycode;
    QString leplayername;
    QString costumipforhosting;
    QString clan;
    QString information;
    QString lenormalchatmessage;
    QString lebuddychatmessage;
    QString lebuddychatwindowsopened;
    QString lebuddyleaves;
    QString lebuddyarrives;
    QString lehighlightning;
    QString lehostsound;
    QString lecostumword;
    QString lestartup;

    int volumeslidervalue;
    int rank;
    int whichuitype;
    int sbmaximumoftextblocks;
    int sbwhorepead;
    int sbmaximumoftextblocksinlog;
    int sbhostrepead;
    int channeltransparency;
    int tusloginmessagenumber;
    int sbsecureloggingrepeatdelay;

    bool cbdontplaysound;
    bool cbcostumword;
    bool chbhostsound;
    bool cbhighlightning;
    bool cbbuddyleaves;
    bool cbplaybuddychatmessage;
    bool cbplaynormalchatmessage;
    bool cbplaybuddychatwindowopened;
    bool cbbuddyarrives;
    bool cbstartup;
    bool cbdontsortinchannels;
    bool cbdontshowballoons;
    bool cbsetawaywhilegaming;
    bool chbdisconnectongame;
    bool chbhidechannelwindowsongame;
    bool useacostumipforhosting;
    bool chbsendhostinfotochan;
    bool chbactionwhenjoining;
    bool chbsmileys;
    bool showinformation;
    bool enablesecurelogging;
    bool spectateleagueserver;
    bool cbalertmeonnotice;
    bool cbalertfromnormal;
    bool cbignorysappearinchannel;
    bool chbjoininfo;
    bool chbpartinfo;
    bool chbquitinfo;
    bool cbopenbuddylistonstartup;
    bool cbdontshowagradientonverifiedusers;
    bool cbunderlinelinksandclans;
    bool cbunderlineverifiedusers;
    bool cbonlyshowranksfromverifiedusers;
    bool cbshowranksonlyfromsecureloggedusers;
    bool chbshowbaloonwhenbuddyhosts;
    bool cbshowipofhosts;
    bool cbsafequerys;
    bool chbshowchannelchatinchatwindows;
    bool chballwaysopenchatwindows;
    bool chbstartchatsminimized;
    bool chbminimized;
    bool chbautojoin;
    bool cbonlyshowflagsfromverifiedusers;
    bool cbservermessageinchannelwindows;
//##########        
    int sbmaximumballonmessages;
    bool chbbuddyballoonleaves;
    bool chbbuddyballoonarives;
    bool chbballoonprivmsg;

protected:    
    void open();
    void sethash();
    QString databasename;
    void validate();    
private:    
    template<typename T>
    bool appendList(QString key, QVariant value, QSqlQuery &query);
    void checkifexistsinstringlist(QString key,QString value);
    void before_get(QString &key) const;
    void before_set(QString &key, QVariant value) const;
    QVariant get(QString key) const;        
    QHash<QString,QVariantList> hash;    
    QSqlQueryModel model;

    friend class settingswindow;    
    void update();
    void variantListToStringList(QStringList &sl,const QString key);
    void variantListToString(QString &s,const QString key);
    void variantListToBool(bool &b,const QString key);
    void variantListToInt(int &i,const QString key);
};
#endif // DATABASE_BASE_H
