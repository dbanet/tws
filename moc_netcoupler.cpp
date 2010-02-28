/****************************************************************************
** Meta object code from reading C++ file 'netcoupler.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "netcoupler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'netcoupler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_netcoupler[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      48,   45,   11,   11, 0x05,
      83,   45,   11,   11, 0x05,
     121,   45,   11,   11, 0x05,
     160,   11,   11,   11, 0x05,
     192,  190,   11,   11, 0x05,
     224,  190,   11,   11, 0x05,
     260,  190,   11,   11, 0x05,
     296,  190,   11,   11, 0x05,
     332,   11,   11,   11, 0x05,
     355,   11,   11,   11, 0x05,
     382,  190,   11,   11, 0x05,
     410,   11,   11,   11, 0x05,
     433,   11,   11,   11, 0x05,
     448,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     466,   11,   11,   11, 0x0a,
     488,   45,   11,   11, 0x0a,
     522,   45,   11,   11, 0x0a,
     557,   11,   11,   11, 0x0a,
     575,   11,   11,   11, 0x08,
     587,  585,   11,   11, 0x08,
     614,   11,   11,   11, 0x28,
     634,  190,   11,   11, 0x08,
     666,   11,   11,   11, 0x08,
     694,   11,   11,   11, 0x08,
     712,  190,   11,   11, 0x08,
     751,   45,   11,   11, 0x08,
     783,   11,   11,   11, 0x08,
     796,   11,   11,   11, 0x08,
     807,  190,   11,   11, 0x08,
     834,  190,   11,   11, 0x08,
     883,   11,   11,   11, 0x08,
     898,   11,   11,   11, 0x08,
     930,  190,   11,   11, 0x08,
     972,   11,   11,   11, 0x08,
     986,   11,   11,   11, 0x08,
    1002,   11,   11,   11, 0x08,
    1017,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_netcoupler[] = {
    "netcoupler\0\0signewwholist(QList<userstruct>)\0"
    ",,\0siggotmsg(QString,QString,QString)\0"
    "siggotnotice(QString,QString,QString)\0"
    "siggotprivmsg(QString,QString,QString)\0"
    "siggotchanellist(QStringList)\0,\0"
    "sigusergarbage(QString,QString)\0"
    "sigusergarbagejoin(QString,QString)\0"
    "sigusergarbagepart(QString,QString)\0"
    "sigusergarbagequit(QString,QString)\0"
    "sigawaystringchanged()\0"
    "sigsettingswindowchanged()\0"
    "siggotidletime(QString,int)\0"
    "signosuchnick(QString)\0sigconnected()\0"
    "sigdisconnected()\0joingamelink(QString)\0"
    "joingame(QString,QString,QString)\0"
    "getnotice(QString,QString,QString)\0"
    "refreshhostlist()\0setaway()\0s\0"
    "usesettingswindow(QString)\0"
    "usesettingswindow()\0sendinfotochan(QString,QString)\0"
    "getchannellist(QStringList)\0"
    "getircip(QString)\0"
    "gethostlist(QList<hoststruct>,QString)\0"
    "getmsg(QString,QString,QString)\0"
    "getwholist()\0sendquit()\0"
    "getscheme(QString,QString)\0"
    "gethostlistforhosting(QList<hoststruct>,QString)\0"
    "hosttimetout()\0sendmessagetoallbuddys(QString)\0"
    "processfinished(int,QProcess::ExitStatus)\0"
    "readprocess()\0hostwontstart()\0"
    "ircconnected()\0ircdiconnected()\0"
};

const QMetaObject netcoupler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_netcoupler,
      qt_meta_data_netcoupler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &netcoupler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *netcoupler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *netcoupler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_netcoupler))
        return static_cast<void*>(const_cast< netcoupler*>(this));
    return QObject::qt_metacast(_clname);
}

int netcoupler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signewwholist((*reinterpret_cast< QList<userstruct>(*)>(_a[1]))); break;
        case 1: siggotmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: siggotnotice((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: siggotprivmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: siggotchanellist((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: sigusergarbage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: sigusergarbagejoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: sigusergarbagepart((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: sigusergarbagequit((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: sigawaystringchanged(); break;
        case 10: sigsettingswindowchanged(); break;
        case 11: siggotidletime((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: signosuchnick((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: sigconnected(); break;
        case 14: sigdisconnected(); break;
        case 15: joingamelink((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: joingame((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 17: getnotice((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 18: refreshhostlist(); break;
        case 19: setaway(); break;
        case 20: usesettingswindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: usesettingswindow(); break;
        case 22: sendinfotochan((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 23: getchannellist((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 24: getircip((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: gethostlist((*reinterpret_cast< QList<hoststruct>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 26: getmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 27: getwholist(); break;
        case 28: sendquit(); break;
        case 29: getscheme((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 30: gethostlistforhosting((*reinterpret_cast< QList<hoststruct>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 31: hosttimetout(); break;
        case 32: sendmessagetoallbuddys((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: processfinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 34: readprocess(); break;
        case 35: hostwontstart(); break;
        case 36: ircconnected(); break;
        case 37: ircdiconnected(); break;
        default: ;
        }
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void netcoupler::signewwholist(QList<userstruct> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void netcoupler::siggotmsg(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void netcoupler::siggotnotice(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void netcoupler::siggotprivmsg(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void netcoupler::siggotchanellist(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void netcoupler::sigusergarbage(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void netcoupler::sigusergarbagejoin(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void netcoupler::sigusergarbagepart(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void netcoupler::sigusergarbagequit(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void netcoupler::sigawaystringchanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void netcoupler::sigsettingswindowchanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void netcoupler::siggotidletime(const QString & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void netcoupler::signosuchnick(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void netcoupler::sigconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void netcoupler::sigdisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}
QT_END_MOC_NAMESPACE
