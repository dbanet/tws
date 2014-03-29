#include"myDebug.h"
#include"mydebugclass.h"

myDebugClass &myDebug(){
    static myDebugClass d;
    return d;
}
