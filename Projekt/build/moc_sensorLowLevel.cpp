/****************************************************************************
** Meta object code from reading C++ file 'sensorLowLevel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Sensor/sensorLowLevel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensorLowLevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SensorLowLevel_t {
    QByteArrayData data[11];
    char stringdata[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_SensorLowLevel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_SensorLowLevel_t qt_meta_stringdata_SensorLowLevel = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 20),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 15),
QT_MOC_LITERAL(4, 53, 8),
QT_MOC_LITERAL(5, 62, 18),
QT_MOC_LITERAL(6, 81, 13),
QT_MOC_LITERAL(7, 95, 13),
QT_MOC_LITERAL(8, 109, 22),
QT_MOC_LITERAL(9, 132, 4),
QT_MOC_LITERAL(10, 137, 3)
    },
    "SensorLowLevel\0signalLaserDataReady\0"
    "\0QVector<double>\0Position\0signalLaserPlotRaw\0"
    "LaserPlotData\0laserPlotData\0"
    "signalSimulationDetect\0quit\0run\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorLowLevel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06,
       5,    1,   44,    2, 0x06,
       8,    0,   47,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    0,   48,    2, 0x0a,
      10,    0,   49,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SensorLowLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SensorLowLevel *_t = static_cast<SensorLowLevel *>(_o);
        switch (_id) {
        case 0: _t->signalLaserDataReady((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< Position(*)>(_a[2]))); break;
        case 1: _t->signalLaserPlotRaw((*reinterpret_cast< LaserPlotData(*)>(_a[1]))); break;
        case 2: _t->signalSimulationDetect(); break;
        case 3: _t->quit(); break;
        case 4: _t->run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Position >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LaserPlotData >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SensorLowLevel::*_t)(QVector<double> , Position );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorLowLevel::signalLaserDataReady)) {
                *result = 0;
            }
        }
        {
            typedef void (SensorLowLevel::*_t)(LaserPlotData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorLowLevel::signalLaserPlotRaw)) {
                *result = 1;
            }
        }
        {
            typedef void (SensorLowLevel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorLowLevel::signalSimulationDetect)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SensorLowLevel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SensorLowLevel.data,
      qt_meta_data_SensorLowLevel,  qt_static_metacall, 0, 0}
};


const QMetaObject *SensorLowLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorLowLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SensorLowLevel.stringdata))
        return static_cast<void*>(const_cast< SensorLowLevel*>(this));
    return QObject::qt_metacast(_clname);
}

int SensorLowLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SensorLowLevel::signalLaserDataReady(QVector<double> _t1, Position _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SensorLowLevel::signalLaserPlotRaw(LaserPlotData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SensorLowLevel::signalSimulationDetect()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
