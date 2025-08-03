//bridge.cpp
#include "bridge.h"
Bridge::Bridge(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"bridge constr()";

}

void Bridge::log(const QString &message)
{
    qDebug() << "[JS] " << message;
}

void Bridge::showAlert(const QString &message)
{
    qDebug()<<"showAlert()";

    QMessageBox::information(nullptr, "Alert from JS", message);
}

void Bridge::closeWindow()
{
    qDebug()<<"closeWindow()";
    emit requestClose();
}

void Bridge::minimizeWindow()
{
    qDebug()<<"minimizeWindow()";

    emit requestMinimize();
}

void Bridge::maximizeWindow()
{
    qDebug()<<"maximizeWindow()";

    emit requestMaximize();
}

void Bridge::receiveFromReact(const QString &param1, const QString &param2) {
    qDebug() << "Received params from React:" << param1 << param2;
    // Do something with the parameters...
}

void Bridge::loadUrl(const QString &url)
{
    qDebug() << "[JS] Request to load URL of bridge.cpp :" << url;
    emit requestLoadUrl(url);
}


