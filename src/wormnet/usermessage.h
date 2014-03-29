#ifndef USERMESSAGE_H
#define USERMESSAGE_H
#include<QString>

#include"global_macros.h"

class usermessage{
public:
    usermessage(QString msg_arg, usermessage_type t_arg, QString user_arg, QString receiver_arg):
            my_msg(msg_arg), my_type(t_arg), my_user(user_arg), my_receiver(receiver_arg){

        Q_ASSERT_X(my_type & e_NOTICE || my_type & e_PRIVMSG || my_type & e_CTCP || my_type & e_RAWCOMMAND || my_type & e_GARBAGE || my_type & e_SCRIPTCOMMAND, Q_FUNC_INFO, qPrintable(QString::number(my_type)));
        if(my_msg.startsWith("\001ACTION",Qt::CaseInsensitive)){
            add_type(e_ACTION);
            my_msg.remove("\001ACTION",Qt::CaseInsensitive).remove('\001');
        } else if(my_msg.startsWith('\001')){
            add_type(e_CTCP);
            my_msg.remove('\001');
        }
        my_msg=my_msg.simplified();
    }
    usermessage(QString msg_arg, int t_arg, QString receiver_arg);
    usermessage(){}

    QString msg() const{
        return my_msg;
    }
    bool has_type(usermessage_type e) const{
        return my_type & e;
    }
    usermessage add_type(usermessage_type e){
        my_type = usermessage_type(my_type | e);
        return *this;
    }
    usermessage merge_types(usermessage u){
        my_type |= u.my_type;
        return *this;
    }
    QString user() const{
        return my_user;
    }
    QString receiver() const{
        return my_receiver;
    }
    int get_type() const{
        return my_type;
    }    
    static usermessage create(QString msg, QString me, QString chatpartner){
        if(msg.startsWith("$")){
            msg.remove(0, 1);
            usermessage u(msg, e_SCRIPTCOMMAND, me, chatpartner);
            return u;
        } else if (msg.startsWith(">!")) {
            msg.remove(0, 2);
            usermessage u(msg, e_CTCP, me, chatpartner);
            return u;
        } else if (msg.startsWith("/")) {
            msg.remove(0, 1);
            usermessage u(msg, e_RAWCOMMAND, me, chatpartner);
            return u;
        } else if (msg.startsWith(">>>")){
            msg.remove(0, 3);
            usermessage u(msg, e_NOTICE, me, chatpartner);
            u.add_type(e_ACTION);
            return u;
        } else if (msg.startsWith(">>")){
            msg.remove(0, 2);
            usermessage u(msg, e_NOTICE, me, chatpartner);
            return u;
        } else if (msg.startsWith(">")) {
            msg.remove(0, 1);
            usermessage u(msg, e_PRIVMSG, me, chatpartner);
            u.add_type(e_ACTION);
            return u;
        }
        return usermessage(msg, e_PRIVMSG, me, chatpartner);;
    }
    static usermessage create(QString msg, QString chatpartner);    
    void settime(QString s){
        my_time=s;
    }
    QString time() const{
        return my_time;
    }
    int type() const{
        return my_type;
    }

protected:
private:
    QString my_msg;
    int my_type;
    QString my_user;
    QString my_receiver;
    QString my_time;

    friend QDataStream &operator>>(QDataStream &ds, usermessage &u);
};

#endif // USERMESSAGE_H
