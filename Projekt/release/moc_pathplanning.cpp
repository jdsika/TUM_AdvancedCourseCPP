/****************************************************************************
** Meta object code from reading C++ file 'pathplanning.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AI/pathplanning.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pathplanning.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PathPlanning_t {
    QByteArrayData data[9];
    char stringdata[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PathPlanning_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PathPlanning_t qt_meta_stringdata_PathPlanning = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 20),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 28),
QT_MOC_LITERAL(4, 64, 9),
QT_MOC_LITERAL(5, 74, 11),
QT_MOC_LITERAL(6, 86, 12),
QT_MOC_LITERAL(7, 99, 10),
QT_MOC_LITERAL(8, 110, 8)
    },
    "PathPlanning\0sendUpdatedWaypoints\0\0"
    "QList<QPair<double,double> >\0waypoints\0"
    "pathDisplay\0PathPlotData\0dataPacket\0"
    "planPath\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PathPlanning[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06,
       5,    1,   32,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       8,    0,   35,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void PathPlanning::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PathPlanning *_t = static_cast<PathPlanning *>(_o);
        switch (_id) {
        case 0: _t->sendUpdatedWaypoints((*reinterpret_cast< QList<QPair<double,double> >(*)>(_a[1]))); break;
        case 1: _t->pathDisplay((*reinterpret_cast< PathPlotData(*)>(_a[1]))); break;
        case 2: _t->planPath(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PathPlotData >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PathPlanning::*_t)(QList<QPair<double,double> > );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PathPlanning::sendUpdatedWaypoints)) {
                *result = 0;
            }
        }
        {
            typedef void (PathPlanning::*_t)(PathPlotData );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PathPlanning::pathDisplay)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject PathPlanning::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PathPlanning.data,
      qt_meta_data_PathPlanning,  qt_static_metacall, 0, 0}
};


const QMetaObject *PathPlanning::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PathPlanning::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PathPlanning.stringdata))
        return static_cast<void*>(const_cast< PathPlanning*>(this));
    return QObject::qt_metacast(_clname);
}

int PathPlanning::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PathPlanning::sendUpdatedWaypoints(QList<QPair<double,double> > _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PathPlanning::pathDisplay(PathPlotData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
