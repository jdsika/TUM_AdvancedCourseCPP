/****************************************************************************
** Meta object code from reading C++ file 'sensorhighlevel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Sensor/sensorhighlevel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensorhighlevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SensorHighLevel_t {
    QByteArrayData data[27];
    char stringdata[410];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_SensorHighLevel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_SensorHighLevel_t qt_meta_stringdata_SensorHighLevel = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 28),
QT_MOC_LITERAL(2, 45, 0),
QT_MOC_LITERAL(3, 46, 8),
QT_MOC_LITERAL(4, 55, 9),
QT_MOC_LITERAL(5, 65, 26),
QT_MOC_LITERAL(6, 92, 19),
QT_MOC_LITERAL(7, 112, 13),
QT_MOC_LITERAL(8, 126, 9),
QT_MOC_LITERAL(9, 136, 22),
QT_MOC_LITERAL(10, 159, 8),
QT_MOC_LITERAL(11, 168, 13),
QT_MOC_LITERAL(12, 182, 19),
QT_MOC_LITERAL(13, 202, 8),
QT_MOC_LITERAL(14, 211, 5),
QT_MOC_LITERAL(15, 217, 25),
QT_MOC_LITERAL(16, 243, 17),
QT_MOC_LITERAL(17, 261, 16),
QT_MOC_LITERAL(18, 278, 15),
QT_MOC_LITERAL(19, 294, 10),
QT_MOC_LITERAL(20, 305, 14),
QT_MOC_LITERAL(21, 320, 19),
QT_MOC_LITERAL(22, 340, 12),
QT_MOC_LITERAL(23, 353, 12),
QT_MOC_LITERAL(24, 366, 18),
QT_MOC_LITERAL(25, 385, 5),
QT_MOC_LITERAL(26, 391, 17)
    },
    "SensorHighLevel\0signalSendRobotControlParams\0"
    "\0velocity\0turnangle\0signalEmergencyStopEnabled\0"
    "signalSendLaserData\0LaserPlotData\0"
    "laserData\0signalSendOdometryData\0"
    "Position\0finalPosition\0signalSendTeamColor\0"
    "CamColor\0color\0signalStartColorDetection\0"
    "signalPlanNewPath\0slotGetLaserData\0"
    "QVector<double>\0sensorData\0positionSignal\0"
    "slotSetFilterParams\0FilterParams\0"
    "cameraParams\0slotStartDetection\0start\0"
    "slotColorDetected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorHighLevel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06,
       5,    1,   74,    2, 0x06,
       6,    1,   77,    2, 0x06,
       9,    1,   80,    2, 0x06,
      12,    1,   83,    2, 0x06,
      15,    0,   86,    2, 0x06,
      16,    0,   87,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      17,    2,   88,    2, 0x0a,
      21,    1,   93,    2, 0x0a,
      24,    1,   96,    2, 0x0a,
      26,    1,   99,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 10,   19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::Bool,   25,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void SensorHighLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SensorHighLevel *_t = static_cast<SensorHighLevel *>(_o);
        switch (_id) {
        case 0: _t->signalSendRobotControlParams((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->signalEmergencyStopEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->signalSendLaserData((*reinterpret_cast< LaserPlotData(*)>(_a[1]))); break;
        case 3: _t->signalSendOdometryData((*reinterpret_cast< Position(*)>(_a[1]))); break;
        case 4: _t->signalSendTeamColor((*reinterpret_cast< CamColor(*)>(_a[1]))); break;
        case 5: _t->signalStartColorDetection(); break;
        case 6: _t->signalPlanNewPath(); break;
        case 7: _t->slotGetLaserData((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< Position(*)>(_a[2]))); break;
        case 8: _t->slotSetFilterParams((*reinterpret_cast< FilterParams(*)>(_a[1]))); break;
        case 9: _t->slotStartDetection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->slotColorDetected((*reinterpret_cast< CamColor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LaserPlotData >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Position >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Position >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< FilterParams >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SensorHighLevel::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalSendRobotControlParams)) {
                *result = 0;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalEmergencyStopEnabled)) {
                *result = 1;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)(LaserPlotData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalSendLaserData)) {
                *result = 2;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)(Position );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalSendOdometryData)) {
                *result = 3;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)(CamColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalSendTeamColor)) {
                *result = 4;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalStartColorDetection)) {
                *result = 5;
            }
        }
        {
            typedef void (SensorHighLevel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SensorHighLevel::signalPlanNewPath)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject SensorHighLevel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SensorHighLevel.data,
      qt_meta_data_SensorHighLevel,  qt_static_metacall, 0, 0}
};


const QMetaObject *SensorHighLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorHighLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SensorHighLevel.stringdata))
        return static_cast<void*>(const_cast< SensorHighLevel*>(this));
    return QObject::qt_metacast(_clname);
}

int SensorHighLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void SensorHighLevel::signalSendRobotControlParams(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SensorHighLevel::signalEmergencyStopEnabled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SensorHighLevel::signalSendLaserData(LaserPlotData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SensorHighLevel::signalSendOdometryData(Position _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SensorHighLevel::signalSendTeamColor(CamColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SensorHighLevel::signalStartColorDetection()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void SensorHighLevel::signalPlanNewPath()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
