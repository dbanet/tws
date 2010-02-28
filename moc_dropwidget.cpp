/****************************************************************************
** Meta object code from reading C++ file 'dropwidget.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qfileupload/dropwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dropwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DropWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      37,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   11,   11,   11, 0x0a,
      64,   11,   11,   11, 0x0a,
      82,   11,   11,   11, 0x0a,
     102,   11,   11,   11, 0x0a,
     120,   11,   11,   11, 0x0a,
     151,  140,   11,   11, 0x0a,
     184,  178,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DropWidget[] = {
    "DropWidget\0\0path\0fileDroped(QString)\0"
    "wantsUpload()\0emitUpload()\0slotAnimateShow()\0"
    "playAnimationShow()\0slotAnimateSync()\0"
    "playAnimationSync()\0done,total\0"
    "setProgress(qint64,qint64)\0files\0"
    "setFilesToUpload(int)\0"
};

const QMetaObject DropWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DropWidget,
      qt_meta_data_DropWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DropWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DropWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DropWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DropWidget))
        return static_cast<void*>(const_cast< DropWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DropWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fileDroped((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: wantsUpload(); break;
        case 2: emitUpload(); break;
        case 3: slotAnimateShow(); break;
        case 4: playAnimationShow(); break;
        case 5: slotAnimateSync(); break;
        case 6: playAnimationSync(); break;
        case 7: setProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 8: setFilesToUpload((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DropWidget::fileDroped(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DropWidget::wantsUpload()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
