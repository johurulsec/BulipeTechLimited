// //mainwindow.h
// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #pragma once
// #include <QWidget>
// #include<QDebug>
// #include<QResizeEvent>
// #include <QMap>

// class QWebEngineView;

// class MainWindow : public QWidget {
//     Q_OBJECT
// public:
//     explicit MainWindow(QWidget* parent = nullptr);

//     void setReactView(QWebEngineView* view);
//     void setContentView(QWebEngineView* view);

//     void addTabContentView(int tabIndex, QWebEngineView* view);
//     void setCurrentTab(int index);  // track which tab is active

// protected:
//     void resizeEvent(QResizeEvent* event) override;

// private:
//     QWebEngineView* reactView = nullptr;
//     QWebEngineView* contentView = nullptr;

//     QMap<int, QWebEngineView*> tabContentViews;

//     int currentTabIndex = 0;

// };

// #endif // MAINWINDOW_H

// mainwindow.h (add or update)
#pragma once

#include <QWidget>
#include <QMap>
#include <QWebEngineView>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

    void setReactView(QWebEngineView* view);

signals:
    void newTabRequested(const QUrl &url);
    void urlChangedForTab(int tabId, const QString &url);

public slots:
    void createTab(int tabId);
    void closeTab(int tabId);
    void setCurrentTab(int tabId);
    void loadUrlForTab(int tabId, const QString &url);
    void goBackForTab(int tabId);
    void goForwardForTab(int tabId);
    void reloadForTab(int tabId);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QMap<int, QWebEngineView*> tabContentViews;
    int currentTabId = -1;
    QWebEngineView* reactView = nullptr;
};

