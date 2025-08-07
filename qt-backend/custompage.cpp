
// custompage.cpp
#include "custompage.h"
#include <QWebEngineProfile>

CustomPage::CustomPage(QObject* parent)
    : QWebEnginePage(parent) {}

QWebEnginePage* CustomPage::createWindow(WebWindowType type) {
    Q_UNUSED(type);

    qDebug()<<"CustomPage::createWindow";

    // Temporary page just to get the URL
    auto tempPage = new QWebEnginePage(profile(), this);

    connect(tempPage, &QWebEnginePage::urlChanged, this, [this, tempPage](const QUrl& url) {
        emit newTabRequested(url);
        tempPage->deleteLater(); // Cleanup
    });

    return tempPage;
}
