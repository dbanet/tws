/****************************************************************************
** Meta object code from reading C++ file 'combobox_wrapper.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "combobox_wrapper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'combobox_wrapper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_combobox_wrapper[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      36,   17,   17,   17, 0x08,
      52,   17,   17,   17, 0x08,
      71,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_combobox_wrapper[] = {
    "combobox_wrapper\0\0on_down_clicked()\0"
    "on_up_clicked()\0on_minus_clicked()\0"
    "on_plus_2_clicked()\0"
};

const QMetaObject combobox_wrapper::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_combobox_wrapper,
      qt_meta_data_combobox_wrapper, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &combobox_wrapper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *combobox_wrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *combobox_wrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_combobox_wrapper))
        return static_cast<void*>(const_cast< combobox_wrapper*>(this));
    return QWidget::qt_metacast(_clname);
}

int combobox_wrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_down_clicked(); break;
        case 1: on_up_clicked(); break;
        case 2: on_minus_clicked(); break;
        case 3: on_plus_2_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
