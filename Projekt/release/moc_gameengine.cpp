/****************************************************************************
** Meta object code from reading C++ file 'gameengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AI/gameengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gameengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GameEngine_t {
    QByteArrayData data[22];
    char stringdata[362];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GameEngine_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GameEngine_t qt_meta_stringdata_GameEngine = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 26),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 19),
QT_MOC_LITERAL(4, 59, 20),
QT_MOC_LITERAL(5, 80, 15),
QT_MOC_LITERAL(6, 96, 15),
QT_MOC_LITERAL(7, 112, 21),
QT_MOC_LITERAL(8, 134, 8),
QT_MOC_LITERAL(9, 143, 5),
QT_MOC_LITERAL(10, 149, 14),
QT_MOC_LITERAL(11, 164, 14),
QT_MOC_LITERAL(12, 179, 15),
QT_MOC_LITERAL(13, 195, 16),
QT_MOC_LITERAL(14, 212, 20),
QT_MOC_LITERAL(15, 233, 19),
QT_MOC_LITERAL(16, 253, 21),
QT_MOC_LITERAL(17, 275, 22),
QT_MOC_LITERAL(18, 298, 9),
QT_MOC_LITERAL(19, 308, 16),
QT_MOC_LITERAL(20, 325, 19),
QT_MOC_LITERAL(21, 345, 15)
    },
    "GameEngine\0signalEmergencyStopEnabled\0"
    "\0enableEmergencyStop\0signalStartDetection\0"
    "startGameEngine\0signalStartGame\0"
    "slotDetectionFinished\0CamColor\0color\0"
    "slotReportGoal\0slotTimerAlive\0"
    "slotTimerEgoPos\0slotRefConnected\0"
    "slotRefConnectFailed\0slotRefDisconnected\0"
    "slotRefDetectionStart\0slotRefTrueColorOfTeam\0"
    "TeamColor\0slotRefGameStart\0"
    "slotRefStopMovement\0slotRefGameOver\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06,
       4,    1,   97,    2, 0x06,
       6,    0,  100,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       7,    1,  101,    2, 0x0a,
      10,    0,  104,    2, 0x0a,
       5,    0,  105,    2, 0x0a,
      11,    0,  106,    2, 0x08,
      12,    0,  107,    2, 0x08,
      13,    0,  108,    2, 0x08,
      14,    0,  109,    2, 0x08,
      15,    0,  110,    2, 0x08,
      16,    0,  111,    2, 0x08,
      17,    1,  112,    2, 0x08,
      19,    0,  115,    2, 0x08,
      20,    0,  116,    2, 0x08,
      21,    0,  117,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GameEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GameEngine *_t = static_cast<GameEngine *>(_o);
        switch (_id) {
        case 0: _t->signalEmergencyStopEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->signalStartDetection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->signalStartGame(); break;
        case 3: _t->slotDetectionFinished((*reinterpret_cast< CamColor(*)>(_a[1]))); break;
        case 4: _t->slotReportGoal(); break;
        case 5: _t->startGameEngine(); break;
        case 6: _t->slotTimerAlive(); break;
        case 7: _t->slotTimerEgoPos(); break;
        case 8: _t->slotRefConnected(); break;
        case 9: _t->slotRefConnectFailed(); break;
        case 10: _t->slotRefDisconnected(); break;
        case 11: _t->slotRefDetectionStart(); break;
        case 12: _t->slotRefTrueColorOfTeam((*reinterpret_cast< TeamColor(*)>(_a[1]))); break;
        case 13: _t->slotRefGameStart(); break;
        case 14: _t->slotRefStopMovement(); break;
        case 15: _t->slotRefGameOver(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GameEngine::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalEmergencyStopEnabled)) {
                *result = 0;
            }
        }
        {
            typedef void (GameEngine::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalStartDetection)) {
                *result = 1;
            }
        }
        {
            typedef void (GameEngine::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalStartGame)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GameEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GameEngine.data,
      qt_meta_data_GameEngine,  qt_static_metacall, 0, 0}
};


const QMetaObject *GameEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameEngine.stringdata))
        return static_cast<void*>(const_cast< GameEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int GameEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void GameEngine::signalEmergencyStopEnabled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GameEngine::signalStartDetection(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GameEngine::signalStartGame()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
