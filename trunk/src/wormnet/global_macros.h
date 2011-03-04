#ifndef GLOBAL_MACROS_H
#define GLOBAL_MACROS_H
#include "singleton.h"
//  #include"global_macros.h"
enum CLASSES{e_leagueserverhandler,e_ircnet};
enum usermessage_type{e_PRIVMSG=1, e_NOTICE=2, e_CTCP=4, e_ACTION=8, e_RAWCOMMAND=16
    , e_GARBAGE=32, e_GARBAGEJOIN=64, e_GARBAGEPART=128, e_GARBAGEQUIT=256
    , e_CHANNELMSGTOCHAT=512, e_CHATMSGTOCHANNEL=1024};

enum hashtypes{e_hash_nick=0, e_hash_myself, e_hash_garbage, e_hash_time, e_hash_garbagejoin, e_hash_garbagepart
                           ,e_hash_garbagequit, e_hash_chat, e_hash_action, e_hash_buddy, e_hash_debug, e_hash_ctcp, e_hash_raw
                           ,e_hash_prv, e_hash_notice, e_hash_noticeaction, e_hash_wa, e_hash_http};

#define DECLARE_SINGLETON(CLASS)                \
private:                                        \
            explicit CLASS();                   \
            virtual ~CLASS();                           \
            explicit CLASS(const CLASS&);       \
        public:                                 \
            friend CLASS &singleton<CLASS>();   \
        private:

#endif // GLOBAL_MACROS_H
