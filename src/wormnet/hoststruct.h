/*
 * hoststruct.h
 *
 *  Created on: 14.10.2008
 *      Author: looki
 */

#ifndef HOSTSTRUCT_H_
#define HOSTSTRUCT_H_
#include<QString>
#include "picturehandler.h"
class hoststruct {
public:
	hoststruct();
	virtual ~hoststruct();
        void sethost(QStringList&);
        void sethost(QString name,QString nick,QString ip,int flag,QString country,QString id="",QString pwd="",QString chan="", QString countrynumber="1");
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
        const int flag() const{
            return myflag;
        }
        QString flagstring() const{           
            return QString::number(myflag);
        }
        const QString country() const{
            QString s=singleton<picturehandler>().map_hostnumber_to_countrycode(mycountrynumber);
            if(s.isEmpty())
                return mycountry;
            else return s;
        }
        const QString &countrynumber() const{
            return mycountrynumber;
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
        const QString &chan() const{
            return mychan;
        }
        void setpwd(QString s);
        void setchan(QString s);
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
        QString mycountrynumber;
        QString rest;		//1, 0/1 for key
        QString mycountry;
        QString myjoinstring;
        QString myhoststring;
        QString mypwd;
        QString mychan;

        bool valid;
        static QList<hoststruct> gethostlist(QString s);
};

#endif /* HOSTSTRUCT_H_ */
