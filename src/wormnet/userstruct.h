/*
 * userstruct.h
 *
 *  Created on: 16.10.2008
 *      Author: looki
 */

#ifndef USERSTRUCT_H_
#define USERSTRUCT_H_
#include <QStringList>
class userstruct {
public:
	userstruct(QStringList);
        userstruct(
            QString channel="",
            QString clan="",
            QString address="",
            QString server="",
            QString nick="",
            QString userMode="H",
            int hopCount=0,
            int flag=49,
            int rank=12,
            QString country="??",
            QString client="Unknown"
        );
	virtual ~userstruct();
	bool operator==(const userstruct&);
        QStringList gamerWho();
	static userstruct whoami(const QString &,const QString &s="");

	QString channel;
        QString clan;
	QString address;
	QString server;
	QString nick;
        QString userMode;
        int hopCount;
	int flag;
        int rank;
	QString country;
	QString client;

	bool wnvalid;		//this nick is to 80% a valid wn user
};
#endif /* USERSTRUCT_H_ */
