// #include "bridge.h"

// Bridge::Bridge(QObject *parent)
//     : QObject{parent}
// {}


#include "bridge.h"
#include <QDebug>

void Bridge::searchFromHome(const QString &query) {
    emit searchRequested(query);
}
