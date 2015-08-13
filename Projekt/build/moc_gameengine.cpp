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
    QByteArrayData data[24];
    char stringdata[392];
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
QT_MOC_LITERAL(4, 59, 15),
QT_MOC_LITERAL(5, 75, 20),
QT_MOC_LITERAL(6, 96, 15),
QT_MOC_LITERAL(7, 112, 14),
QT_MOC_LITERAL(8, 127, 21),
QT_MOC_LITERAL(9, 149, 8),
QT_MOC_LITERAL(10, 158, 5),
QT_MOC_LITERAL(11, 164, 14),
QT_MOC_LITERAL(12, 179, 14),
QT_MOC_LITERAL(13, 194, 15),
QT_MOC_LITERAL(14, 210, 16),
QT_MOC_LITERAL(15, 227, 20),
QT_MOC_LITERAL(16, 248, 19),
QT_MOC_LITERAL(17, 268, 21),
QT_MOC_LITERAL(18, 290, 22),
QT_MOC_LITERAL(19, 313, 9),
QT_MOC_LITERAL(20, 323, 16),
QT_MOC_LITERAL(21, 340, 19),
QT_MOC_LITERAL(22, 360, 15),
QT_MOC_LITERAL(23, 376, 14)
    },
    "GameEngine\0signalEmergencyStopEnabled\0"
    "\0enableEmergencyStop\0signalStartGame\0"
    "signalStartDetection\0startGameEngine\0"
    "signalAnnoyFoe\0slotDetectionFinished\0"
    "CamColor\0color\0slotReportGoal\0"
    "slotTimerAlive\0slotTimerEgoPos\0"
    "slotRefConnected\0slotRefConnectFailed\0"
    "slotRefDisconnected\0slotRefDetectionStart\0"
    "slotRefTrueColorOfTeam\0TeamColor\0"
    "slotRefGameStart\0slotRefStopMovement\0"
    "slotRefGameOver\0slotTimerAnnoy\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GameEngine[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06,
       4,    0,  102,    2, 0x06,
       5,    1,  103,    2, 0x06,
       7,    0,  106,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       8,    1,  107,    2, 0x0a,
      11,    0,  110,    2, 0x0a,
      12,    0,  111,    2, 0x08,
      13,    0,  112,    2, 0x08,
      14,    0,  113,    2, 0x08,
      15,    0,  114,    2, 0x08,
      16,    0,  115,    2, 0x08,
      17,    0,  116,    2, 0x08,
      18,    1,  117,    2, 0x08,
      20,    0,  120,    2, 0x08,
      21,    0,  121,    2, 0x08,
      22,    0,  122,    2, 0x08,
      23,    0,  123,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   10,
    QMetaType::Void,
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
        case 1: _t->signalStartGame(); break;
        case 2: _t->signalStartDetection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->signalAnnoyFoe(); break;
        case 4: _t->slotDetectionFinished((*reinterpret_cast< CamColor(*)>(_a[1]))); break;
        case 5: _t->slotReportGoal(); break;
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
        case 16: _t->slotTimerAnnoy(); break;
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
            typedef void (GameEngine::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalStartGame)) {
                *result = 1;
            }
        }
        {
            typedef void (GameEngine::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalStartDetection)) {
                *result = 2;
            }
        }
        {
            typedef void (GameEngine::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GameEngine::signalAnnoyFoe)) {
                *result = 3;
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
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
void GameEngine::signalStartGame()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GameEngine::signalStartDetection(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GameEngine::signalAnnoyFoe()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
