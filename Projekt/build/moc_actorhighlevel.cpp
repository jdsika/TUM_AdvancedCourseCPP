/****************************************************************************
** Meta object code from reading C++ file 'actorhighlevel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Actor/actorhighlevel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'actorhighlevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ActorHighLevel_t {
    QByteArrayData data[23];
    char stringdata[339];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ActorHighLevel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ActorHighLevel_t qt_meta_stringdata_ActorHighLevel = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 28),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 8),
QT_MOC_LITERAL(4, 54, 9),
QT_MOC_LITERAL(5, 64, 16),
QT_MOC_LITERAL(6, 81, 12),
QT_MOC_LITERAL(7, 94, 12),
QT_MOC_LITERAL(8, 107, 13),
QT_MOC_LITERAL(9, 121, 11),
QT_MOC_LITERAL(10, 133, 1),
QT_MOC_LITERAL(11, 135, 14),
QT_MOC_LITERAL(12, 150, 19),
QT_MOC_LITERAL(13, 170, 28),
QT_MOC_LITERAL(14, 199, 9),
QT_MOC_LITERAL(15, 209, 15),
QT_MOC_LITERAL(16, 225, 22),
QT_MOC_LITERAL(17, 248, 21),
QT_MOC_LITERAL(18, 270, 14),
QT_MOC_LITERAL(19, 285, 19),
QT_MOC_LITERAL(20, 305, 9),
QT_MOC_LITERAL(21, 315, 1),
QT_MOC_LITERAL(22, 317, 20)
    },
    "ActorHighLevel\0signalSendRobotControlParams\0"
    "\0velocity\0turnangle\0signalSplinePlot\0"
    "PathPlotData\0pathPlotData\0signalPIDPlot\0"
    "PIDPlotData\0d\0signalPuckDone\0"
    "slotUpdateWaypoints\0QList<QPair<double,double> >\0"
    "waypoints\0slotReleasePuck\0"
    "slotPushAndReleasePuck\0m_releasePuckDistance\0"
    "slotGatherPuck\0slotChangePIDParams\0"
    "PIDParams\0p\0slotTimerSendPIDPlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ActorHighLevel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06,
       5,    1,   69,    2, 0x06,
       8,    1,   72,    2, 0x06,
      11,    0,   75,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      12,    1,   76,    2, 0x0a,
      15,    0,   79,    2, 0x0a,
      16,    1,   80,    2, 0x0a,
      18,    0,   83,    2, 0x0a,
      19,    1,   84,    2, 0x0a,
      22,    0,   87,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,

       0        // eod
};

void ActorHighLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ActorHighLevel *_t = static_cast<ActorHighLevel *>(_o);
        switch (_id) {
        case 0: _t->signalSendRobotControlParams((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->signalSplinePlot((*reinterpret_cast< PathPlotData(*)>(_a[1]))); break;
        case 2: _t->signalPIDPlot((*reinterpret_cast< PIDPlotData(*)>(_a[1]))); break;
        case 3: _t->signalPuckDone(); break;
        case 4: _t->slotUpdateWaypoints((*reinterpret_cast< QList<QPair<double,double> >(*)>(_a[1]))); break;
        case 5: _t->slotReleasePuck(); break;
        case 6: _t->slotPushAndReleasePuck((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->slotGatherPuck(); break;
        case 8: _t->slotChangePIDParams((*reinterpret_cast< PIDParams(*)>(_a[1]))); break;
        case 9: _t->slotTimerSendPIDPlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PIDPlotData >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PIDParams >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ActorHighLevel::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ActorHighLevel::signalSendRobotControlParams)) {
                *result = 0;
            }
        }
        {
            typedef void (ActorHighLevel::*_t)(PathPlotData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ActorHighLevel::signalSplinePlot)) {
                *result = 1;
            }
        }
        {
            typedef void (ActorHighLevel::*_t)(PIDPlotData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ActorHighLevel::signalPIDPlot)) {
                *result = 2;
            }
        }
        {
            typedef void (ActorHighLevel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ActorHighLevel::signalPuckDone)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ActorHighLevel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ActorHighLevel.data,
      qt_meta_data_ActorHighLevel,  qt_static_metacall, 0, 0}
};


const QMetaObject *ActorHighLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ActorHighLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ActorHighLevel.stringdata))
        return static_cast<void*>(const_cast< ActorHighLevel*>(this));
    return QObject::qt_metacast(_clname);
}

int ActorHighLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ActorHighLevel::signalSendRobotControlParams(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ActorHighLevel::signalSplinePlot(PathPlotData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ActorHighLevel::signalPIDPlot(PIDPlotData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ActorHighLevel::signalPuckDone()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
