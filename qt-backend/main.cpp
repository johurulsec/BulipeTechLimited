#include <QApplication>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QVBoxLayout>
#include "bridge.h"
#include <QScreen>
QWebEngineView* contentView = nullptr;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowFlags(Qt::FramelessWindowHint);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    mainWindow.resize(screenGeometry.width(), screenGeometry.height());
    mainWindow.move(screenGeometry.topLeft());

    // React UI WebEngineView
    QWebEngineView* reactView = new QWebEngineView(&mainWindow);
    reactView->resize(mainWindow.size());
    // reactView->resize(1200, 800);
    reactView->show();

    // Web page content view (separate)
    contentView = new QWebEngineView(&mainWindow);
    contentView->setStyleSheet("background: white;");
    contentView->setVisible(false); // Initially hidden
    // contentView->setVisible(true); // test purpose

    // QWebChannel setup
    QWebChannel* channel = new QWebChannel();
    Bridge* bridge = new Bridge();



    channel->registerObject(QStringLiteral("bridge"), bridge);
    reactView->page()->setWebChannel(channel);

    reactView->setUrl(QUrl("https://qt-web-app.surge.sh/")); // Your React build

    QObject::connect(bridge, &Bridge::requestMinimize, [&mainWindow]() {
        mainWindow.showMinimized();
    });

    // QObject::connect(bridge, &Bridge::requestMaximize, [&mainWindow]() {
    //     if (mainWindow.isMaximized())
    //         mainWindow.showNormal();
    //     else
    //         mainWindow.showMaximized();
    // });

    QObject::connect(bridge, &Bridge::requestMaximize, [&mainWindow]() {
        if (mainWindow.isMaximized())
            mainWindow.showNormal();
        else
            mainWindow.showMaximized();
    });

    QObject::connect(bridge, &Bridge::requestClose, [&mainWindow]() {
        mainWindow.close();
    });

    // 🌐 Load real web content in contentView
    QObject::connect(bridge, &Bridge::requestLoadUrl, [=](const QString& url) {
        QUrl qurl = QUrl::fromUserInput(url);
        if (!qurl.isValid()) return;

        contentView->setUrl(qurl);
        contentView->setVisible(true);

        // Run JS to get the position of webview-slot
        reactView->page()->runJavaScript(R"(
            (function() {
                var el = document.getElementById('webview-slot');
                if (!el) return null;
                var rect = el.getBoundingClientRect();
                return {
                    top: rect.top,
                    left: rect.left,
                    width: rect.width,
                    height: rect.height
                };
            })()
        )", [=](const QVariant &result) {
                                             if (!result.isValid()) return;
                                             QVariantMap rect = result.toMap();
                                             int x = rect["left"].toInt();
                                             int y = rect["top"].toInt();
                                             int width = rect["width"].toInt();
                                             int height = rect["height"].toInt();

                                             contentView->setGeometry(x, y, width, height);
                                             contentView->raise();
                                         });
    });

    QObject::connect(reactView, &QWebEngineView::titleChanged, bridge, &Bridge::updateTitle);

    // Re-position webview on resize


    mainWindow.show();
    return app.exec();
}
