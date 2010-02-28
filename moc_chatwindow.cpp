/****************************************************************************
** Meta object code from reading C++ file 'chatwindow.h'
**
** Created: Sun Feb 14 00:16:40 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chatwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_chatwindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      31,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   40,   11,   11, 0x08,
      59,   11,   11,   11, 0x28,
      69,   11,   11,   11, 0x08,
      82,   11,   11,   11, 0x08,
     103,  101,   11,   11, 0x08,
     138,   40,   11,   11, 0x08,
     165,   11,   11,   11, 0x28,
     185,   11,   11,   11, 0x08,
     201,   11,   11,   11, 0x08,
     218,   11,   11,   11, 0x08,
     233,   11,   11,   11, 0x08,
     249,   11,   11,   11, 0x08,
     267,   11,   11,   11, 0x08,
     297,   11,   11,   11, 0x08,
     315,   11,   11,   11, 0x08,
     331,   11,   11,   11, 0x08,
     349,  101,   11,   11, 0x08,
     374,   11,   11,   11, 0x08,
     400,  397,   11,   11, 0x08,
     436,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_chatwindow[] = {
    "chatwindow\0\0sigalert(QWidget*)\0closed()\0"
    "s\0sendmsg(QString)\0sendmsg()\0sendnotice()\0"
    "sendnoticeaction()\0,\0"
    "garbagemapchanged(QString,QString)\0"
    "usesettingswindow(QString)\0"
    "usesettingswindow()\0pbmuteclicked()\0"
    "pbbuddyclicked()\0pblogclicked()\0"
    "pbidleclicked()\0pbuploadclicked()\0"
    "pbpbsendfilehistorieclicked()\0"
    "pbpbfilehistory()\0pbctcpclicked()\0"
    "pbstatusclicked()\0gotidletime(QString,int)\0"
    "gotnosuchnick(QString)\0,,\0"
    "channelmsg(QString,QString,QString)\0"
    "filtergarbage()\0"
};

const QMetaObject chatwindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_chatwindow,
      qt_meta_data_chatwindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &chatwindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *chatwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *chatwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_chatwindow))
        return static_cast<void*>(const_cast< chatwindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int chatwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigalert((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: closed(); break;
        case 2: sendmsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: sendmsg(); break;
        case 4: sendnotice(); break;
        case 5: sendnoticeaction(); break;
        case 6: garbagemapchanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: usesettingswindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: usesettingswindow(); break;
        case 9: pbmuteclicked(); break;
        case 10: pbbuddyclicked(); break;
        case 11: pblogclicked(); break;
        case 12: pbidleclicked(); break;
        case 13: pbuploadclicked(); break;
        case 14: pbpbsendfilehistorieclicked(); break;
        case 15: pbpbfilehistory(); break;
        case 16: pbctcpclicked(); break;
        case 17: pbstatusclicked(); break;
        case 18: gotidletime((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: gotnosuchnick((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: channelmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 21: filtergarbage(); break;
        default: ;
        }
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void chatwindow::sigalert(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void chatwindow::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
