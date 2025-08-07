
// custompage.h
#ifndef CUSTOMPAGE_H
#define CUSTOMPAGE_H

#include <QObject>
#include <QWebEnginePage>
#include<QDebug>

class CustomPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit CustomPage(QObject* parent = nullptr);

signals:
    void newTabRequested(const QUrl& url); // This will notify us to open new tab

protected:
    QWebEnginePage* createWindow(WebWindowType type) override;
};

#endif // CUSTOMPAGE_H
