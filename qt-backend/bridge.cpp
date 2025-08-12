// bridge.cpp
#include "bridge.h"
#include <QDebug>

Bridge::Bridge(QObject* parent) : QObject(parent) {
    qDebug() << "Bridge constructed";
}

void Bridge::createTab(int tabId) {
    qDebug() << "Bridge::createTab" << tabId;
    emit requestCreateTab(tabId);
}

void Bridge::closeTab(int tabId) {
    qDebug() << "Bridge::closeTab" << tabId;
    emit requestCloseTab(tabId);
}

void Bridge::setCurrentTab(int tabId) {
    qDebug() << "Bridge::setCurrentTab" << tabId;
    m_currentTabId = tabId;
    emit requestSetCurrentTab(tabId);
}

void Bridge::loadUrl(const QString &url) {
    qDebug() << "Bridge::loadUrl() Request to load URL :" << url << " for m_currentTabId:" << m_currentTabId;
    if (m_currentTabId < 0) {
        qDebug() << "Bridge: no current tab set";
        return;
    }
    emit requestLoadUrlForTab(m_currentTabId, url);
}

void Bridge::goBack() {
    qDebug() << "Bridge::goBack for tab" << m_currentTabId;
    if (m_currentTabId < 0) return;
    emit requestGoBackForTab(m_currentTabId);
}

void Bridge::goForward() {
    qDebug() << "Bridge::goForward for tab" << m_currentTabId;
    if (m_currentTabId < 0) return;
    emit requestGoForwardForTab(m_currentTabId);
}

void Bridge::reload() {
    qDebug() << "Bridge::reload for tab" << m_currentTabId;
    if (m_currentTabId < 0) return;
    emit requestReloadForTab(m_currentTabId);
}

void Bridge::sendOpenInNewTab(const QUrl &url) {
    qDebug() << "Bridge::sendOpenInNewTab(" << url << ")";
    emit openInNewTab(url.toString());
}

void Bridge::closeWindow()
{
    qDebug()<<"Bridge::closeWindow()";
    emit requestClose();
}

void Bridge::minimizeWindow()
{
    qDebug()<<"Bridge::minimizeWindow()";

    emit requestMinimize();
}

void Bridge::maximizeWindow()
{
    qDebug()<<"Bridge::maximizeWindow()";

    emit requestMaximize();
}

void Bridge::log(const QString &message)
{
    qDebug() << "[JS] " << message<<" of Bridge::log";

    // check if it contains the MUI Tabs invalid value error
    if (message.contains("MUI: The `value` provided to the Tabs component is invalid",
                         Qt::CaseInsensitive))
    {
        qDebug() << "Detected MUI Tabs invalid value error! No tab contain. so, emit requestClose() signal";
        emit requestClose();
    }
}



