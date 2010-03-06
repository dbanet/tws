#ifndef GLOBAL_MACROS_H
#define GLOBAL_MACROS_H
#include "singleton.h"

//  #include"global_macros.h"

#define DECLARE_SINGLETON(CLASS)    \
private:                            \
    CLASS();                        \
    ~CLASS();                       \
    CLASS(const CLASS&);            \
public:                             \
    friend CLASS &singleton<CLASS>();\
private:


#endif // GLOBAL_MACROS_H
