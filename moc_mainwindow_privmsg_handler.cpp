/****************************************************************************
** Meta object code from reading C++ file 'mainwindow_privmsg_handler.h'
**
** Created: Wed Dec 23 23:20:38 2009
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow_privmsg_handler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow_privmsg_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mainwindow_privmsg_handler[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      46,   28,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_mainwindow_privmsg_handler[] = {
    "mainwindow_privmsg_handler\0\0"
    "user,receiver,msg\0gotprvmsg(QString,QString,QString)\0"
};

const QMetaObject mainwindow_privmsg_handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mainwindow_privmsg_handler,
      qt_meta_data_mainwindow_privmsg_handler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mainwindow_privmsg_handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mainwindow_privmsg_handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mainwindow_privmsg_handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainwindow_privmsg_handler))
        return static_cast<void*>(const_cast< mainwindow_privmsg_handler*>(this));
    return QObject::qt_metacast(_clname);
}

int mainwindow_privmsg_handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: gotprvmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
