#include"myDebug.h"

myDebugClass &myDebug(){
    static myDebugClass d;
    return d;
}
