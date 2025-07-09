#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QDebug>

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

    addNewTab(); // Start with 1 tab

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

    //download menu start
    downloadsMenu = new QMenu("Downloads", this);

    QAction *showDownloadsMenuAction = new QAction("Show Downloads", this);
    dropdownMenu->addAction(showDownloadsMenuAction);
    connect(showDownloadsMenuAction, &QAction::triggered, this, [this]() {
        downloadsMenu->popup(QCursor::pos());
    });

    //download menu end

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

    return topBar;
}

void MainWindow::addNewTab(const QUrl &url) {
    qDebug()<<"addNewTab() for this url :"<<url.toString();

    QWebEngineView *view = new QWebEngineView;

    //profile = view->page()->profile();
    profile->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
                              "(KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36");


    view->load(url);

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
    qDebug()<<"onUrlEntered(), url:"<<url.toString();
    // QString tmp_url = url.toString();
    // QString sub_str = ".com";
    // if (tmp_url.contains(sub_str)) {
    //     qDebug() << "Substring .com found!";
    // } else {
    //     qDebug() << "Substring .com not found.";
    //     tmp_url += sub_str;

    //     if(QUrl::fromUserInput(tmp_url).isValid()){
    //         qDebug() << "url is valid";
    //         url = QUrl::fromUserInput(tmp_url);
    //     }else{
    //         qDebug() << "url is not valid.";
    //     }

    // }

    if (auto view = currentWebView()){
        view->load(url);
    }
}


//2nd way

/*
void MainWindow::onUrlEntered() {

    QUrl url2 = QUrl::fromUserInput(urlLineEdit->text());
    qDebug()<<"onUrlEntered(), url:"<<url2.toString();

    QString input = urlLineEdit->text().trimmed();
    QStringList domainExtensions = { ".com", ".net", ".org" };
    QUrl finalUrl;
    bool loaded = false;

    // Check if it's already a valid URL with a scheme
    QUrl url = QUrl::fromUserInput(input);
    if (url.isValid() && url.scheme().startsWith("http")) {
        finalUrl = url;
        loaded = true;
        qDebug() << "Input is valid URL with scheme.";
    } else {
        // Try appending common domain extensions
        for (const QString &ext : domainExtensions) {
            QString tryUrlStr = input;
            if (!input.contains(ext)) {
                tryUrlStr += ext;
            }

            QUrl tryUrl = QUrl::fromUserInput(tryUrlStr);
            if (tryUrl.isValid()) {
                qDebug() << "Trying with extension:" << ext << "->" << tryUrl;
                finalUrl = tryUrl;
                loaded = true;
                break;
            }
        }
    }

    // If no valid URL, treat it as search query (Google fallback)
    if (!loaded) {
        QString query = QUrl::toPercentEncoding(input);
        finalUrl = QUrl("https://www.google.com/search?q=" + query);
        qDebug() << "Fallback to Google Search:" << finalUrl;
    }

    // Load finalUrl
    if (auto view = currentWebView()) {
        view->load(finalUrl);
    }
}
*/

//3rd way

/*
void MainWindow::onUrlEntered() {
    QString input = urlLineEdit->text().trimmed();

    QStringList prefixes = {
        "https://", "http://", "https://www.", "http://www."
    };
    QStringList extensions = {
        "", ".com", ".net", ".org", ".bd", ".info"
    };

    QUrl finalUrl;
    bool found = false;

    // Case 1: check if input is a valid URL directly
    QUrl initialUrl = QUrl::fromUserInput(input);
    if (initialUrl.isValid() && initialUrl.scheme().startsWith("http")) {
        finalUrl = initialUrl;
        found = true;
        qDebug() << "Direct valid URL:" << finalUrl.toString();
    } else {
        // Case 2: Try combinations of prefixes and extensions
        for (const QString &prefix : prefixes) {
            for (const QString &ext : extensions) {
                QString tryStr = input;

                if (!input.endsWith(ext) && !ext.isEmpty()) {
                    tryStr += ext;
                }

                QString fullTry = prefix + tryStr;
                QUrl tryUrl(fullTry);

                if (tryUrl.isValid()) {
                    qDebug() << "Trying:" << tryUrl;
                    finalUrl = tryUrl;
                    found = true;
                    goto LOAD;
                }
            }
        }
    }

    // Case 3: Fallback to search engine
    if (!found) {
        QString query = QUrl::toPercentEncoding(input);
        finalUrl = QUrl("https://www.google.com/search?q=" + query);
        qDebug() << "Fallback to search:" << finalUrl;
    }

LOAD:
    urlLineEdit->setText(finalUrl.toString());
    if (auto view = currentWebView()) {
        qDebug()<<"before load url ...";
        view->load(finalUrl);
    }
}
*/


/*
void MainWindow::onUrlEntered() {

    //     QUrl url = QUrl::fromUserInput(urlLineEdit->text());
    //     qDebug()<<"onUrlEntered(), url:"<<url.toString();
    //     QString tmp_url = url.toString();
    //     QString sub_str = ".com";
    //     if (tmp_url.contains(sub_str)) {
    //         qDebug() << "Substring .com found!";
    //     } else {
    //         qDebug() << "Substring .com not found.";
    //         tmp_url += sub_str;

    //         if(QUrl::fromUserInput(tmp_url).isValid()){
    //             qDebug() << "url is valid";
    //             url = QUrl::fromUserInput(tmp_url);
    //         }else{
    //             qDebug() << "url is not valid.";
    //         }

    //     }

    //     if (auto view = currentWebView()){
    //         view->load(url);
    //     }

    QString input = urlLineEdit->text().trimmed();

    // QStringList prefixes = {"https://", "http://", "https://www.", "http://www."};
    // QStringList extensions = {"", ".com", ".net", ".org", ".bd", ".info"};

    QStringList prefixes = {"https://www."};
    QStringList extensions = {"", ".com"};

    QStringList tryUrls;

    // Generate all URL combinations
    for (const QString &prefix : prefixes) {
        for (const QString &ext : extensions) {
            QString tryStr = input;
            if (!ext.isEmpty() && !tryStr.endsWith(ext))
                tryStr += ext;

            QString fullUrlStr = prefix + tryStr;
            QUrl url(fullUrlStr);
            if (url.isValid()) {
                tryUrls << url.toString();
                qDebug()<<"generated url :"<<url.toString();
            }
        }
    }

    // Now, check each one via QNetworkAccessManager
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    std::function<void(int)> tryNextUrl;
    tryNextUrl = [=](int index) mutable {
        qDebug() <<"tryUrls.size() :"<<tryUrls.size()<<", index:"<<index;

        if (index >= tryUrls.size()) {
            qDebug() << "index >= tryUrls.size()";

            // Fallback: Google search
            QString query = QUrl::toPercentEncoding(input);
            QUrl fallback("https://www.google.com/search?q=" + query);
            qDebug() << "All attempts failed. Loading fallback:" << fallback;
            urlLineEdit->setText(fallback.toString());
            if (auto view = currentWebView()) {
                view->load(fallback);
            }
            qDebug() << "before return!";
            return;
        }

        QUrl tryUrl(tryUrls[index]);
        QNetworkRequest request(tryUrl);
        QNetworkReply *reply = manager->head(request);

        QObject::connect(reply, &QNetworkReply::finished, this, [=]() mutable {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug() << "statusCode :"<<statusCode<<" statuscode.toInt():"<<statusCode.toInt();

            if (reply->error() == QNetworkReply::NoError &&
                statusCode.isValid() &&
                statusCode.toInt() >= 200 && statusCode.toInt() < 400) {
                // Valid URL, load it
                qDebug() << "Valid URL found:" << tryUrl.toString();
                urlLineEdit->setText(tryUrl.toString());
                if (auto view = currentWebView()) {
                    view->load(tryUrl);
                }
                reply->deleteLater();
                manager->deleteLater();
                return;

            } else {
                index +=1;
                qDebug() << "Invalid URL found:" << tryUrl.toString()<<", next index:"<<index;
                reply->deleteLater();
                // tryNextUrl(index + 1);
                index +=1;
                tryNextUrl(index);

            }
        });
    };

    tryNextUrl(0); // Start testing from first URL
}
*/


//4rt way

/*
void MainWindow::onUrlEntered() {
    //first step

    QStringList tryUrls;
    QUrl rawUrl = QUrl::fromUserInput(urlLineEdit->text());
    qDebug()<<"onUrlEntered(), rawUrl:"<<rawUrl.toString();

    if (rawUrl.isValid()) {
        tryUrls << rawUrl.toString();
    }

    //second step

    QString input = urlLineEdit->text().trimmed();
    qDebug()<<"user url input :"<<input;

    QStringList prefixes = {"https://", "http://", "https://www.", "http://www."};
    QStringList extensions = {"", ".com", ".net", ".org", ".bd", ".info"};

    // Generate all URL combinations
    for (const QString &prefix : prefixes) {
        for (const QString &ext : extensions) {
            QString tryStr = input;
            if (!ext.isEmpty() && !tryStr.endsWith(ext))
                tryStr += ext;

            QString fullUrlStr = prefix + tryStr;
            QUrl url(fullUrlStr);
            if (url.isValid()) {
                tryUrls << url.toString();
                qDebug()<<"generated url :"<<url.toString();
            }
        }
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Use shared_ptr to capture lambda recursively
    auto tryNextUrl = std::make_shared<std::function<void(int)>>();

    *tryNextUrl = [=](int index) mutable {
        qDebug() <<"tryUrls.size() :"<<tryUrls.size()<<", index:"<<index;

        if (index >= tryUrls.size()) {
            // Fallback to search
            QString query = QUrl::toPercentEncoding(input);
            QUrl fallback("https://www.google.com/search?q=" + query);
            qDebug() << "All attempts failed. Loading fallback:" << fallback;
            urlLineEdit->setText(fallback.toString());
            if (auto view = currentWebView()) {
                view->load(fallback);
            }
            manager->deleteLater();
            qDebug() << "before return!";
            return;
        }

        QUrl tryUrl(tryUrls[index]);
        QNetworkRequest request(tryUrl);
        QNetworkReply *reply = manager->head(request);

        QObject::connect(reply, &QNetworkReply::finished, this, [=]() mutable {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug() << "statusCode :"<<statusCode<<" statuscode.toInt():"<<statusCode.toInt();

            if (reply->error() == QNetworkReply::NoError &&
                statusCode.isValid() &&
                statusCode.toInt() >= 200 && statusCode.toInt() < 400) {

                qDebug() << "Valid URL found:" << tryUrl;
                urlLineEdit->setText(tryUrl.toString());
                if (auto view = currentWebView()) {
                    view->load(tryUrl);
                }
                reply->deleteLater();
                manager->deleteLater();
            } else {
                qDebug() << "Invalid URL found:" << tryUrl.toString()<<", next index:"<<index+1;

                reply->deleteLater();
                (*tryNextUrl)(index + 1); // recursive call
            }
        });
    };

    // Start from index 0
    (*tryNextUrl)(0);

}
*/


QString MainWindow::stripUrlPrefix(const QString &input)
{
    qDebug()<<"stripUrlPrefix(), user garbage input url :"<<input;

    QString cleaned = input.trimmed();

    // Parse with QUrl (if possible)
    QUrl url = QUrl::fromUserInput(cleaned);
    if (url.isValid() && !url.host().isEmpty()) {
        QString host = url.host();

        // Remove "www." if present
        if (host.startsWith("www."))
            host = host.mid(4);

        return host;
    }

    // Else fallback: manually remove common schemes/prefixes
    cleaned.remove(QRegularExpression("^https?://"));
    cleaned.remove(QRegularExpression("^www\\."));

    qDebug()<<"stripUrlPrefix(), after clean url-core part :"<<cleaned;

    return cleaned;
}

// 5th way

/*
void MainWindow::onUrlEntered() {
    //first step

    // QStringList tryUrls;
    // QUrl rawUrl = QUrl::fromUserInput(urlLineEdit->text());
    // qDebug()<<"onUrlEntered(), rawUrl:"<<rawUrl.toString();

    // QString corePart = stripUrlPrefix(rawUrl.toString()); // ✅ Extract "google" from "http://google"
    // qDebug() << "cleaned input:" << corePart;

    // if (rawUrl.isValid()) {
    //     tryUrls << rawUrl.toString();
    // }

    // //second step

    // QString input = urlLineEdit->text().trimmed();
    // qDebug()<<"user url input :"<<input;

    // QStringList prefixes = {"https://", "http://", "https://www.", "http://www."};
    // QStringList extensions = {"", ".com", ".net", ".org", ".bd", ".info"};

    // // Generate all URL combinations
    // for (const QString &prefix : prefixes) {
    //     for (const QString &ext : extensions) {
    //         QString tryStr = input;
    //         if (!ext.isEmpty() && !tryStr.endsWith(ext))
    //             tryStr += ext;

    //         QString fullUrlStr = prefix + tryStr;
    //         QUrl url(fullUrlStr);
    //         if (url.isValid()) {
    //             tryUrls << url.toString();
    //             qDebug()<<"generated url :"<<url.toString();
    //         }
    //     }
    // }

    QString input = urlLineEdit->text().trimmed();
    qDebug() << "user url input :" << input;

    // 1. Try loading user input directly
    QUrl rawUrl = QUrl::fromUserInput(input);
    qDebug() << "onUrlEntered(), rawUrl:" << rawUrl.toString();

    QString corePart = stripUrlPrefix(input); // ✅ Extract "google" from "http://google"
    qDebug() << "cleaned input:" << corePart;

    QStringList prefixes = {"https://", "http://", "https://www.", "http://www."};
    QStringList extensions = {"", ".com", ".net", ".org", ".bd", ".info"};

    QStringList tryUrls;

    // First try raw input
    if (rawUrl.isValid() && rawUrl.scheme().startsWith("http"))
        tryUrls << rawUrl.toString();

    // Generate alternative URLs
    for (const QString &prefix : prefixes) {
        for (const QString &ext : extensions) {
            QString fullTry = prefix + corePart;
            if (!ext.isEmpty() && !corePart.endsWith(ext))
                fullTry += ext;

            QUrl tryUrl(fullTry);
            if (tryUrl.isValid() && !tryUrls.contains(tryUrl.toString())) {
                tryUrls << tryUrl.toString();
                qDebug() << "generated url :" << tryUrl.toString();
            }
        }
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    // Use shared_ptr to capture lambda recursively
    auto tryNextUrl = std::make_shared<std::function<void(int)>>();

    *tryNextUrl = [=](int index) mutable {
        qDebug() <<"tryUrls.size() :"<<tryUrls.size()<<", index:"<<index;

        if (index >= tryUrls.size()) {
            // Fallback to search
            QString query = QUrl::toPercentEncoding(input);
            QUrl fallback("https://www.google.com/search?q=" + query);
            qDebug() << "All attempts failed. Loading fallback:" << fallback;
            urlLineEdit->setText(fallback.toString());
            if (auto view = currentWebView()) {
                view->load(fallback);
            }
            manager->deleteLater();
            qDebug() << "before return!";
            return;
        }

        QUrl tryUrl(tryUrls[index]);
        QNetworkRequest request(tryUrl);
        QNetworkReply *reply = manager->head(request);

        QObject::connect(reply, &QNetworkReply::finished, this, [=]() mutable {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug() << "statusCode :"<<statusCode<<" statuscode.toInt():"<<statusCode.toInt();

            if (reply->error() == QNetworkReply::NoError &&
                statusCode.isValid() &&
                statusCode.toInt() >= 200 && statusCode.toInt() < 400) {

                qDebug() << "Valid URL found:" << tryUrl;
                urlLineEdit->setText(tryUrl.toString());
                if (auto view = currentWebView()) {
                    view->load(tryUrl);
                }
                reply->deleteLater();
                manager->deleteLater();
            } else {
                qDebug() << "Invalid URL found:" << tryUrl.toString()<<", next index:"<<index+1;

                reply->deleteLater();
                (*tryNextUrl)(index + 1); // recursive call
            }
        });
    };

    // Start from index 0
    (*tryNextUrl)(0);

}
*/


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
    qDebug()<<"onTabChanged()";

    if (tabWidget->tabText(index) == "+") {
        addNewTab();
        return;
    }
    if (auto view = currentWebView()) {
        qDebug()<<"auto view = currentWebView()? Yes";
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

    qDebug()<<"inside handledownload() filename :"<<filename;

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

    QAction *downloadAction = new QAction(QString("⬇ %1").arg(filename), this);
    downloadsMenu->addAction(downloadAction);
    downloadActions[download] = downloadAction;

    // // Default click action: open the file after download finishes
    // connect(downloadAction, &QAction::triggered, this, [=]() {
    //     if (QFile::exists(download->downloadDirectory() + "/" + download->downloadFileName())) {
    //         QDesktopServices::openUrl(QUrl::fromLocalFile(
    //             download->downloadDirectory() + "/" + download->downloadFileName()));
    //     }
    // });

    connect(downloadAction, &QAction::triggered, this, [=]() {
        QString filePath = download->downloadDirectory() + "/" + download->downloadFileName();
        qDebug()<<"pdf filePath :"<<filePath;

        if (QFile::exists(filePath)) {
            qDebug()<<"QFile::exists(filePath)? Yes";

            // QUrl localUrl = QUrl::fromLocalFile(filePath);
            // qDebug()<<"pdf localUrl :"<<localUrl.toString();
            // this->addNewTab(localUrl);  // ✅ Open in your own browser

            QString viewerPath = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/pdfjs/web/viewer.html").toString();
            qDebug()<<"viewerPath :"<<viewerPath;
            QUrl pdfViewerUrl = QUrl(viewerPath + "?file=" + QUrl::fromLocalFile(filePath).toEncoded());
            qDebug()<<"pdfViewerUrl :"<<pdfViewerUrl.toString();

            addNewTab(pdfViewerUrl);
        }        

    });


}




