#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

#include "bridge.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    qDebug()<<"MW constructor";

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    mainLayout->setSpacing(5);

    // Setup top bar
    QWidget *topBar = createTopBar();
    profile = QWebEngineProfile::defaultProfile();  // make sure to move this from addNewTab()
    mainLayout->addWidget(topBar);

    // Tab widget
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    mainLayout->addWidget(tabWidget);

    // "+" tab
    QWidget *plusTab = new QWidget;
    tabWidget->addTab(plusTab, "+");

    // Connections
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    // Finalize
    setCentralWidget(central);

    //DownloadManager
    setupDownloadManager();

    // Menu Bar
    menuBar = new QMenuBar(this);
    historyMenu = new QMenu("History", this);
    menuBar->addMenu(historyMenu);
    setMenuBar(menuBar);

    bookmarksMenu = new QMenu("Bookmarks", this);
    qDebug()<<"createTopBar() step-1";
    // If you use menuBar, you can add:
    menuBar->addMenu(bookmarksMenu);

    QSettings settings("BulipeTech", "BulipeBrowser");
    historyList = settings.value("history").toStringList();
    //updateHistoryMenu();

    bookmarksList = settings.value("bookmarks").toStringList();
    updateBookmarksMenu();

    QAction *clearHistoryAction = new QAction("Clear History", this);
    connect(clearHistoryAction, &QAction::triggered, this, [this]() {
        historyList.clear();
        updateHistoryMenu();
    });

    historyMenu->addSeparator();
    historyMenu->addAction(clearHistoryAction);

    addNewTab(QUrl("qrc:/html/home.html")); // Start with 1 tab

    setupHistoryDock();

}

MainWindow::~MainWindow()
{
    qDebug()<<"Destructor of ~MainWindow()";

    QSettings settings("BulipeTech", "BulipeBrowser");
    settings.setValue("history", historyList);

    //delete ui;
}

QWidget *MainWindow::createTopBar() {
    qDebug()<<"createTopBar()";

    QWidget *topBar = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(topBar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    backButton = new QPushButton("<", this);
    forwardButton = new QPushButton(">", this);
    reloadButton = new QPushButton("⟳", this);
    urlLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("Search", this);

    QPushButton *downloadsButton = new QPushButton("Downloads", this);
    layout->addWidget(downloadsButton);
    connect(downloadsButton, &QPushButton::clicked, this, [this]() {
        downloadDock->setVisible(!downloadDock->isVisible());
    });


    // Three-dot menu
    menuButton = new QPushButton("⋮", this);
    dropdownMenu = new QMenu(this);
    menuButton->setMenu(dropdownMenu); // attaches menu to the button
    // Add stretch to push the menu button to the right
    layout->addStretch();

    bookmarkStarButton = new QPushButton("⭐", this);
    layout->addWidget(bookmarkStarButton);

    layout->addWidget(menuButton);

    QAction *historyAction = new QAction("History", this);
    QAction *clearHistoryAction = new QAction("Clear History", this);

    dropdownMenu->addAction(historyAction);
    dropdownMenu->addAction(clearHistoryAction);

    connect(historyAction, &QAction::triggered, this, [this]() {
        if (historyDock) {
            historyDock->setVisible(!historyDock->isVisible()); // Toggle sidebar
        }
    });

    connect(clearHistoryAction, &QAction::triggered, this, [this]() {
        historyList.clear();
        updateHistoryMenu();
    });


    // --- end Three-dot menu

    // bookmarks start

    QAction *bookmarkPageAction = new QAction("Bookmark This Page", this);
    QAction *showBookmarksAction = new QAction("Show Bookmarks", this);
    QAction *clearBookmarksAction = new QAction("Clear Bookmarks", this);

    dropdownMenu->addAction(bookmarkPageAction);
    dropdownMenu->addAction(showBookmarksAction);
    dropdownMenu->addAction(clearBookmarksAction);

    connect(bookmarkPageAction, &QAction::triggered, this, [this]() {
        if (auto view = currentWebView()) {
            addToBookmarks(view->url());
        }
    });

    connect(showBookmarksAction, &QAction::triggered, this, [this]() {
        bookmarksMenu->popup(QCursor::pos()); // Show at cursor
    });

    connect(clearBookmarksAction, &QAction::triggered, this, [this]() {
        bookmarksList.clear();
        updateBookmarksMenu();
    });

    connect(bookmarkStarButton, &QPushButton::clicked, this, [this]() {
        if (auto view = currentWebView()) {
            addToBookmarks(view->url());
        }
    });

// bookmarks end

    layout->addWidget(backButton);
    layout->addWidget(forwardButton);
    layout->addWidget(reloadButton);
    layout->addWidget(urlLineEdit, 1);
    layout->addWidget(searchButton);

    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(forwardButton, &QPushButton::clicked, this, &MainWindow::onForwardClicked);
    connect(reloadButton, &QPushButton::clicked, this, &MainWindow::onReloadClicked);
    connect(urlLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onUrlEntered);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    // // Connect download handler
    // webView = new QWebEngineView(this);
    // // setCentralWidget(webView);

    // webView->page()->profile()->setDownloadPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    // connect(webView->page()->profile(), &QWebEngineProfile::downloadRequested,
    //         this, &MainWindow::handleDownloadRequested);

    return topBar;
}

//original

/*
void MainWindow::addNewTab(const QUrl &url) {
    qDebug() << "addNewTab()";

    QWebEngineView *view = new QWebEngineView;

    profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                              "AppleWebKit/537.36 (KHTML, like Gecko) "
                              "Chrome/119.0.0.0 Safari/537.36");

    // If url is empty, use default custom home page
    QUrl loadUrl = url.isEmpty() ? QUrl("qrc:/html/home.html") : url;

    view->load(loadUrl);

    int insertIndex = tabWidget->count() - 1;
    tabWidget->insertTab(insertIndex, view, "New Tab");
    tabWidget->setCurrentIndex(insertIndex);

    connect(view, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        int idx = tabWidget->indexOf(view);
        if (idx != -1)
            tabWidget->setTabText(idx, title);
    });

    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::onUrlChanged);
}
*/

//2nd way

/*
void MainWindow::addNewTab(const QUrl &url) {
    QWebEngineView *view = new QWebEngineView;

    Bridge *bridge = new Bridge(this);
    QWebChannel *channel = new QWebChannel(view->page());
    channel->registerObject("bridge", bridge);
    view->page()->setWebChannel(channel);

    connect(bridge, &Bridge::searchRequested, this, [this, view](const QString &query) {
        QString targetUrl;
        if (query.contains('.') && !query.contains(' ')) {
            targetUrl = query.startsWith("http") ? query : "https://" + query;
        } else {
            targetUrl = "https://www.google.com/search?q=" + QUrl::toPercentEncoding(query);
        }

        view->load(QUrl(targetUrl));
    });

    QUrl loadUrl = url.isEmpty() ? QUrl("qrc:/html/home.html") : url;
    view->load(loadUrl);

    int insertIndex = tabWidget->count() - 1;
    tabWidget->insertTab(insertIndex, view, "New Tab");
    tabWidget->setCurrentIndex(insertIndex);

    connect(view, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        int idx = tabWidget->indexOf(view);
        if (idx != -1)
            tabWidget->setTabText(idx, title);
    });

    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::onUrlChanged);
}
*/

//3rd way

void MainWindow::addNewTab(const QUrl &url) {
    QWebEngineView *view = new QWebEngineView;

    qDebug()<<"addNweTab(), url:"<<url.toString();

    QUrl loadUrl = url.isEmpty() ? QUrl("qrc:/html/home.html") : url;
    view->load(loadUrl);

    if (loadUrl.toString().contains("home.html")) {
        QWebChannel *channel = new QWebChannel(view);
        BrowserInterface *bridge = new BrowserInterface(this);
        channel->registerObject("browserInterface", bridge);
        view->page()->setWebChannel(channel);
    }

    int index = tabWidget->count() - 1;
    tabWidget->insertTab(index, view, "New Tab");
    tabWidget->setCurrentIndex(index);

    connect(view, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        tabWidget->setTabText(tabWidget->indexOf(view), title);
    });

    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::onUrlChanged);
}

// void MainWindow::onTabChanged(int index) {
//     qDebug() << "onTabChanged()";

//     if (tabWidget->tabText(index) == "+") {
//         addNewTab(QUrl("qrc:/html/home.html"));  // 👈 Load custom home page
//         tabWidget->setCurrentIndex(tabWidget->count() - 2); // Switch to the new tab
//         return;
//     }

//     if (auto view = currentWebView()) {
//         urlLineEdit->setText(view->url().toString());
//     }
// }


QWebEngineView* MainWindow::currentWebView() const {
    qDebug()<<"currentWebView() const";

    return qobject_cast<QWebEngineView*>(tabWidget->currentWidget());
}

void MainWindow::onBackClicked() {
    qDebug()<<"onBackClicked()";

    if (auto view = currentWebView()) view->back();
}

void MainWindow::onForwardClicked() {
    qDebug()<<"onForwardClicked()";

    if (auto view = currentWebView()) view->forward();
}

void MainWindow::onReloadClicked() {
    qDebug()<<"onReloadClicked()";

    if (auto view = currentWebView()) view->reload();
}

void MainWindow::onUrlEntered() {

    QUrl url = QUrl::fromUserInput(urlLineEdit->text());
    qDebug()<<"onUrlEntered(), url:"<<url;

    if (auto view = currentWebView()) view->load(url);
}

void MainWindow::onSearchClicked() {

    QUrl url = QUrl::fromUserInput(urlLineEdit->text());
    qDebug()<<"onSearchClicked(), url:"<<url;

    if (auto view = currentWebView()) view->load(url);
}

void MainWindow::onUrlChanged(const QUrl &url) {
    qDebug()<<"onUrlChanged(), url =>"<<url.toString();

    urlLineEdit->setText(url.toString());

    addToHistory(url);
}


void MainWindow::onTabChanged(int index) {
    if (tabWidget->tabText(index) == "+") {
        addNewTab(QUrl("qrc:/html/home.html"));
        tabWidget->setCurrentIndex(tabWidget->count() - 2);
        return;
    }

    if (auto view = currentWebView()) {
        urlLineEdit->setText(view->url().toString());
    }
}


void MainWindow::closeTab(int index) {
    qDebug()<<"closeTab()";

    if (tabWidget->tabText(index) == "+") return;
    QWidget *tab = tabWidget->widget(index);
    tabWidget->removeTab(index);
    delete tab;
}


void MainWindow::addToHistory(const QUrl &url) {
    qDebug()<<"addToHistory()";

    QString urlStr = url.toString();
    if (!historyList.contains(urlStr)) {
        historyList.prepend(urlStr); // Most recent on top
        if (historyList.size() > 50) historyList.removeLast(); // Limit size
        updateHistoryMenu();
    }
}


// void MainWindow::updateHistoryMenu() {
//     historyMenu->clear();

//     for (const QString &urlStr : historyList) {
//         QAction *action = new QAction(urlStr, this);
//         connect(action, &QAction::triggered, this, [this, urlStr]() {
//             QUrl url = QUrl::fromUserInput(urlStr);
//             if (auto view = currentWebView()) {
//                 view->load(url);
//             }
//         });
//         historyMenu->addAction(action);
//     }
// }

void MainWindow::updateHistoryMenu() {

    qDebug()<<"updateHistoryMenu()";

    historyMenu->clear();
    if (historyListWidget) {
        historyListWidget->clear();
    }

    for (const QString &urlStr : historyList) {
        QAction *action = new QAction(urlStr, this);
        connect(action, &QAction::triggered, this, [this, urlStr]() {
            QUrl url = QUrl::fromUserInput(urlStr);
            if (auto view = currentWebView()) {
                view->load(url);
            }
        });
        historyMenu->addAction(action);

        if (historyListWidget) {
            historyListWidget->addItem(urlStr);
        }
    }

    historyMenu->addSeparator();
    QAction *clearHistoryAction = new QAction("Clear History", this);
    connect(clearHistoryAction, &QAction::triggered, this, [this]() {
        historyList.clear();
        updateHistoryMenu();
    });
    historyMenu->addAction(clearHistoryAction);
}



void MainWindow::setupHistoryDock() {

    historyDock = new QDockWidget("Browsing History", this);
    historyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    historyListWidget = new QListWidget(historyDock);
    historyDock->setWidget(historyListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, historyDock);

    connect(historyListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        QUrl url = QUrl::fromUserInput(item->text());
        if (auto view = currentWebView()) view->load(url);
    });

    // Fill from existing history
    historyListWidget->addItems(historyList);
}


void MainWindow::addToBookmarks(const QUrl &url) {
    qDebug()<<"addToBookmarks()";

    QString urlStr = url.toString();
    if (!bookmarksList.contains(urlStr)) {
        bookmarksList.prepend(urlStr);
        if (bookmarksList.size() > 100) bookmarksList.removeLast();
        updateBookmarksMenu();
    }
}

void MainWindow::updateBookmarksMenu() {
    qDebug()<<"updateBookmarksMenu()";

    if (!bookmarksMenu) return;

    bookmarksMenu->clear();

    for (const QString &urlStr : bookmarksList) {
        QAction *action = new QAction(urlStr, this);
        connect(action, &QAction::triggered, this, [this, urlStr]() {
            if (auto view = currentWebView()) {
                view->load(QUrl::fromUserInput(urlStr));
            }
        });
        bookmarksMenu->addAction(action);
    }

    // Save to settings
    QSettings settings("BulipeTech", "BulipeBrowser");
    settings.setValue("bookmarks", bookmarksList);
}


void MainWindow::setupDownloadManager() {
    qDebug()<<"setupDownloadManager()";

    downloadDock = new QDockWidget("Download Manager", this);
    downloadDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    downloadListWidget = new QListWidget(downloadDock);
    downloadDock->setWidget(downloadListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, downloadDock);
    downloadDock->setVisible(false); // initially hidden

    // Connect global profile to capture downloads
    connect(profile, &QWebEngineProfile::downloadRequested,
            this, &MainWindow::handleDownload);
}


void MainWindow::handleDownload(QWebEngineDownloadRequest *download) {
    qDebug()<<"handleDownload()";

    QString filename = download->downloadFileName();
    QString fullPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + filename;
    download->setDownloadDirectory(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    download->setDownloadFileName(filename);
    download->accept(); // start download

    QListWidgetItem *item = new QListWidgetItem(QString("⬇ %1").arg(filename));
    downloadListWidget->addItem(item);
    downloadDock->setVisible(true);

    // Create and attach progress bar
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    downloadListWidget->setItemWidget(item, progressBar);
    downloadProgressBars[download] = progressBar;

    connect(download, &QWebEngineDownloadRequest::totalBytesChanged, this, [=]() {
        if (download->totalBytes() > 0) {
            int percent = static_cast<int>((download->receivedBytes() * 100) / download->totalBytes());
            progressBar->setValue(percent);
        }
    });

    connect(download, &QWebEngineDownloadRequest::stateChanged, this, [=]() {
        progressBar->setValue(100);
        item->setText(QString("✅ %1 - Completed").arg(filename));
    });

    connect(download, &QObject::destroyed, this, [=]() {
        downloadProgressBars.remove(download);
    });

    // if (download->state() != QWebEngineDownloadRequest::DownloadCompleted) {
    //     QMessageBox::warning(this, "Download Failed",
    //                          QString("Failed to download %1.\nReason: %2")
    //                              .arg(filename)
    //                              .arg(download->interruptReasonString()));
    // }

    // if (download->state() == QWebEngineDownloadRequest::DownloadCompleted) {
    //     QMessageBox::warning(this, "Download Completed",
    //                          QString("Completed to download %1.\nReason: %2")
    //                              .arg(filename)
    //                              .arg(download->interruptReasonString()));
    // }


}


void MainWindow::handleSearchFromHomePage(const QString &query) {
    QUrl searchUrl;

    if (query.contains('.') && !query.contains(' ')) {
        QString urlString = query.startsWith("http") ? query : "https://" + query;
        searchUrl = QUrl::fromUserInput(urlString);
    } else {
        searchUrl = QUrl("https://www.google.com/search?q=" + QUrl::toPercentEncoding(query));
    }

    addNewTab(searchUrl);
}



// void MainWindow::handleDownloadRequested(QWebEngineDownloadRequest *download)
// {
//     qDebug()<<"handleDownloadRequested()";

//     // Ask user for download location
//     QString defaultFileName = QFileInfo(download->downloadFileName()).fileName();
//     QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
//                                                     QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + defaultFileName);

//     if (filePath.isEmpty()) {
//         download->cancel();
//         return;
//     }

//     download->setDownloadFileName(filePath);
//     download->accept();

//     // // Connect signals to track download progress
//     // connect(download, &QWebEngineDownloadRequest::downloadProgress, [this, download](qint64 bytesReceived, qint64 bytesTotal) {
//     //     qDebug() << "Download progress:" << bytesReceived << "of" << bytesTotal;
//     //     // Update your UI here (e.g., progress bar)
//     // });

//     // connect(download, &QWebEngineDownloadRequest::finished, [this, download]() {
//     //     if (download->state() == QWebEngineDownloadRequest::DownloadCompleted) {
//     //         QMessageBox::information(this, tr("Download Complete"),
//     //                                  tr("File has been downloaded successfully."));
//     //     } else {
//     //         QMessageBox::warning(this, tr("Download Failed"),
//     //                              tr("File download failed or was cancelled."));
//     //     }
//     //     download->deleteLater();
//     // });
// }



