#include "browserbridge.h"
// Implementation already in header (inline)
class Bridge : public QObject {
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void log(const QString &message) {
        qDebug() << "[React UI]" << message;
    }
};
