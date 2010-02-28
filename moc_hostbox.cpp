/****************************************************************************
** Meta object code from reading C++ file 'hostbox.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hostbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hostbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_hostbox[] = {

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
      44,    8,    8,    8, 0x08,
      57,    8,    8,    8, 0x08,
      69,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_hostbox[] = {
    "hostbox\0\0sigok()\0on_pbshowdetails_clicked()\0"
    "addclicked()\0okclicked()\0cancelclicked()\0"
};

const QMetaObject hostbox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_hostbox,
      qt_meta_data_hostbox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &hostbox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *hostbox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *hostbox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_hostbox))
        return static_cast<void*>(const_cast< hostbox*>(this));
    return QWidget::qt_metacast(_clname);
}

int hostbox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigok(); break;
        case 1: on_pbshowdetails_clicked(); break;
        case 2: addclicked(); break;
        case 3: okclicked(); break;
        case 4: cancelclicked(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void hostbox::sigok()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
