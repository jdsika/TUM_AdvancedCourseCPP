/****************************************************************************
** Meta object code from reading C++ file 'game.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AI/game.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Game_t {
    QByteArrayData data[16];
    char stringdata[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Game_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Game_t qt_meta_stringdata_Game = {
    {
QT_MOC_LITERAL(0, 0, 4),
QT_MOC_LITERAL(1, 5, 16),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 17),
QT_MOC_LITERAL(4, 41, 24),
QT_MOC_LITERAL(5, 66, 20),
QT_MOC_LITERAL(6, 87, 19),
QT_MOC_LITERAL(7, 107, 16),
QT_MOC_LITERAL(8, 124, 13),
QT_MOC_LITERAL(9, 138, 15),
QT_MOC_LITERAL(10, 154, 8),
QT_MOC_LITERAL(11, 163, 5),
QT_MOC_LITERAL(12, 169, 32),
QT_MOC_LITERAL(13, 202, 4),
QT_MOC_LITERAL(14, 207, 3),
QT_MOC_LITERAL(15, 211, 12)
    },
    "Game\0signalReportGoal\0\0signalPuckRelease\0"
    "signalStartColorDetectAI\0signalPushAndRelease\0"
    "releasePuckDistance\0signalGatherPuck\0"
    "slotStartGame\0slotColorDetect\0CamColor\0"
    "color\0slotActorHighLevelIsDoneWithPuck\0"
    "quit\0run\0slotAnnoyFoe\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Game[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06,
       3,    0,   70,    2, 0x06,
       4,    0,   71,    2, 0x06,
       5,    1,   72,    2, 0x06,
       7,    0,   75,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       8,    0,   76,    2, 0x0a,
       9,    1,   77,    2, 0x0a,
      12,    0,   80,    2, 0x0a,
      13,    0,   81,    2, 0x0a,
      14,    0,   82,    2, 0x0a,
      15,    0,   83,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Game::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Game *_t = static_cast<Game *>(_o);
        switch (_id) {
        case 0: _t->signalReportGoal(); break;
        case 1: _t->signalPuckRelease(); break;
        case 2: _t->signalStartColorDetectAI(); break;
        case 3: _t->signalPushAndRelease((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->signalGatherPuck(); break;
        case 5: _t->slotStartGame(); break;
        case 6: _t->slotColorDetect((*reinterpret_cast< CamColor(*)>(_a[1]))); break;
        case 7: _t->slotActorHighLevelIsDoneWithPuck(); break;
        case 8: _t->quit(); break;
        case 9: _t->run(); break;
        case 10: _t->slotAnnoyFoe(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Game::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Game::signalReportGoal)) {
                *result = 0;
            }
        }
        {
            typedef void (Game::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Game::signalPuckRelease)) {
                *result = 1;
            }
        }
        {
            typedef void (Game::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Game::signalStartColorDetectAI)) {
                *result = 2;
            }
        }
        {
            typedef void (Game::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Game::signalPushAndRelease)) {
                *result = 3;
            }
        }
        {
            typedef void (Game::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Game::signalGatherPuck)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject Game::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Game.data,
      qt_meta_data_Game,  qt_static_metacall, 0, 0}
};


const QMetaObject *Game::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Game::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Game.stringdata))
        return static_cast<void*>(const_cast< Game*>(this));
    return QObject::qt_metacast(_clname);
}

int Game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Game::signalReportGoal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Game::signalPuckRelease()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Game::signalStartColorDetectAI()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Game::signalPushAndRelease(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Game::signalGatherPuck()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
