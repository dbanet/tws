/****************************************************************************
** Meta object code from reading C++ file 'sound_handler.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sound_handler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sound_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sound_handler[] = {

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
      15,   14,   14,   14, 0x0a,
      40,   35,   14,   14, 0x0a,
      69,   35,   14,   14, 0x0a,
      97,   14,   14,   14, 0x0a,
     124,   14,   14,   14, 0x0a,
     146,   14,   14,   14, 0x0a,
     178,  171,   14,   14, 0x0a,
     220,   14,   14,   14, 0x0a,
     244,  171,   14,   14, 0x0a,
     285,  283,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sound_handler[] = {
    "sound_handler\0\0play_startupsound()\0"
    "user\0play_normalmsgsound(QString)\0"
    "play_buddymsgsound(QString)\0"
    "play_chatwindowopensound()\0"
    "play_buddyleftsound()\0play_buddyarrivedsound()\0"
    "user,w\0play_highlightningsound(QString,QWidget*)\0"
    "play_buddyhostedsound()\0"
    "play_costumwordsound(QString,QWidget*)\0"
    "i\0volumechange(int)\0"
};

const QMetaObject sound_handler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_sound_handler,
      qt_meta_data_sound_handler, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sound_handler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sound_handler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sound_handler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sound_handler))
        return static_cast<void*>(const_cast< sound_handler*>(this));
    return QObject::qt_metacast(_clname);
}

int sound_handler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: play_startupsound(); break;
        case 1: play_normalmsgsound((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: play_buddymsgsound((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: play_chatwindowopensound(); break;
        case 4: play_buddyleftsound(); break;
        case 5: play_buddyarrivedsound(); break;
        case 6: play_highlightningsound((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 7: play_buddyhostedsound(); break;
        case 8: play_costumwordsound((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 9: volumechange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
