QT += core gui webenginewidgets webchannel \
    widgets

CONFIG += c++17

TARGET = webBrowser

TEMPLATE = app

SOURCES += main.cpp \
    bridge.cpp \
    custompage.cpp \
    mainwindow.cpp

HEADERS += bridge.h \
    custompage.h \
    mainwindow.h

RESOURCES += resources.qrc

