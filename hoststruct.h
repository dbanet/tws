/*
 * hoststruct.h
 *
 *  Created on: 14.10.2008
 *      Author: looki
 */

#ifndef HOSTSTRUCT_H_
#define HOSTSTRUCT_H_
#include<QString>
class hoststruct {
public:
	hoststruct();
	virtual ~hoststruct();
	void sethost(QStringList&);
	QString gamename;
	QString playername;
	QString gameip;		//ip:port
	int flag;		//int
	bool withkey;
	QString gameid;
	QString rest;		//1, 0/1 for key

	QString joinstring;
	QString hoststring;

	static bool *boolhelper;
};

#endif /* HOSTSTRUCT_H_ */
