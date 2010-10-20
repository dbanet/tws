#ifndef GLOBAL_MACROS_H
#define GLOBAL_MACROS_H
#include "singleton.h"
//  #include"global_macros.h"
enum CLASSES{
    e_leagueserverhandler,e_ircnet
};

#define DECLARE_SINGLETON(CLASS)        \
private:                                \
    explicit CLASS();                   \
    ~CLASS();                           \
    explicit CLASS(const CLASS&);       \
public:                                 \
    friend CLASS &singleton<CLASS>();   \
private:


#endif // GLOBAL_MACROS_H
