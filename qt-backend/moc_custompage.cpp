/****************************************************************************
** Meta object code from reading C++ file 'custompage.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "custompage.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'custompage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN10CustomPageE_t {};
} // unnamed namespace

template <> constexpr inline auto CustomPage::qt_create_metaobjectdata<qt_meta_tag_ZN10CustomPageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CustomPage",
        "newTabRequested",
        "",
        "url"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'newTabRequested'
        QtMocHelpers::SignalData<void(const QUrl &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QUrl, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CustomPage, qt_meta_tag_ZN10CustomPageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CustomPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWebEnginePage::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10CustomPageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10CustomPageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10CustomPageE_t>.metaTypes,
    nullptr
} };

void CustomPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CustomPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newTabRequested((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CustomPage::*)(const QUrl & )>(_a, &CustomPage::newTabRequested, 0))
            return;
    }
}

const QMetaObject *CustomPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10CustomPageE_t>.strings))
        return static_cast<void*>(this);
    return QWebEnginePage::qt_metacast(_clname);
}

int CustomPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebEnginePage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CustomPage::newTabRequested(const QUrl & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
