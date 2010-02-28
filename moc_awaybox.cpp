/****************************************************************************
** Meta object code from reading C++ file 'awaybox.h'
**
** Created: Sun Feb 14 00:16:40 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "awaybox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'awaybox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_awaybox[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      17,    8,    8,    8, 0x08,
      29,    8,    8,    8, 0x08,
      45,    8,    8,    8, 0x08,
      59,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_awaybox[] = {
    "awaybox\0\0sigok()\0okclicked()\0"
    "cancelclicked()\0backclicked()\0"
    "forwardclicked()\0"
};

const QMetaObject awaybox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_awaybox,
      qt_meta_data_awaybox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &awaybox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *awaybox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *awaybox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_awaybox))
        return static_cast<void*>(const_cast< awaybox*>(this));
    return QWidget::qt_metacast(_clname);
}

int awaybox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigok(); break;
        case 1: okclicked(); break;
        case 2: cancelclicked(); break;
        case 3: backclicked(); break;
        case 4: forwardclicked(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void awaybox::sigok()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
