/****************************************************************************
** Meta object code from reading C++ file 'piranha.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "piranha.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'piranha.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UIpiranha[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   10,   10,   10, 0x08,
      41,   10,   10,   10, 0x08,
      56,   10,   10,   10, 0x08,
      70,   10,   10,   10, 0x08,
      77,   10,   10,   10, 0x08,
      85,   10,   10,   10, 0x08,
      93,   10,   10,   10, 0x08,
     101,   10,   10,   10, 0x08,
     108,   10,   10,   10, 0x08,
     116,   10,   10,   10, 0x08,
     140,  129,   10,   10, 0x08,
     169,  158,   10,   10, 0x08,
     198,  187,   10,   10, 0x08,
     229,  216,   10,   10, 0x08,
     249,   10,   10,   10, 0x08,
     266,   10,  262,   10, 0x08,
     296,   10,  289,   10, 0x08,
     318,  316,   10,   10, 0x08,
     351,   10,   10,   10, 0x08,
     383,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UIpiranha[] = {
    "UIpiranha\0\0abortCapture()\0export_image()\0"
    "record_start()\0record_stop()\0exit()\0"
    "setup()\0apply()\0start()\0stop()\0reset()\0"
    "initValues()\0imageIndex\0imageChanged(int)\0"
    "trackIndex\0trackChanged(int)\0flagsIndex\0"
    "flagsChanged(int)\0programIndex\0"
    "programChanged(int)\0printFrame()\0int\0"
    "acquireImage(cv::Mat*)\0QImage\0"
    "mat2qimage(cv::Mat)\0,\0"
    "trackDataPlot(cv::Mat*,cv::Mat*)\0"
    "trackDataPlotEmbedded(cv::Mat*)\0"
    "trackDataPlotTrajectory(cv::Mat*)\0"
};

void UIpiranha::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UIpiranha *_t = static_cast<UIpiranha *>(_o);
        switch (_id) {
        case 0: _t->abortCapture(); break;
        case 1: _t->export_image(); break;
        case 2: _t->record_start(); break;
        case 3: _t->record_stop(); break;
        case 4: _t->exit(); break;
        case 5: _t->setup(); break;
        case 6: _t->apply(); break;
        case 7: _t->start(); break;
        case 8: _t->stop(); break;
        case 9: _t->reset(); break;
        case 10: _t->initValues(); break;
        case 11: _t->imageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->trackChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->flagsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->programChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->printFrame(); break;
        case 16: { int _r = _t->acquireImage((*reinterpret_cast< cv::Mat*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 17: { QImage _r = _t->mat2qimage((*reinterpret_cast< const cv::Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = _r; }  break;
        case 18: _t->trackDataPlot((*reinterpret_cast< cv::Mat*(*)>(_a[1])),(*reinterpret_cast< cv::Mat*(*)>(_a[2]))); break;
        case 19: _t->trackDataPlotEmbedded((*reinterpret_cast< cv::Mat*(*)>(_a[1]))); break;
        case 20: _t->trackDataPlotTrajectory((*reinterpret_cast< cv::Mat*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UIpiranha::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UIpiranha::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_UIpiranha,
      qt_meta_data_UIpiranha, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UIpiranha::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UIpiranha::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UIpiranha::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIpiranha))
        return static_cast<void*>(const_cast< UIpiranha*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int UIpiranha::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void UIpiranha::abortCapture()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
