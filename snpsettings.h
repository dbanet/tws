/*
 * snpsettings.h
 *
 *  Created on: 24.10.2008
 *      Author: looki
 */

#ifndef SNPSETTINGS_H_
#define SNPSETTINGS_H_
#include<QMap>
#include<QString>
#include<QVariant>
#include<QDataStream>
#include"global_macros.h"
class snpsettings {
public:	
	void load();
	void safe();
        void safeonquit();
	QMap<QString,QVariant> map;

        DECLARE_SINGLETON(snpsettings);
};
#endif /* SNPSETTINGS_H_ */
