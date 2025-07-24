QT += core gui webenginewidgets webchannel \
    widgets

CONFIG += c++17

TARGET = webBrowser

TEMPLATE = app

SOURCES += main.cpp \
    bridge.cpp \
    mainwindow.cpp

HEADERS += bridge.h \
    bridge.h \
    mainwindow.h
