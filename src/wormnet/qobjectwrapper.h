#ifndef QOBJECTWRAPPER_H
#define QOBJECTWRAPPER_H
#include<stdexcept>
template <typename T>
        class qobjectwrapper
{
public:
    qobjectwrapper();
    ~qobjectwrapper();
    static T &ref();
    static bool isNull();
    static void del(){
        t->deleteLater();
    }
private:
    static T *t;
    static int refcounter;
};
template <typename T> T *qobjectwrapper<T>::t=NULL;
template <typename T> int qobjectwrapper<T>::refcounter=0;

template <typename T>
        qobjectwrapper<T>::qobjectwrapper(){
    refcounter++;
    if(refcounter>1)
        throw std::runtime_error(qPrintable("in qobjectwrapper.. 2 instances of "+t->objectName()));
    t=new T;
}
template <typename T>
        qobjectwrapper<T>::~qobjectwrapper(){
}
template <typename T>
        bool qobjectwrapper<T>::isNull(){
    return t==NULL;
}
template <typename T>
        T &qobjectwrapper<T>::ref(){
    if(t==NULL)
        throw std::runtime_error(qPrintable("in qobjectwrapper<"+T::toString()+">::ref requests a nullptr!"));
    return *t;
}
#endif // QOBJECTWRAPPER_H
