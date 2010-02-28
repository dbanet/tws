/****************************************************************************
** Meta object code from reading C++ file 'buttonlayout.h'
**
** Created: Sun Feb 14 00:16:40 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "buttonlayout.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buttonlayout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_buttonlayout[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      30,   13,   13,   13, 0x05,
      51,   13,   13,   13, 0x05,
      67,   13,   13,   13, 0x05,
      87,   13,   13,   13, 0x05,
      99,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     111,   13,   13,   13, 0x08,
     131,   13,   13,   13, 0x08,
     154,   13,   13,   13, 0x08,
     175,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_buttonlayout[] = {
    "buttonlayout\0\0pbhostclicked()\0"
    "pbminimizedclicked()\0pbhideclicked()\0"
    "sigchangealpha(int)\0sigshowme()\0"
    "sighideme()\0on_pbsort_clicked()\0"
    "on_pbballoon_clicked()\0on_pbsound_clicked()\0"
    "on_pbcostumwords_clicked()\0"
};

const QMetaObject buttonlayout::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_buttonlayout,
      qt_meta_data_buttonlayout, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &buttonlayout::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *buttonlayout::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *buttonlayout::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_buttonlayout))
        return static_cast<void*>(const_cast< buttonlayout*>(this));
    return QWidget::qt_metacast(_clname);
}

int buttonlayout::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pbhostclicked(); break;
        case 1: pbminimizedclicked(); break;
        case 2: pbhideclicked(); break;
        case 3: sigchangealpha((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: sigshowme(); break;
        case 5: sighideme(); break;
        case 6: on_pbsort_clicked(); break;
        case 7: on_pbballoon_clicked(); break;
        case 8: on_pbsound_clicked(); break;
        case 9: on_pbcostumwords_clicked(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void buttonlayout::pbhostclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void buttonlayout::pbminimizedclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void buttonlayout::pbhideclicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void buttonlayout::sigchangealpha(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void buttonlayout::sigshowme()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void buttonlayout::sighideme()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
