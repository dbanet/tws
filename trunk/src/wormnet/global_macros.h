#ifndef GLOBAL_MACROS_H
#define GLOBAL_MACROS_H
#include "singleton.h"
//  #include"global_macros.h"
enum CLASSES{e_leagueserverhandler,e_ircnet};
enum type{e_PRIVMSG=1, e_NOTICE=2, e_CTCP=4, e_ACTION=8, e_RAWCOMMAND=16};

enum hashtypes{e_nick=0, e_myself, e_garbage, e_time, e_garbagejoin, e_garbagepart
               ,e_garbagequit, e_chat, e_action, e_buddy, e_debug, e_ctcp, e_raw
               ,e_prv, e_notice, e_noticeaction, e_wa, e_http};

#define DECLARE_SINGLETON(CLASS)        \
private:                                \
    explicit CLASS();                   \
    ~CLASS();                           \
    explicit CLASS(const CLASS&);       \
public:                                 \
    friend CLASS &singleton<CLASS>();   \
private:

#endif // GLOBAL_MACROS_H
