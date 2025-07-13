#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFile file(":/style.qss");  // or just "style.qss" if not using .qrc
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);  // ✅ FIXED
    } else {
        qDebug() << "Failed to load stylesheet.";
    }
    MainWindow w;
    w.show();
    return a.exec();
}
