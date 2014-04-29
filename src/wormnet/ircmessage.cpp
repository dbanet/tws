#include "ircmessage.h"
#include <QString>

#define IRC_MSG_PREFIX_INDICATOR   ":"  // if msg starts with colon, the following is the msg prefix
#define IRC_MSG_PREFIX_SEPARATOR   " "  // the prefix doesn't contain spaces; the space indicates its end
#define IRC_MSG_TRAILING_SEPARATOR " :" // the trailing starts from the first occurance of " :" and lasts til the msgend

ircMessage::ircMessage(QString message){
    if(message.isEmpty()) throw new QString("The message was empty.");

    /* check if the message is issued by a user, e. g. ":uname!ident@uhost QUIT..." */
    if(message.startsWith(IRC_MSG_PREFIX_INDICATOR)){
        this->type=USER;
        this->prefix=message.split(IRC_MSG_PREFIX_SEPARATOR)[0].remove(0,QString(IRC_MSG_PREFIX_INDICATOR).length());
    }
    else this->type=SERVER;

    /* check if the message contains the trailing part, e. g. "... PRIVMSG :lorem ipsum dolor sit amet" */
    if(message.contains(" :")){
        this->hasTrailPart=true;
        this->trailing=message.section(" :",1);
    }
    else this->hasTrailPart=false;

    /* getting the command and params */
    QStringList cmdAndParam=
    message.mid(
        this->type==USER?   /* if there is a prefix, exclude it   */
            message.indexOf(IRC_MSG_PREFIX_SEPARATOR)+QString(IRC_MSG_PREFIX_SEPARATOR).length()
          :
            0,
        this->hasTrailPart? /* if there is a trailing, exclude it */
            message.indexOf(IRC_MSG_TRAILING_SEPARATOR)-1
          :
            -1
    ).split(' ');                 /* the words are separated by spaces */
    this->command=cmdAndParam[0]; /* the first word is the command,    */
    cmdAndParam.removeFirst();    /* and the remaining words           */
    this->paramList=cmdAndParam;  /* are the parameters of the command */
}
bool ircMessage::isServMsg(){
    return this->type==SERVER;
}
bool ircMessage::isUserMsg(){
    return this->type==USER;
}
bool ircMessage::hasTrailing(){
    return this->hasTrailPart;
}
