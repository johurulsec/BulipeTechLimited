/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "addToHistory",
    "",
    "url",
    "updateHistoryMenu",
    "setupHistoryDock",
    "addToBookmarks",
    "updateBookmarksMenu",
    "setupDownloadManager",
    "handleDownload",
    "QWebEngineDownloadRequest*",
    "download",
    "handleSearchFromHomePage",
    "query",
    "addNewTab",
    "onBackClicked",
    "onForwardClicked",
    "onReloadClicked",
    "onUrlEntered",
    "onSearchClicked",
    "onTabChanged",
    "index",
    "onUrlChanged",
    "closeTab"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  122,    2, 0x0a,    1 /* Public */,
       4,    0,  125,    2, 0x0a,    3 /* Public */,
       5,    0,  126,    2, 0x0a,    4 /* Public */,
       6,    1,  127,    2, 0x0a,    5 /* Public */,
       7,    0,  130,    2, 0x0a,    7 /* Public */,
       8,    0,  131,    2, 0x0a,    8 /* Public */,
       9,    1,  132,    2, 0x0a,    9 /* Public */,
      12,    1,  135,    2, 0x0a,   11 /* Public */,
      14,    1,  138,    2, 0x08,   13 /* Private */,
      14,    0,  141,    2, 0x28,   15 /* Private | MethodCloned */,
      15,    0,  142,    2, 0x08,   16 /* Private */,
      16,    0,  143,    2, 0x08,   17 /* Private */,
      17,    0,  144,    2, 0x08,   18 /* Private */,
      18,    0,  145,    2, 0x08,   19 /* Private */,
      19,    0,  146,    2, 0x08,   20 /* Private */,
      20,    1,  147,    2, 0x08,   21 /* Private */,
      22,    1,  150,    2, 0x08,   23 /* Private */,
      23,    1,  153,    2, 0x08,   25 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QUrl,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QUrl,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QUrl,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::QUrl,    3,
    QMetaType::Void, QMetaType::Int,   21,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'addToHistory'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>,
        // method 'updateHistoryMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupHistoryDock'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addToBookmarks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>,
        // method 'updateBookmarksMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupDownloadManager'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleDownload'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWebEngineDownloadRequest *, std::false_type>,
        // method 'handleSearchFromHomePage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addNewTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>,
        // method 'addNewTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBackClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onForwardClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReloadClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUrlEntered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onUrlChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QUrl &, std::false_type>,
        // method 'closeTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addToHistory((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 1: _t->updateHistoryMenu(); break;
        case 2: _t->setupHistoryDock(); break;
        case 3: _t->addToBookmarks((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 4: _t->updateBookmarksMenu(); break;
        case 5: _t->setupDownloadManager(); break;
        case 6: _t->handleDownload((*reinterpret_cast< std::add_pointer_t<QWebEngineDownloadRequest*>>(_a[1]))); break;
        case 7: _t->handleSearchFromHomePage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->addNewTab((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 9: _t->addNewTab(); break;
        case 10: _t->onBackClicked(); break;
        case 11: _t->onForwardClicked(); break;
        case 12: _t->onReloadClicked(); break;
        case 13: _t->onUrlEntered(); break;
        case 14: _t->onSearchClicked(); break;
        case 15: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->onUrlChanged((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 17: _t->closeTab((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWebEngineDownloadRequest* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
