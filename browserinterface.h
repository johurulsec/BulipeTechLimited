
#ifndef BROWSERINTERFACE_H
#define BROWSERINTERFACE_H

#include <QObject>

class MainWindow;

class BrowserInterface : public QObject {
    Q_OBJECT
public:
    explicit BrowserInterface(MainWindow *parent = nullptr);

public slots:
    void searchRequested(const QString &query);

private:
    MainWindow *mainWindow;
};

#endif // BROWSERINTERFACE_H
