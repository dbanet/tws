/****************************************************************************
** Meta object code from reading C++ file 'balloon_handler.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "balloon_handler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'balloon_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_balloon_handler[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      32,   16,   16,   16, 0x0a,
      44,   16,   16,   16, 0x0a,
      59,   16,   16,   16, 0x0a,
      80,   71,   16,   16, 0x0a,
     129,  109,   16,   16, 0x0a,
     165,  155,   16,   16, 0x0a,
     198,   16,   16,   16, 0x0a,
     210,   16,   16,   16, 0x0a,
     233,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_balloon_handler[] = {
    "balloon_handler\0\0disconnected()\0"
    "connected()\0buddyarrived()\0buddyleft()\0"
    "user,msg\0got_privmsg(QString,QString)\0"
    "playername,gamename\0got_game(QString,QString)\0"
    "word,user\0got_costum_word(QString,QString)\0"
    "hide_tray()\0set_normal_tray_icon()\0"
    "set_away_tray_icon()\0"
};

const QMetaObject balloon_handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_balloon_handler,
      qt_meta_data_balloon_handler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &balloon_handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *balloon_handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *balloon_handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_balloon_handler))
        return static_cast<void*>(const_cast< balloon_handler*>(this));
    return QObject::qt_metacast(_clname);
}

int balloon_handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: disconnected(); break;
        case 1: connected(); break;
        case 2: buddyarrived(); break;
        case 3: buddyleft(); break;
        case 4: got_privmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: got_game((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: got_costum_word((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: hide_tray(); break;
        case 8: set_normal_tray_icon(); break;
        case 9: set_away_tray_icon(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
