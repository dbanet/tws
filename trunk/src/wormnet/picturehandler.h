#ifndef PICTUREHANDLER_H
#define PICTUREHANDLER_H

#include"global_macros.h"
#include"userstruct.h"
#include<QMap>
class QPixmap;
class QComboBox;
class picturehandler
{
public:
    QPixmap *getflag(QString country);
    QPixmap *getrank(int i);
    QPixmap *getleaguerank(QString s);
    void fillflags(QComboBox *cb);
    void fillranks(QComboBox *cb);
    QVariant getflag(const userstruct &u) const;
    QString map_hostcountrycode_to_number(QString country);
    QString map_hostnumber_to_countrycode(QString number);
    QString map_countrycode_to_number(QString country);
    QString map_number_to_countrycode(const int i);
    int ranklistsize();

    QPixmap *locked;
    QPixmap *unlocked;

    DECLARE_SINGLETON(picturehandler);
private:
    QMap<QString, QPixmap*> flaglist;
    QList<QPixmap*> ranklist;
    QMap<QString, QPixmap*> leagueranklist;
};

#endif // PICTUREHANDLER_H
