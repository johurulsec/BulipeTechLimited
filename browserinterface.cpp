// #include "browserinterface.h"

// BrowserInterface::BrowserInterface(QObject *parent)
//     : QObject{parent}
// {}

#include "browserinterface.h"
#include "mainwindow.h"

BrowserInterface::BrowserInterface(MainWindow *parent)
    : QObject(parent), mainWindow(parent) {}

void BrowserInterface::searchRequested(const QString &query) {
    mainWindow->handleSearchFromHomePage(query);
}
