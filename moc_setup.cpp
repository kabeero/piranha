/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "setup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UIsetup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      16,    8,    8,    8, 0x08,
      25,    8,    8,    8, 0x08,
      43,    8,    8,    8, 0x08,
      57,    8,    8,    8, 0x08,
      70,    8,    8,    8, 0x08,
      83,    8,    8,    8, 0x08,
      96,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UIsetup[] = {
    "UIsetup\0\0scan()\0module()\0programPlatform()\0"
    "programJTAG()\0programUSB()\0procStdErr()\0"
    "procStdOut()\0deviceChanged(QString)\0"
};

void UIsetup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UIsetup *_t = static_cast<UIsetup *>(_o);
        switch (_id) {
        case 0: _t->scan(); break;
        case 1: _t->module(); break;
        case 2: _t->programPlatform(); break;
        case 3: _t->programJTAG(); break;
        case 4: _t->programUSB(); break;
        case 5: _t->procStdErr(); break;
        case 6: _t->procStdOut(); break;
        case 7: _t->deviceChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UIsetup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UIsetup::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_UIsetup,
      qt_meta_data_UIsetup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UIsetup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UIsetup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UIsetup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIsetup))
        return static_cast<void*>(const_cast< UIsetup*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int UIsetup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
