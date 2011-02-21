#ifndef SQLSETTINGS_H
#define SQLSETTINGS_H
#include<QString>
#include<QVariantList>
#include<QDebug>

#include"global_macros.h"
#include"database_base.h"

class sqlsettings: public database_base
{
public:        
    void start(QString db);

    DECLARE_SINGLETON(sqlsettings);

private:
    void loadsnpiniDefaults();
    void loadsettingswindowDefaults();    
    void installTranslationBySystemLocale();    
    void open();
    void close();    

    void importOldini(QString folder);

    bool databasexists(); 

    bool loadOldFile(QString file);
    void prepare(QString key,QVariant value);
    void exec();

    bool existingSettingsValid;
};
namespace{
    sqlsettings &S_S=singleton<sqlsettings>();
}
#endif // SQLSETTINGS_H
