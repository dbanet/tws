#ifndef SQLSETTINGS_H
#define SQLSETTINGS_H
#include <QString>
#include <QVariantList>
#include <QDebug>
#include <QDir>

#include "global_macros.h"
#include "database_base.h"

class sqlsettings: public database_base
{
public:        
    void start(QDir,QString);

    DECLARE_SINGLETON(sqlsettings);

private:
    void loadsnpiniDefaults();
    void loadsettingswindowDefaults();    
    void installTranslationBySystemLocale();    

    void importOldini(QString folder=QString());

    bool databasexists(QDir,QString);

    bool loadOldFile(QString file);
    void keepSettings();
};
namespace{
    sqlsettings &S_S=singleton<sqlsettings>();
}
#endif // SQLSETTINGS_H
