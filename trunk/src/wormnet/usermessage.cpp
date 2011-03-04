#include"usermessage.h"
#include"netcoupler.h"
usermessage::usermessage(QString msg_arg, int t_arg, QString receiver_arg):
        my_msg(msg_arg), my_type(t_arg), my_user(singleton<netcoupler>().nick), my_receiver(receiver_arg){

    Q_ASSERT_X(my_type & e_NOTICE || my_type & e_PRIVMSG || my_type & e_CTCP || my_type & e_RAWCOMMAND || my_type & e_GARBAGE, Q_FUNC_INFO, qPrintable(QString::number(my_type)));
    if(my_msg.startsWith("\001ACTION",Qt::CaseInsensitive)){
        add_type(e_ACTION);
        my_msg.remove("\001ACTION",Qt::CaseInsensitive).remove('\001');
    } else if(my_msg.startsWith('\001')){
        add_type(e_CTCP);
        my_msg.remove('\001');
    }
    my_msg=my_msg.simplified();
}
usermessage usermessage::create(QString msg, QString chatpartner){
    return create(msg, singleton<netcoupler>().nick, chatpartner);
}
