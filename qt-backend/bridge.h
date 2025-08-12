
// bridge.h
#pragma once

#include <QObject>
#include <QWebEngineView>

class Bridge : public QObject {
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr);

    Q_INVOKABLE void createTab(int tabId);
    Q_INVOKABLE void closeTab(int tabId);
    Q_INVOKABLE void setCurrentTab(int tabId);
    Q_INVOKABLE void loadUrl(const QString &url);
    Q_INVOKABLE void goBack();
    Q_INVOKABLE void goForward();
    Q_INVOKABLE void reload();

    // helper to send new tab events from C++ to React
    void sendOpenInNewTab(const QUrl &url);


public slots:
    void log(const QString &message);
    void closeWindow();
    void minimizeWindow();
    void maximizeWindow();    

signals:
    // Event emitted to React when C++ says a new tab should open
    void openInNewTab(const QString &url);

    // Routing signals that MainWindow will handle
    void requestCreateTab(int tabId);
    void requestCloseTab(int tabId);
    void requestSetCurrentTab(int tabId);
    void requestLoadUrlForTab(int tabId, const QString &url);
    void requestGoBackForTab(int tabId);
    void requestGoForwardForTab(int tabId);
    void requestReloadForTab(int tabId);

    void requestClose();

    void requestMinimize();
    void requestMaximize();
    void updateTabUrl(int tabId, const QString &url);

private:
    int m_currentTabId = -1;
};




