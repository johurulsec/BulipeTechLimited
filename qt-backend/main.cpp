//main.cpp
#include <QApplication>
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QScreen>
#include <QDebug>

#include "bridge.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "main.cpp start ...";

    MainWindow mainWindow;

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    mainWindow.resize(screenGeometry.width(), screenGeometry.height());
    mainWindow.move(screenGeometry.topLeft());

    // React UI WebEngineView
    QWebEngineView* reactView = new QWebEngineView(&mainWindow);
    reactView->resize(mainWindow.size());
    reactView->show();

    mainWindow.setReactView(reactView);

    // QWebChannel setup
    QWebChannel* channel = new QWebChannel();
    Bridge* bridge = new Bridge();

    channel->registerObject(QStringLiteral("bridge"), bridge);
    reactView->page()->setWebChannel(channel);

    // Connect Bridge routing signals to MainWindow slots
    QObject::connect(bridge, &Bridge::requestCreateTab, &mainWindow, &MainWindow::createTab);
    QObject::connect(bridge, &Bridge::requestCloseTab, &mainWindow, &MainWindow::closeTab);
    QObject::connect(bridge, &Bridge::requestSetCurrentTab, &mainWindow, &MainWindow::setCurrentTab);
    QObject::connect(bridge, &Bridge::requestLoadUrlForTab, &mainWindow, &MainWindow::loadUrlForTab);
    QObject::connect(bridge, &Bridge::requestGoBackForTab, &mainWindow, &MainWindow::goBackForTab);
    QObject::connect(bridge, &Bridge::requestGoForwardForTab, &mainWindow, &MainWindow::goForwardForTab);
    QObject::connect(bridge, &Bridge::requestReloadForTab, &mainWindow, &MainWindow::reloadForTab);

    // Forward new-tab requests coming from C++ pages to React via Bridge
    QObject::connect(&mainWindow, &MainWindow::newTabRequested, bridge, &Bridge::sendOpenInNewTab);

    //for back/forward button press and tabBar url change
    QObject::connect(&mainWindow, &MainWindow::urlChangedForTab, bridge, &Bridge::updateTabUrl);

    // Load React app
    reactView->setUrl(QUrl("http://localhost:5173/"));

    // Create initial tab 0 and set current
    bridge->createTab(0);
    bridge->setCurrentTab(0);

    QObject::connect(bridge, &Bridge::requestMinimize, [&mainWindow]() {
        qDebug()<<"mainWindow.showMinimized() of main()";
        mainWindow.showMinimized();
    });

    QObject::connect(bridge, &Bridge::requestMaximize, [&mainWindow]() {
        if (mainWindow.isMaximized()){
            qDebug()<<"isMaximized() showNormal() of main()";
            mainWindow.showNormal();
        }
        else{
            qDebug()<<"isNormal() showMaximized of main()";
            mainWindow.showMaximized();
        }
    });

    QObject::connect(bridge, &Bridge::requestClose, [&mainWindow]() {
        qDebug()<<"mainWindow.close() of main()";
        mainWindow.close();
    });

    mainWindow.show();
    return app.exec();
}

/*
//main.cpp
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
#include<QWebEngineSettings>
#include "custompage.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug()<<"main.cpp start ...";
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
    reactView->page()->setWebChannel(channel);//webchannel setup

    // for back, forward, reload purposes
    bridge->setContentView(contentView);

    // for custompage in newTab open
    auto contentPage = new CustomPage(contentView);
    contentView->setPage(contentPage);

    QObject::connect(contentPage, &CustomPage::newTabRequested, [bridge](const QUrl& url) {
        qDebug() << "New tab requested for URL:" << url;
        // Send this to React to add new tab
        if (bridge) {
            bridge->sendOpenInNewTab(url.toString());
        }
    });


    QObject::connect(reactView, &QWebEngineView::loadFinished, [](bool ok) {
        if (ok)
            qDebug() << "React UI loaded of main()";
        else
            qDebug() << "Failed to load React UI of main()";
    });

    // reactView->setUrl(QUrl("https://qt-web-app.surge.sh/")); // React build
    // reactView->setUrl(QUrl("https://qt-web-app2.surge.sh/"));
    reactView->setUrl(QUrl("http://localhost:5173/"));// React build

    QObject::connect(bridge, &Bridge::requestMinimize, [&mainWindow]() {
        mainWindow.showMinimized();
    });

    QObject::connect(bridge, &Bridge::requestMaximize, [&mainWindow]() {
        if (mainWindow.isMaximized()){
            qDebug()<<"isMaximized() showNormal() of main()";
            mainWindow.showNormal();
        }
        else{
            qDebug()<<"isNormal() showMaximized of main()";
            mainWindow.showMaximized();
        }
    });

    QObject::connect(bridge, &Bridge::requestClose, [&mainWindow]() {
        mainWindow.close();
    });

    // Load real web content in contentView
    QObject::connect(bridge, &Bridge::requestLoadUrl, [=](const QString& url) {
        qDebug() << "Received URL request of main.cpp :" << url;

        QUrl qurl = QUrl::fromUserInput(url);
        if (!qurl.isValid() || url.isEmpty()) {
            contentView->setVisible(false);
            qDebug()<<"Empty or invalid url detected of main.cpp class! so return"<<qurl;
            return;
        }else{
            qDebug()<<"valid url of main.cpp :"<<qurl.toString();
        }

        // if (qurl.scheme() == "https" && qurl.host() == "web.whatsapp.com"){
        //     qDebug()<<"https://web.whatsapp.com/ detected so, set user agent";

        //     // Set user agent for whatsapp-web but not work gemini-chatbot!
        //     contentView->page()->profile()->setHttpUserAgent(
        //         "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
        //         "(KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0"
        //         );
        // }

        // // Enable all necessary features
        // QWebEngineSettings* settings = contentView->page()->settings();
        // settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
        // settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        // settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        // settings->setAttribute(QWebEngineSettings::WebAttribute::AllowRunningInsecureContent, true);

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
                                             if (!result.isValid()){
                                                 qDebug() << "Failed to get slot position from React of main.cpp. so return";
                                                 return;
                                             }
                                             QVariantMap rect = result.toMap();
                                             qDebug() << "Webview slot position main.cpp :" << rect;

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
*/

