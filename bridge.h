// #ifndef BRIDGE_H
// #define BRIDGE_H

// #include <QObject>

// class Bridge : public QObject
// {
//     Q_OBJECT
// public:
//     explicit Bridge(QObject *parent = nullptr);

// signals:
// };

// #endif // BRIDGE_H


#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QString>

class Bridge : public QObject {
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void searchRequested(const QString &query);

public slots:
    void searchFromHome(const QString &query);
};

#endif // BRIDGE_H
