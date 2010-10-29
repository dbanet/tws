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
        const QString &name() const{
            return myname;
        }
        const QString &nick() const{
            return mynick;
        }
        const QString &ip() const{
            return myip;
        }
        const QString &id() const{
            return myid;
        }
        const int &flag() const{
            return myflag;
        }
        const QString &country() const{
            return mycountry;
        }
        const QString &joinstring() const{
            return myjoinstring;
        }
        const QString &hoststring() const{
            return myhoststring;
        }
        const int withkey() const{
            return mywithkey;
        }
        const QString &pwd() const{
            return mypwd;
        }

        void setpwd(QString s);
        bool isvalid();
        static QList<hoststruct> extracthostlist(QString s);
        bool compare(const hoststruct &h) const;
private:
        QString myname;
        QString mynick;
        QString myip;		//ip:port
        int myflag;		//int
        bool mywithkey;
        QString myid;
        QString rest;		//1, 0/1 for key
        QString mycountry;
        QString myjoinstring;
        QString myhoststring;
        QString mypwd;

        bool valid;
	static bool *boolhelper;
        static QList<hoststruct> gethostlist(QString s);
};

#endif /* HOSTSTRUCT_H_ */
