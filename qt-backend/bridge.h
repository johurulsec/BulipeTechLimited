#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QMessageBox>
class Bridge : public QObject {
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr);

public slots:
    void log(const QString &message);
    void showAlert(const QString &message);
    void closeWindow();

    void minimizeWindow();
    void maximizeWindow();
    void receiveFromReact(const QString &param1, const QString &param2);
    void loadUrl(const QString &url);


signals:
    void notify(const QString &message);
    void requestClose();

    void requestMinimize();
    void requestMaximize();
    void requestLocalStorage();
    void requestLoadUrl(const QString &url);
    void updateTitle(const QString &title);

};

#endif // BRIDGE_H
