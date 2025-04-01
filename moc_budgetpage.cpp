/****************************************************************************
** Meta object code from reading C++ file 'budgetpage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "budgetpage.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'budgetpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10BudgetPageE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10BudgetPageE = QtMocHelpers::stringData(
    "BudgetPage",
    "onBudgetGoalChangedSlot",
    "",
    "goal",
    "onBudgetChangeSlot",
    "budget",
    "onExpenseChangedSlot",
    "delta",
    "onBudgetPeriodTypeChangedSlot",
    "index",
    "onBudgetPeriodChangeSlot",
    "period",
    "newExpenseCategory",
    "name",
    "description",
    "changeBudgetPage",
    "newExpense",
    "deleteExpense",
    "BudgetPageExpenses*",
    "toDelete",
    "updateBarGraph",
    "addExpenseCategory",
    "changeExpenseCategory",
    "createBudgetsCSV",
    "importCSV"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10BudgetPageE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x0a,    1 /* Public */,
       4,    1,  101,    2, 0x0a,    3 /* Public */,
       6,    1,  104,    2, 0x0a,    5 /* Public */,
       8,    1,  107,    2, 0x0a,    7 /* Public */,
      10,    2,  110,    2, 0x0a,    9 /* Public */,
      12,    2,  115,    2, 0x0a,   12 /* Public */,
      15,    0,  120,    2, 0x08,   15 /* Private */,
      16,    0,  121,    2, 0x08,   16 /* Private */,
      17,    1,  122,    2, 0x08,   17 /* Private */,
      20,    0,  125,    2, 0x08,   19 /* Private */,
      21,    0,  126,    2, 0x08,   20 /* Private */,
      22,    1,  127,    2, 0x08,   21 /* Private */,
      23,    0,  130,    2, 0x08,   23 /* Private */,
      24,    0,  131,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Char,    9,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject BudgetPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN10BudgetPageE.offsetsAndSizes,
    qt_meta_data_ZN10BudgetPageE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10BudgetPageE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BudgetPage, std::true_type>,
        // method 'onBudgetGoalChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onBudgetChangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onExpenseChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onBudgetPeriodTypeChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onBudgetPeriodChangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<char, std::false_type>,
        // method 'newExpenseCategory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'changeBudgetPage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newExpense'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteExpense'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<BudgetPageExpenses *, std::false_type>,
        // method 'updateBarGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addExpenseCategory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changeExpenseCategory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'createBudgetsCSV'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'importCSV'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void BudgetPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BudgetPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onBudgetGoalChangedSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 1: _t->onBudgetChangeSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->onExpenseChangedSlot((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->onBudgetPeriodTypeChangedSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onBudgetPeriodChangeSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<char>>(_a[2]))); break;
        case 5: _t->newExpenseCategory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->changeBudgetPage(); break;
        case 7: _t->newExpense(); break;
        case 8: _t->deleteExpense((*reinterpret_cast< std::add_pointer_t<BudgetPageExpenses*>>(_a[1]))); break;
        case 9: _t->updateBarGraph(); break;
        case 10: _t->addExpenseCategory(); break;
        case 11: _t->changeExpenseCategory((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->createBudgetsCSV(); break;
        case 13: _t->importCSV(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< BudgetPageExpenses* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *BudgetPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BudgetPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10BudgetPageE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BudgetPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
