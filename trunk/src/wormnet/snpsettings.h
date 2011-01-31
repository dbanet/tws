
 /* snpsettings.h
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#ifndef SNPSETTINGS_H_
#define SNPSETTINGS_H_
#include<QMap>
#include<QString>
#include<QVariant>
#include"global_macros.h"
class snpsettings {
public:
    void start(){load();}
    void load();
    void safe();
    void safeonquit();
    void loadDefaults();

    QString getstring(QString key) const;
    QStringList getstringlist(QString key) const;
    bool getbool(QString key) const;
    int getint(QString key) const;
    QVariantList getlist(QString key) const;
    QByteArray getbytearray(QString key) const;

    void set(QString key, QVariant value);

    bool contains(QString key) const;

    DECLARE_SINGLETON(snpsettings);
private:
    QMap<QString,QVariant> map;
    void installTranslationBySystemLocale();
    QString version;
    void validate();
    void checkifexistsinstringlist(QString key,QString value);
};
namespace{
    snpsettings &S_S=singleton<snpsettings>();
}
#endif /* SNPSETTINGS_H_*/
