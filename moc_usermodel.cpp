/****************************************************************************
** Meta object code from reading C++ file 'usermodel.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usermodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_usermodel[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x05,
      55,   10,   10,   10, 0x05,
      70,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   10,   10,   10, 0x08,
     103,   10,   10,   10, 0x08,
     115,   11,   10,   10, 0x0a,
     145,  143,   10,   10, 0x0a,
     172,   10,   10,   10, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_usermodel[] = {
    "usermodel\0\0,\0sigselectitem(QModelIndex,const QWidget*)\0"
    "sigbuddyleft()\0sigbuddyarrived()\0"
    "buddyarrived()\0buddyleft()\0"
    "sortslot(int,Qt::SortOrder)\0s\0"
    "usesettingswindow(QString)\0"
    "usesettingswindow()\0"
};

const QMetaObject usermodel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_usermodel,
      qt_meta_data_usermodel, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &usermodel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *usermodel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *usermodel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_usermodel))
        return static_cast<void*>(const_cast< usermodel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int usermodel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigselectitem((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QWidget*(*)>(_a[2]))); break;
        case 1: sigbuddyleft(); break;
        case 2: sigbuddyarrived(); break;
        case 3: buddyarrived(); break;
        case 4: buddyleft(); break;
        case 5: sortslot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 6: usesettingswindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: usesettingswindow(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void usermodel::sigselectitem(const QModelIndex & _t1, const QWidget * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void usermodel::sigbuddyleft()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void usermodel::sigbuddyarrived()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
