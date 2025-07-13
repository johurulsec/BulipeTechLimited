QT += core gui webenginewidgets webchannel
QT += webenginecore
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webBrowser_July

TEMPLATE = app

SOURCES += main.cpp \
    browserinterface.cpp \
    mainwindow.cpp
HEADERS += mainwindow.h \
    browserinterface.h
FORMS += mainwindow.ui

DISTFILES += \
    home.html

RESOURCES += \
    resources.qrc
