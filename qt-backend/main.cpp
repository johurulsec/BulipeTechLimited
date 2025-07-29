#include <QApplication>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QVBoxLayout>
#include "bridge.h"
#include <QScreen>
#include<QResizeEvent>
#include<QWebEngineProfile>
QWebEngineView* contentView = nullptr;

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    mainWindow.resize(screenGeometry.width(), screenGeometry.height());
    mainWindow.move(screenGeometry.topLeft());

    // React UI WebEngineView
    QWebEngineView* reactView = new QWebEngineView(&mainWindow);
    reactView->resize(mainWindow.size());
    reactView->show();

    // Web page content view (separate)
    contentView = new QWebEngineView(&mainWindow);
    contentView->setStyleSheet("background: white;");
    contentView->setVisible(false); // Initially hidden

    mainWindow.setReactView(reactView);
    mainWindow.setContentView(contentView);

    // QWebChannel setup
    QWebChannel* channel = new QWebChannel();
    Bridge* bridge = new Bridge();

    channel->registerObject(QStringLiteral("bridge"), bridge);
    reactView->page()->setWebChannel(channel);

    // QWebEngineProfile *profile;
    // profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
    //                           "(KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36");

    // reactView->setUrl(QUrl("https://qt-web-app.surge.sh/")); // React build
    // reactView->setUrl(QUrl("https://qt-web-app2.surge.sh/"));
    reactView->setUrl(QUrl("http://localhost:5173/"));// React build

    QObject::connect(bridge, &Bridge::requestMinimize, [&mainWindow]() {
        mainWindow.showMinimized();
    });

    QObject::connect(bridge, &Bridge::requestMaximize, [&mainWindow]() {
        if (mainWindow.isMaximized())
            mainWindow.showNormal();
        else
            mainWindow.showMaximized();
    });

    QObject::connect(bridge, &Bridge::requestClose, [&mainWindow]() {
        mainWindow.close();
    });

    // Load real web content in contentView
    QObject::connect(bridge, &Bridge::requestLoadUrl, [=](const QString& url) {
        QUrl qurl = QUrl::fromUserInput(url);
        if (!qurl.isValid() || url.isEmpty()) {
            contentView->setVisible(false);
            qDebug()<<"Empty or invalid url detected"<<qurl;
            return;
        }

        // // Set user agent
        // contentView->page()->profile()->setHttpUserAgent(
        //     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        //     "(KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36"
        //     );

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

    mainWindow.show();

    return app.exec();
}

