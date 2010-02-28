/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Feb 14 00:16:41 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mainwindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   11,   11,   11, 0x08,
      67,   11,   11,   11, 0x08,
      81,   11,   11,   11, 0x08,
      99,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     149,   11,   11,   11, 0x08,
     199,   11,   11,   11, 0x08,
     227,   11,   11,   11, 0x08,
     251,   11,   11,   11, 0x08,
     275,   11,   11,   11, 0x08,
     293,   11,   11,   11, 0x08,
     318,  316,   11,   11, 0x08,
     345,   11,   11,   11, 0x28,
     365,   11,   11,   11, 0x08,
     383,   11,   11,   11, 0x08,
     394,   11,   11,   11, 0x08,
     406,   11,   11,   11, 0x08,
     429,  427,   11,   11, 0x08,
     460,   11,   11,   11, 0x08,
     483,   11,   11,   11, 0x08,
     507,   11,   11,   11, 0x08,
     555,  537,   11,   11, 0x08,
     590,   11,   11,   11, 0x08,
     602,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mainwindow[] = {
    "mainwindow\0\0sigopenchatwindow(QString)\0"
    "getchannellist(QStringList)\0joinclicked()\0"
    "joinslot(QString)\0chooseclicked()\0"
    "onquit()\0returntotabsettings(int)\0"
    "trayactivation(QSystemTrayIcon::ActivationReason)\0"
    "traymenutriggered(QAction*)\0"
    "pbrememberjoinclicked()\0snpsetcontains(QString)\0"
    "pbeditorclicked()\0windowremoved(QString)\0"
    "s\0usesettingswindow(QString)\0"
    "usesettingswindow()\0chatwinowclosed()\0"
    "quitslot()\0awayboxok()\0awaymessagechanged()\0"
    ",\0gotctcpsignal(QString,QString)\0"
    "settextscheme(QString)\0openchatwindow(QString)\0"
    "openchatwindowhidden(QString)\0"
    "user,receiver,msg\0gotprvmsg(QString,QString,QString)\0"
    "connected()\0disconnected()\0"
};

const QMetaObject mainwindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mainwindow,
      qt_meta_data_mainwindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &mainwindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *mainwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *mainwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mainwindow))
        return static_cast<void*>(const_cast< mainwindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int mainwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigopenchatwindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: getchannellist((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: joinclicked(); break;
        case 3: joinslot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: chooseclicked(); break;
        case 5: onquit(); break;
        case 6: returntotabsettings((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: trayactivation((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 8: traymenutriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 9: pbrememberjoinclicked(); break;
        case 10: snpsetcontains((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: pbeditorclicked(); break;
        case 12: windowremoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: usesettingswindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: usesettingswindow(); break;
        case 15: chatwinowclosed(); break;
        case 16: quitslot(); break;
        case 17: awayboxok(); break;
        case 18: awaymessagechanged(); break;
        case 19: gotctcpsignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 20: settextscheme((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: openchatwindow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: openchatwindowhidden((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: gotprvmsg((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 24: connected(); break;
        case 25: disconnected(); break;
        default: ;
        }
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void mainwindow::sigopenchatwindow(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
