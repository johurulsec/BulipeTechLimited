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
    addNewTab(); // Start with 1 tab

}

MainWindow::~MainWindow()
{
    qDebug()<<"Destructor of ~MainWindow()";
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
