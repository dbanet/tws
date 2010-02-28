/****************************************************************************
** Meta object code from reading C++ file 'logbrowser.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "logbrowser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logbrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_logbrowser[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   11,   11,   11, 0x08,
      54,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_logbrowser[] = {
    "logbrowser\0\0sigopenchatwindow(QString)\0"
    "closeclicked()\0chatclicked()\0"
};

const QMetaObject logbrowser::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_logbrowser,
      qt_meta_data_logbrowser, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &logbrowser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *logbrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *logbrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_logbrowser))
        return static_cast<void*>(const_cast< logbrowser*>(this));
    return QWidget::qt_metacast(_clname);
}

int logbrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigopenchatwindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: closeclicked(); break;
        case 2: chatclicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void logbrowser::sigopenchatwindow(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
