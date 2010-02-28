/****************************************************************************
** Meta object code from reading C++ file 'uploadedfileatomic.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "uploadedfileatomic.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uploadedfileatomic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_uploadedfileatomic[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      34,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_uploadedfileatomic[] = {
    "uploadedfileatomic\0\0copyclicked()\0"
    "delclicked()\0"
};

const QMetaObject uploadedfileatomic::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_uploadedfileatomic,
      qt_meta_data_uploadedfileatomic, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &uploadedfileatomic::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *uploadedfileatomic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *uploadedfileatomic::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_uploadedfileatomic))
        return static_cast<void*>(const_cast< uploadedfileatomic*>(this));
    return QWidget::qt_metacast(_clname);
}

int uploadedfileatomic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: copyclicked(); break;
        case 1: delclicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
