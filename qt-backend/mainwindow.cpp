
// mainwindow.cpp
#include "mainwindow.h"
#include "custompage.h"
#include <QWebEngineProfile>
#include <QDebug>
#include <QVariant>
#include<QWebEngineHistory>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    qDebug()<<"constr mw";
    setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::setReactView(QWebEngineView* view) {
    qDebug()<<"setReactView() mw";
    reactView = view;
}

void MainWindow::createTab(int tabId) {
    if (tabContentViews.contains(tabId)){
        qDebug() << "MainWindow::createTab, tabContentViews.contains(tabId)" << tabId<<", so return";
        return;
    }
    qDebug() << "MainWindow::createTab" << tabId;
    auto *view = new QWebEngineView(this);    

    // give each view its own CustomPage so popups/new windows behave per-tab
    auto *page = new CustomPage(view);
    view->setPage(page);

    view->setVisible(false);
    view->resize(size());

    tabContentViews.insert(tabId, view);//new tab-page insert

    // forward new-tab requests from this page to the main window signal
    connect(page, &CustomPage::newTabRequested, this, &MainWindow::newTabRequested);

    // send URL changes to React
    connect(view, &QWebEngineView::urlChanged, this, [this, tabId](const QUrl &newUrl) {
        qDebug() << "URL changed for tab back/forward" << tabId << "to" << newUrl;
        emit urlChangedForTab(tabId, newUrl.toString());
    });
}

void MainWindow::closeTab(int tabId) {
    qDebug() << "MainWindow::closeTab" << tabId;

    if (!tabContentViews.contains(tabId)){
        qDebug() << "MainWindow::closeTab, !tabContentViews.contains(tabId)" << tabId<<", so return";
        return;
    }
    auto *view = tabContentViews.take(tabId);
    view->deleteLater();
    if (currentTabId == tabId) currentTabId = -1;
}

void MainWindow::setCurrentTab(int tabId) {
    qDebug()<<"MainWindow::setCurrentTab(), tabId:"<<tabId<<", currentTabId:"<<currentTabId;

    if (currentTabId == tabId){
        qDebug()<<"currentTabId == tabId, so return!";
        return;
    }
    if (tabContentViews.contains(currentTabId))
        tabContentViews[currentTabId]->setVisible(false);

    currentTabId = tabId;

    if (tabContentViews.contains(currentTabId)) {
        auto *v = tabContentViews[currentTabId];
        v->setVisible(true);
        v->raise();
        v->resize(size());
    }
}

void MainWindow::loadUrlForTab(int tabId, const QString &url) {
    qDebug() << "MainWindow::loadUrlForTab" << tabId << url;

    if (!tabContentViews.contains(tabId)) {
        createTab(tabId); // create on demand
    }
    auto *v = tabContentViews.value(tabId);
    if (!v) return;

    QUrl qurl = QUrl::fromUserInput(url);
    if (!qurl.isValid() || url.isEmpty()) {
        v->setVisible(false);
        qDebug() << "Invalid URL:" << url<<" so, return!";
        return;
    }

    v->setUrl(qurl);

    // position the view on top of the #webview-slot in React
    if (!reactView) return;
    reactView->page()->runJavaScript(R"(
        (function() {
            var el = document.getElementById('webview-slot');
            if (!el) return null;
            var rect = el.getBoundingClientRect();
            return { left: rect.left, top: rect.top, width: rect.width, height: rect.height };
        })()
    )", [this, tabId](const QVariant &result) {
                                         if (!result.isValid()) return;
                                         QVariantMap rect = result.toMap();
                                         int x = rect["left"].toInt();
                                         int y = rect["top"].toInt();
                                         int w = rect["width"].toInt();
                                         int h = rect["height"].toInt();
                                         if (!tabContentViews.contains(tabId)) return;
                                         auto *v = tabContentViews.value(tabId);
                                         v->setGeometry(x, y, w, h);
                                         if (currentTabId == tabId) {
                                             v->setVisible(true);
                                             v->raise();
                                         } else {
                                             v->setVisible(false);
                                         }
                                     });
}

void MainWindow::goBackForTab(int tabId) {
    qDebug() << "MainWindow::goBackForTab" << tabId;

    if (!tabContentViews.contains(tabId)) return;
    auto *v = tabContentViews.value(tabId);
    if (v->history()->canGoBack()) v->back();
}


void MainWindow::goForwardForTab(int tabId) {
    qDebug() << "MainWindow::goForwardForTab" << tabId;

    if (!tabContentViews.contains(tabId)) return;
    auto *v = tabContentViews.value(tabId);
    if (v->history()->canGoForward()) v->forward();
}

void MainWindow::reloadForTab(int tabId) {
    qDebug() << "MainWindow::reloadForTab" << tabId;

    if (!tabContentViews.contains(tabId)) return;
    tabContentViews.value(tabId)->reload();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    qDebug() << "MainWindow::resizeEvent";

    QWidget::resizeEvent(event);
    if (reactView) reactView->resize(size());
    for (auto view : tabContentViews) {
        view->resize(size());
    }
}

