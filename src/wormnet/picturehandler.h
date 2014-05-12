#ifndef PICTUREHANDLER_H
#define PICTUREHANDLER_H

#include "global_macros.h"
#include "userstruct.h"
#include <QHash>
class QPixmap;
class QComboBox;
class pictureHandler
{
public:
    QPixmap *getFlag(QString country);
    QPixmap *getRank(int i);
    QPixmap *getLeagueRank(QString s);
    void fillFlags(QComboBox *cb);
    void fillRanks(QComboBox *cb);
    QVariant getFlag(const userstruct &u) const;
    QString mapHostCountryCodeToNumber(QString country);
    QString mapHostNumberToCountryCode(QString number);
    QString mapCountryCodeToNumber(QString country);
    QString mapNumberToCountryCode(const int i);
    int ranklistsize();

    QPixmap *locked;
    QPixmap *unlocked;

    DECLARE_SINGLETON(pictureHandler);
private:
    QHash<QString, QPixmap*> flaglist;
    QList<QPixmap*> ranklist;
    QHash<QString, QPixmap*> leagueranklist;
};

#endif // PICTUREHANDLER_H
