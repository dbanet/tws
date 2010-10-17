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
#include"global_macros.h"
class snpsettings {
public:	
	void load();
	void safe();
        void safeonquit();
        void loadDefaults();
	QMap<QString,QVariant> map;

        DECLARE_SINGLETON(snpsettings);
private:
        void installTranslationBySystemLocale();
        QString version;
        void validate();
};
#endif /* SNPSETTINGS_H_ */
