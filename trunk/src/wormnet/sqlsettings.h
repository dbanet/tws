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
    void start(QString db, QString old);

    QString getstring(QString key) const;
    QStringList getstringlist(QString key) const;
    bool getbool(QString key) const;
    int getint(QString key) const;
    QVariantList getlist(QString key) const;
    QByteArray getbytearray(QString key) const;

    void set(QString key, QVariant value);

    bool contains(QString key) const;       

    DECLARE_SINGLETON(sqlsettings);

private:
    void loadDefaults();
    QVariant get(QString key);
    void validate();
    void installTranslationBySystemLocale();
    void checkifexistsinstringlist(QString key,QString value);
    void open();
    void close();

    void before_get(QString &key) const;
    void before_set(QString &key, QVariant value) const;

    void importOldSnpini(QString folder);

    QSqlQueryModel model;

    bool databasexists();

    QString language_file;
    QString databasename;
    QString oldsettingsfile;

    friend class settingswindow;
};
namespace{
    sqlsettings &S_S=singleton<sqlsettings>();
}
#endif // SQLSETTINGS_H
