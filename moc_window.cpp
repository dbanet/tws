/****************************************************************************
** Meta object code from reading C++ file 'window.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_window[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      33,    7,    7,    7, 0x05,
      52,    7,    7,    7, 0x05,
      76,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,  103,    7,    7, 0x08,
     138,  103,    7,    7, 0x08,
     175,  173,    7,    7, 0x08,
     207,  173,    7,    7, 0x08,
     239,  173,    7,    7, 0x08,
     271,    7,    7,    7, 0x08,
     281,    7,    7,    7, 0x08,
     294,    7,    7,    7, 0x08,
     323,    7,    7,    7, 0x08,
     355,    7,    7,    7, 0x08,
     376,    7,    7,    7, 0x08,
     400,    7,    7,    7, 0x08,
     429,    7,    7,    7, 0x08,
     461,    7,    7,    7, 0x08,
     470,    7,    7,    7, 0x08,
     489,  173,    7,    7, 0x08,
     541,  173,    7,    7, 0x08,
     584,  582,    7,    7, 0x08,
     611,    7,    7,    7, 0x28,
     634,  631,    7,    7, 0x08,
     662,    7,    7,    7, 0x08,
     679,    7,    7,    7, 0x08,
     693,    7,    7,    7, 0x08,
     704,    7,    7,    7, 0x08,
     715,    7,    7,    7, 0x08,
     732,    7,    7,    7, 0x08,
     746,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_window[] = {
    "window\0\0sigwindowclosed(QString)\0"
    "sigalert(QWidget*)\0sigjoinchannel(QString)\0"
    "sigopenchatwindow(QString)\0,,\0"
    "gotmsg(QString,QString,QString)\0"
    "gotnotice(QString,QString,QString)\0,\0"
    "gotgarbagejoin(QString,QString)\0"
    "gotgarbagepart(QString,QString)\0"
    "gotgarbagequit(QString,QString)\0"
    "sendmsg()\0sendnotice()\0"
    "useritempressed(QModelIndex)\0"
    "useritemdblclicked(QModelIndex)\0"
    "getuserinfo(QString)\0openchatwindow(QString)\0"
    "hostitempressed(QModelIndex)\0"
    "hostitemdblclicked(QModelIndex)\0"
    "hboxok()\0hboxprvok(QString)\0"
    "userselectionchanged(QItemSelection,QItemSelection)\0"
    "setselection(QModelIndex,const QWidget*)\0"
    "s\0usesettingswindow(QString)\0"
    "usesettingswindow()\0sl\0"
    "expandchannels(QStringList)\0"
    "expandchannels()\0getjoinmenu()\0"
    "openhbox()\0minimize()\0changealpha(int)\0"
    "showbuttons()\0getuserscount(QStringList)\0"
};

const QMetaObject window::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_window,
      qt_meta_data_window, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_window))
        return static_cast<void*>(const_cast< window*>(this));
    return QWidget::qt_metacast(_clname);
}

int window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigwindowclosed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: sigalert((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 2: sigjoinchannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: sigopenchatwindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: gotmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: gotnotice((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: gotgarbagejoin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: gotgarbagepart((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: gotgarbagequit((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: sendmsg(); break;
        case 10: sendnotice(); break;
        case 11: useritempressed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 12: useritemdblclicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 13: getuserinfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: openchatwindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: hostitempressed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 16: hostitemdblclicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 17: hboxok(); break;
        case 18: hboxprvok((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: userselectionchanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 20: setselection((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QWidget*(*)>(_a[2]))); break;
        case 21: usesettingswindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: usesettingswindow(); break;
        case 23: expandchannels((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 24: expandchannels(); break;
        case 25: getjoinmenu(); break;
        case 26: openhbox(); break;
        case 27: minimize(); break;
        case 28: changealpha((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: showbuttons(); break;
        case 30: getuserscount((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void window::sigwindowclosed(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void window::sigalert(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void window::sigjoinchannel(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void window::sigopenchatwindow(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
