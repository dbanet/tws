/****************************************************************************
** Meta object code from reading C++ file 'snoppanet.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "snoppanet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'snoppanet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_snoppanet[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x05,
      52,   10,   10,   10, 0x05,
      70,   11,   10,   10, 0x05,
     104,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     123,   10,   10,   10, 0x0a,
     141,   10,   10,   10, 0x08,
     153,   10,   10,   10, 0x08,
     192,   10,   10,   10, 0x08,
     206,   10,   10,   10, 0x08,
     224,   10,   10,   10, 0x08,
     236,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_snoppanet[] = {
    "snoppanet\0\0,\0sighostlist(QList<hoststruct>,QString)\0"
    "sigircip(QString)\0sigchannelscheme(QString,QString)\0"
    "sighostwontstart()\0refreshhostlist()\0"
    "readircip()\0httpError(QNetworkReply::NetworkError)\0"
    "hosttimeout()\0readgamelist(int)\0"
    "getscheme()\0readhostreply()\0"
};

const QMetaObject snoppanet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_snoppanet,
      qt_meta_data_snoppanet, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &snoppanet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *snoppanet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *snoppanet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_snoppanet))
        return static_cast<void*>(const_cast< snoppanet*>(this));
    return QObject::qt_metacast(_clname);
}

int snoppanet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sighostlist((*reinterpret_cast< QList<hoststruct>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: sigircip((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: sigchannelscheme((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: sighostwontstart(); break;
        case 4: refreshhostlist(); break;
        case 5: readircip(); break;
        case 6: httpError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 7: hosttimeout(); break;
        case 8: readgamelist((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: getscheme(); break;
        case 10: readhostreply(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void snoppanet::sighostlist(QList<hoststruct> _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void snoppanet::sigircip(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void snoppanet::sigchannelscheme(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void snoppanet::sighostwontstart()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
