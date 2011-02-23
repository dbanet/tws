/*
 * snpsettings.h
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#ifndef charformatsettings_H_
#define charformatsettings_H_
#include<QString>
#include<QVariant>
#include<QDataStream>
#include"global_macros.h"
//void operator<<(QDataStream&,const QMap<QString,QVariant>&);
//void operator>>(QDataStream&,QMap<QString,QVariant>&);
class charformatsettings {
    DECLARE_SINGLETON(charformatsettings)
public:
	void load();
	void safe();
        static QHash<QString,QVariant> map;
};
/*void operator<<(QDataStream &ds,const QMap<QString,QVariant> &m){
	QMap<QString,QVariant>::const_iterator i=m.begin();
	while(i!=m.end()){
		ds<<i.key()<<*i;
		i++;
	}
}
void operator>>(QDataStream &ds,QMap<QString,QVariant> &m){
	QMap<QString,QVariant>::const_iterator i=m.begin();
	QString s;
	QVariant v;
	while(!ds.atEnd()){
		ds>>s;
		ds>>v;
		m[s]=v;
	}
}*/
#endif /* charformatsettings_H_ */
