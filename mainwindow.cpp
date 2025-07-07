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

    return topBar;
}

void MainWindow::addNewTab(const QUrl &url) {
    qDebug()<<"addNewTab()";

    QWebEngineView *view = new QWebEngineView;

    profile = view->page()->profile();
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
    qDebug()<<"onTabChanged()";

    if (tabWidget->tabText(index) == "+") {
        addNewTab();
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



