/****************************************************************************
** Meta object code from reading C++ file 'actorLowLevel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Actor/actorLowLevel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'actorLowLevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ActorLowLevel_t {
    QByteArrayData data[11];
    char stringdata[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ActorLowLevel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ActorLowLevel_t qt_meta_stringdata_ActorLowLevel = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 28),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 8),
QT_MOC_LITERAL(4, 53, 9),
QT_MOC_LITERAL(5, 63, 22),
QT_MOC_LITERAL(6, 86, 24),
QT_MOC_LITERAL(7, 111, 11),
QT_MOC_LITERAL(8, 123, 11),
QT_MOC_LITERAL(9, 135, 8),
QT_MOC_LITERAL(10, 144, 15)
    },
    "ActorLowLevel\0setRobotRemoteControllParams\0"
    "\0velocity\0turnangle\0setRobotControllParams\0"
    "slotEmergencyStopEnabled\0isEmergency\0"
    "setOdometry\0Position\0currentPosition\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ActorLowLevel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x0a,
       5,    2,   39,    2, 0x0a,
       6,    1,   44,    2, 0x0a,
       8,    1,   47,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void ActorLowLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ActorLowLevel *_t = static_cast<ActorLowLevel *>(_o);
        switch (_id) {
        case 0: _t->setRobotRemoteControllParams((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->setRobotControllParams((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->slotEmergencyStopEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setOdometry((*reinterpret_cast< Position(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Position >(); break;
            }
            break;
        }
    }
}

const QMetaObject ActorLowLevel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ActorLowLevel.data,
      qt_meta_data_ActorLowLevel,  qt_static_metacall, 0, 0}
};


const QMetaObject *ActorLowLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ActorLowLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ActorLowLevel.stringdata))
        return static_cast<void*>(const_cast< ActorLowLevel*>(this));
    return QObject::qt_metacast(_clname);
}

int ActorLowLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
