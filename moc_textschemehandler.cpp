/****************************************************************************
** Meta object code from reading C++ file 'textschemehandler.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "textschemehandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textschemehandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_textschemehandler[] = {

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
      19,   18,   18,   18, 0x08,
      34,   18,   18,   18, 0x08,
      48,   18,   18,   18, 0x08,
      63,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_textschemehandler[] = {
    "textschemehandler\0\0closeclicked()\0"
    "fontclicked()\0colorclicked()\0showclicked()\0"
};

const QMetaObject textschemehandler::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_textschemehandler,
      qt_meta_data_textschemehandler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &textschemehandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *textschemehandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *textschemehandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_textschemehandler))
        return static_cast<void*>(const_cast< textschemehandler*>(this));
    return QWidget::qt_metacast(_clname);
}

int textschemehandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeclicked(); break;
        case 1: fontclicked(); break;
        case 2: colorclicked(); break;
        case 3: showclicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
