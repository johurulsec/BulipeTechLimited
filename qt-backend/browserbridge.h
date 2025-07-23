#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QDebug>

class Bridge : public QObject {
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void log(const QString &message) {
        qDebug() << "[React UI]" << message;
    }
};

#endif // BRIDGE_H
