#ifndef DATABASE_BASE_H
#define DATABASE_BASE_H
#include<QString>
#include<QVariant>
#include<QSqlQueryModel>
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
protected:    
    void open();
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
    void sethash();
    QSqlQueryModel model;

    friend class settingswindow;    
};
#endif // DATABASE_BASE_H
