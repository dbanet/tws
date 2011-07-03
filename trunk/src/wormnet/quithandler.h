#ifndef QUITHANDLER_H
#define QUITHANDLER_H

#include<QObject>
#include"global_macros.h"
class quithandler : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(quithandler);
public:    
    void inducequit(QString = QString());    
signals:

public slots:
    void beforequit();

private slots:
    void quit();

};

#endif // QUITHANDLER_H
