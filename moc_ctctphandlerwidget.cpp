/****************************************************************************
** Meta object code from reading C++ file 'ctctphandlerwidget.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ctctphandlerwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ctctphandlerwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ctctphandlerwidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      35,   19,   19,   19, 0x08,
      54,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ctctphandlerwidget[] = {
    "ctctphandlerwidget\0\0safeandclose()\0"
    "addcommandbyhand()\0pbhowtoclicked()\0"
};

const QMetaObject ctctphandlerwidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ctctphandlerwidget,
      qt_meta_data_ctctphandlerwidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ctctphandlerwidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ctctphandlerwidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ctctphandlerwidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ctctphandlerwidget))
        return static_cast<void*>(const_cast< ctctphandlerwidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int ctctphandlerwidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: safeandclose(); break;
        case 1: addcommandbyhand(); break;
        case 2: pbhowtoclicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
