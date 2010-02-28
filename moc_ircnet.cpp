/****************************************************************************
** Meta object code from reading C++ file 'ircnet.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ircnet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ircnet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ircnet[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    8,    7,    7, 0x05,
      43,    8,    7,    7, 0x05,
      78,    7,    7,    7, 0x05,
     102,    7,    7,    7, 0x05,
     117,    7,    7,    7, 0x05,
     137,  135,    7,    7, 0x05,
     169,  135,    7,    7, 0x05,
     205,  135,    7,    7, 0x05,
     241,  135,    7,    7, 0x05,
     277,  135,    7,    7, 0x05,
     305,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     328,    7,    7,    7, 0x0a,
     336,    7,    7,    7, 0x0a,
     348,    7,    7,    7, 0x0a,
     363,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ircnet[] = {
    "ircnet\0\0,,\0sigmsg(QString,QString,QString)\0"
    "signotice(QString,QString,QString)\0"
    "siggetlist(QStringList)\0sigconnected()\0"
    "sigdisconnected()\0,\0sigusergarbage(QString,QString)\0"
    "sigusergarbagejoin(QString,QString)\0"
    "sigusergarbagepart(QString,QString)\0"
    "sigusergarbagequit(QString,QString)\0"
    "siggotidletime(QString,int)\0"
    "signosuchnick(QString)\0start()\0"
    "connected()\0disconnected()\0tcpread()\0"
};

const QMetaObject ircnet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ircnet,
      qt_meta_data_ircnet, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ircnet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ircnet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ircnet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ircnet))
        return static_cast<void*>(const_cast< ircnet*>(this));
    return QObject::qt_metacast(_clname);
}

int ircnet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: signotice((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: siggetlist((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: sigconnected(); break;
        case 4: sigdisconnected(); break;
        case 5: sigusergarbage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: sigusergarbagejoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: sigusergarbagepart((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: sigusergarbagequit((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: siggotidletime((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: signosuchnick((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: start(); break;
        case 12: connected(); break;
        case 13: disconnected(); break;
        case 14: tcpread(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ircnet::sigmsg(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ircnet::signotice(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ircnet::siggetlist(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ircnet::sigconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void ircnet::sigdisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void ircnet::sigusergarbage(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ircnet::sigusergarbagejoin(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ircnet::sigusergarbagepart(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ircnet::sigusergarbagequit(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ircnet::siggotidletime(const QString & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ircnet::signosuchnick(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
