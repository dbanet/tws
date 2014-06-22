#ifndef SINGLETON_H
#define SINGLETON_H
//  #include "singleton.h"
template <typename T>
        T &singleton(){
    static T t;
    return t;
}
#endif // SINGLETON_H
