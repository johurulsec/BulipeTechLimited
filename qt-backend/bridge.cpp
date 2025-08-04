//bridge.cpp
#include "bridge.h"
Bridge::Bridge(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"bridge constr() bridge.cpp";

}

void Bridge::setContentView(QWebEngineView *view)
{
    contentView = view;
}

void Bridge::log(const QString &message)
{
    qDebug() << "[JS] " << message<<", inside of bridge.cpp";
}

void Bridge::showAlert(const QString &message)
{
    qDebug()<<"showAlert()";

    QMessageBox::information(nullptr, "Alert from JS", message);
}

void Bridge::closeWindow()
{
    qDebug()<<"closeWindow() of bridge.cpp";
    emit requestClose();
}

void Bridge::minimizeWindow()
{
    qDebug()<<"minimizeWindow() of bridge.cpp";

    emit requestMinimize();
}

void Bridge::maximizeWindow()
{
    qDebug()<<"maximizeWindow() of bridge.cpp";

    emit requestMaximize();
}

void Bridge::receiveFromReact(const QString &param1, const QString &param2) {
    qDebug() << "Received params from React of bridge.cpp :" << param1 << param2;
    // Do something with the parameters...
}

void Bridge::loadUrl(const QString &url)
{
    qDebug() << "[JS] Request to load URL of bridge.cpp :" << url;
    emit requestLoadUrl(url);
}

void Bridge::goBack()
{
    qDebug()<<"goBack() of bridge.cpp";

    if (contentView && contentView->history()->canGoBack())
        contentView->back();
}

void Bridge::goForward() {
    qDebug()<<"goForward() of bridge.cpp";

    if (contentView && contentView->history()->canGoForward())
        contentView->forward();
}

void Bridge::reload() {
    qDebug()<<"reload() of bridge.cpp";

    if (contentView)
        contentView->reload();
}


