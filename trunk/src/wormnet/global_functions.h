#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H
#include"myDebug.h"
#include<QString>
#include<QStringList>

#include"usermessage.h"

extern QStringList querylist;
class QPixmap;
class QComboBox;

// #include"global_functions.h"
QStringList refreshcombobox(QComboBox *cb);
//void sethostport(QString port);
//QString gethostport();
void appendtoquerylist(QString user);
void safeusergarbage();
void loadusergarbage();
void appendhistory(usermessage u);
const QHash<QString, QList<usermessage> > history();
const QList<usermessage> history(usermessage u);
const QList<usermessage> history(QString user);
QString time();
void safequerylist();
void loadquerylist();
void info(const QString &);
void info(const QStringList &);
void info(int);
void fillString(QString &s, QString ss, int length);

namespace {
    bool compareCI(const QString &s1, const QString &s2) {
        return s1.toLower() == s2.toLower();
    }
    //----------------------------------------------------------------------------------------------
    bool startswithCI(const QString &s1, const QString &s2) {
        return s1.startsWith(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsCI(const QString &s1, const QString &s2) {
        return s1.contains(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsCI(const QStringList &sl, const QString &s2) {
        return sl.contains(s2, Qt::CaseInsensitive);
    }
    //----------------------------------------------------------------------------------------------
    bool containsOneCI(const QString &s, const QStringList &sl, QString *arg) {
        foreach(QString str,sl){
            if(s.contains(str, Qt::CaseInsensitive)){
                if(arg!=NULL)
                    *arg=str;
                return true;
            }
        }
        return false;
    }
    //----------------------------------------------------------------------------------------------
    QStringList removeCI(QStringList &sl, const QString &s) {
        foreach(QString str,sl) {
            if (compareCI(s, str))
                sl.removeAt(sl.indexOf(str));
        }
        return sl;
    }
    //----------------------------------------------------------------------------------------------
    bool containsClickableLink(const QString &s){
        if(containsCI(s,"http://") || containsCI(s,"www.") || containsCI(s,"ftp://") || containsCI(s,"https://"))
            return true;
        return false;
    }
    //----------------------------------------------------------------------------------------------
    bool isClickableLink(const QString &s){
        if(s.startsWith("http://",Qt::CaseInsensitive) || s.startsWith("www.",Qt::CaseInsensitive) || s.startsWith("ftp://",Qt::CaseInsensitive) || s.startsWith("https://",Qt::CaseInsensitive))
            return true;
        return false;
    }
    //----------------------------------------------------------------------------------------------
    QString makeValidColumnName(QString s){
        //return s.remove(":");
        return s.remove('.').remove(':').remove('/').replace(' ','_').remove('-').remove('#');
    }
    //----------------------------------------------------------------------------------------------    
    //----------------------------------------------------------------------------------------------

}
//----------------------------------------------------------------------------------------------
#endif // GLOBAL_FUNCTIONS_H
