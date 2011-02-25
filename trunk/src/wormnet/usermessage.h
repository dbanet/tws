#ifndef USERMESSAGE_H
#define USERMESSAGE_H
#include<QString>
#include"global_functions.h"
#include"global_macros.h"
class usermessage{    
public:    
    usermessage(QString s_arg, type t_arg, QString arg_u, QString arg_r):
            my_msg(s_arg), my_type(t_arg), my_user(arg_u), my_receiver(arg_r){

        Q_ASSERT(my_type == e_NOTICE || my_type == e_PRIVMSG);
        if(my_msg.startsWith("\001ACTION",Qt::CaseInsensitive)){
            add_type(e_ACTION);
            my_msg.remove("\001ACTION",Qt::CaseInsensitive).remove('\001');
        } else if(my_msg.startsWith('\001')){
            add_type(e_CTCP);
            my_msg.remove('\001');
        }
        my_msg=my_msg.simplified();
    }

    QString msg() const{
        return my_msg;
    }
    bool has_type(type e) const{
        return my_type & e;
    }
    void add_type(type e){
        my_type = type(my_type | e);
    }
    QString user() const{
        return my_user;
    }
    QString receiver() const{
        return my_receiver;
    }
    type get_type() const{
        return my_type;
    }

private:
    QString my_msg;
    type my_type;
    QString my_user;
    QString my_receiver;
};

#endif // USERMESSAGE_H
