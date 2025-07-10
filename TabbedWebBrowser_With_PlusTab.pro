QT += core gui webenginewidgets webchannel
QT += webenginecore
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sample-webBrowser_9July

TEMPLATE = app

SOURCES += main.cpp \
    bridge.cpp \
    browserinterface.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h \
    bridge.h \
    browserinterface.h
FORMS += mainwindow.ui

DISTFILES += \
    home.html

RESOURCES += \
    resources.qrc
