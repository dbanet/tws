#ifndef SQLSETTINGS_H
#define SQLSETTINGS_H
#include<QString>
#include<QVariantList>
#include<QDebug>
#include<QSqlQueryModel>

#include"global_macros.h"

class sqlsettings
{
public:    
    void loadDefaults();
    void start();

    QString getstring(QString key) const;
    QStringList getstringlist(QString key) const;
    bool getbool(QString key) const;
    int getint(QString key) const;
    QVariantList getlist(QString key) const;
    QByteArray getbytearray(QString key) const;

    void firstrun();

    void set(QString key, QVariant value);

    bool contains(QString key) const;       

    DECLARE_SINGLETON(sqlsettings);

private:
    void validate();
    void installTranslationBySystemLocale();
    void checkifexistsinstringlist(QString key,QString value);
    void open();
    void close();

    void before_get(QString key) const;
    void before_set(QString key, QVariant value) const;

    QSqlQueryModel model;
};
namespace{
    sqlsettings &S_S=singleton<sqlsettings>();
}
#endif // SQLSETTINGS_H
