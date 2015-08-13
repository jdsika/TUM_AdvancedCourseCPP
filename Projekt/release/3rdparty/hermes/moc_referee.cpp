/****************************************************************************
** Meta object code from reading C++ file 'referee.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../3rdparty/hermes/referee.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'referee.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Referee_t {
    QByteArrayData data[18];
    char stringdata[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Referee_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Referee_t qt_meta_stringdata_Referee = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 12),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 14),
QT_MOC_LITERAL(4, 37, 9),
QT_MOC_LITERAL(5, 47, 8),
QT_MOC_LITERAL(6, 56, 8),
QT_MOC_LITERAL(7, 65, 1),
QT_MOC_LITERAL(8, 67, 1),
QT_MOC_LITERAL(9, 69, 15),
QT_MOC_LITERAL(10, 85, 9),
QT_MOC_LITERAL(11, 95, 5),
QT_MOC_LITERAL(12, 101, 12),
QT_MOC_LITERAL(13, 114, 9),
QT_MOC_LITERAL(14, 124, 13),
QT_MOC_LITERAL(15, 138, 8),
QT_MOC_LITERAL(16, 147, 13),
QT_MOC_LITERAL(17, 161, 16)
    },
    "Referee\0disconnected\0\0detectionStart\0"
    "gameStart\0gameOver\0abValues\0a\0b\0"
    "trueColorOfTeam\0TeamColor\0color\0"
    "stopMovement\0connected\0connectFailed\0"
    "slotRead\0slotConnected\0slotDisconnected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Referee[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06,
       3,    0,   75,    2, 0x06,
       4,    0,   76,    2, 0x06,
       5,    0,   77,    2, 0x06,
       6,    2,   78,    2, 0x06,
       9,    1,   83,    2, 0x06,
      12,    0,   86,    2, 0x06,
      13,    0,   87,    2, 0x06,
      14,    0,   88,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      15,    0,   89,    2, 0x08,
      16,    0,   90,    2, 0x08,
      17,    0,   91,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Referee::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Referee *_t = static_cast<Referee *>(_o);
        switch (_id) {
        case 0: _t->disconnected(); break;
        case 1: _t->detectionStart(); break;
        case 2: _t->gameStart(); break;
        case 3: _t->gameOver(); break;
        case 4: _t->abValues((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->trueColorOfTeam((*reinterpret_cast< TeamColor(*)>(_a[1]))); break;
        case 6: _t->stopMovement(); break;
        case 7: _t->connected(); break;
        case 8: _t->connectFailed(); break;
        case 9: _t->slotRead(); break;
        case 10: _t->slotConnected(); break;
        case 11: _t->slotDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::disconnected)) {
                *result = 0;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::detectionStart)) {
                *result = 1;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::gameStart)) {
                *result = 2;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::gameOver)) {
                *result = 3;
            }
        }
        {
            typedef void (Referee::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::abValues)) {
                *result = 4;
            }
        }
        {
            typedef void (Referee::*_t)(TeamColor );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::trueColorOfTeam)) {
                *result = 5;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::stopMovement)) {
                *result = 6;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::connected)) {
                *result = 7;
            }
        }
        {
            typedef void (Referee::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Referee::connectFailed)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject Referee::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Referee.data,
      qt_meta_data_Referee,  qt_static_metacall, 0, 0}
};


const QMetaObject *Referee::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Referee::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Referee.stringdata))
        return static_cast<void*>(const_cast< Referee*>(this));
    return QObject::qt_metacast(_clname);
}

int Referee::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Referee::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Referee::detectionStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Referee::gameStart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Referee::gameOver()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Referee::abValues(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Referee::trueColorOfTeam(TeamColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Referee::stopMovement()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Referee::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Referee::connectFailed()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
