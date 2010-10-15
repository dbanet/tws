/*
 * hoststruct.cpp
 *
 *  Created on: 14.10.2008
 *      Author: looki
 */

#include "hoststruct.h"
#include<QStringList>
#include<QPixmap>
bool *hoststruct::boolhelper = new bool;
extern QList<QPixmap*> flaglist; //declared in main.cpp
extern int flaglistsize;
hoststruct::hoststruct() {
	// TODO Auto-generated constructor stub
}

hoststruct::~hoststruct() {
	// TODO Auto-generated destructor stub
}
void hoststruct::sethost(QStringList &sl) {
	Q_ASSERT(sl.size()>=7);
	//wa://'+Edit1.Text+Port+'?gameid='+Edit3.Text+'&scheme='+Edit4.Text;
	if (sl.size() >= 7) {
		gamename = sl.takeFirst();
		playername = sl.takeFirst();
		gameip = sl.takeFirst();
		flag = sl.takeFirst().toInt(boolhelper);
		if (!(*boolhelper && flag < flaglistsize && flag >= 0))
			flag = 49;                
		sl.takeFirst();
		withkey = sl.takeFirst().toInt();
		gameid = sl.takeFirst();
		joinstring = "wa://" + gameip + "?gameid=" + gameid;
		hoststring = QString("wa://") + "?gameid=" + gameid;
		rest = sl.join(" ");
	} else {
		if (sl.size() >= 1)
			gamename = sl.takeFirst();
		if (sl.size() >= 1)
			playername = sl.takeFirst();
		if (sl.size() >= 1)
			gameip = sl.takeFirst();
		if (sl.size() >= 1)
			flag = sl.takeFirst().toInt(boolhelper);
		if (sl.size() >= 1)
			if (!(*boolhelper && flag < flaglistsize && flag >= 0))
				flag = 49;
		if (sl.size() >= 1)
			sl.takeFirst();
		if (sl.size() >= 1)
			withkey = sl.takeFirst().toInt();
		if (sl.size() >= 1)
			gameid = sl.takeFirst();
		joinstring = "wa://" + gameip + "?gameid=" + gameid;
		hoststring = QString("wa://") + "?gameid=" + gameid;
		rest = sl.join(" ");
	}        
}
