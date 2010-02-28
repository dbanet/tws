/****************************************************************************
** Meta object code from reading C++ file 'qmainwindow.h'
**
** Created: Sun Feb 14 00:16:42 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qfileupload/qmainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      28,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x0a,
      58,   11,   11,   11, 0x0a,
      72,   11,   11,   11, 0x0a,
      95,   89,   11,   11, 0x0a,
     137,  121,   11,   11, 0x0a,
     163,   11,   11,   11, 0x0a,
     178,  173,   11,   11, 0x0a,
     195,   11,   11,   11, 0x0a,
     216,  205,   11,   11, 0x0a,
     251,  246,   11,   11, 0x0a,
     289,  282,   11,   11, 0x0a,
     318,   11,   11,   11, 0x0a,
     332,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0resetProgress()\0saveSettings()\0"
    "loadSettings()\0startUpload()\0"
    "uploadFinished()\0error\0httpRequestFinished(bool)\0"
    "requestId,error\0commandFinished(int,bool)\0"
    "addFile()\0path\0addFile(QString)\0"
    "delFile()\0done,total\0updateProgress(qint64,qint64)\0"
    "item\0changeWidget(QListWidgetItem*)\0"
    "dll,ll\0saveVerlauf(QString,QString)\0"
    "loadVerlauf()\0delVerlauf()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    if (!strcmp(_clname, "Ui::MainWindow"))
        return static_cast< Ui::MainWindow*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resetProgress(); break;
        case 1: saveSettings(); break;
        case 2: loadSettings(); break;
        case 3: startUpload(); break;
        case 4: uploadFinished(); break;
        case 5: httpRequestFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: commandFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: addFile(); break;
        case 8: addFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: delFile(); break;
        case 10: updateProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 11: changeWidget((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 12: saveVerlauf((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 13: loadVerlauf(); break;
        case 14: delVerlauf(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
