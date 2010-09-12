/*
 * inihandlerclass.h
 *
 *  Created on: 17.10.2008
 *      Author: looki
 */

#ifndef INIHANDLERCLASS_H_
#define INIHANDLERCLASS_H_
#include<QString>
#include<QTcpSocket>
#include<QNetworkRequest>
class inihandlerclass {
public:
	inihandlerclass();
	virtual ~inihandlerclass();
	void tcpwrite(const QString&,QTcpSocket&);
	QStringList stringlistfromini(const QString&);
	QNetworkRequest requestfromini(const QString&);
private:
	QString file;
};

#endif /* INIHANDLERCLASS_H_ */
