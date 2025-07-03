#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon("./imgs/backbutton.png"));
    ui->backButton->setStyleSheet("QPushButton { border: none; }");

    ui->forwardButton->setIcon(QIcon("./imgs/forward_button.png"));
    ui->forwardButton->setStyleSheet("QPushButton { border: none; }");

    ui->reloadButton->setIcon(QIcon("./imgs/reload_button.jpg"));
    ui->reloadButton->setStyleSheet("QPushButton { border: none; }");

    //qDebug()<<"Allah is one";
    webView = new QWebEngineView(this);
    // webView->load(QUrl("https://www.qt.io"));

    // Add web view to the vertical layout (below toolbar)
    ui->verticalLayout->addWidget(webView);

    // Connect toolbar buttons
    connect(ui->backButton, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::onForwardClicked);
    connect(ui->reloadButton, &QPushButton::clicked, this, &MainWindow::onReloadClicked);
    connect(ui->urlLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onUrlEntered);
    connect(webView, &QWebEngineView::urlChanged, this, &MainWindow::onUrlChanged);

    connect(ui->backButton, &QPushButton::clicked, webView, &QWebEngineView::back);

    // Load default home page
    //webView->load(QUrl("https://bulipetech.com/"));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onBackClicked()
{
    qDebug()<<"onBackClicked()";

    webView->back();
}

void MainWindow::onForwardClicked()
{
    qDebug()<<"onForwardClicked()";

    webView->forward();
}

void MainWindow::onReloadClicked()
{
    qDebug()<<"onReloadClicked()";

    webView->reload();
}

void MainWindow::onUrlEntered()
{
    qDebug()<<"onUrlEntered()";

    QUrl url = QUrl::fromUserInput(ui->urlLineEdit->text());
    webView->load(url);
}

void MainWindow::onUrlChanged(const QUrl &url)
{
    qDebug()<<"onUrlChanged()";

    ui->urlLineEdit->setText(url.toString());
}

void MainWindow::on_pushButton_clicked()
{
    // QString url_prefix = "https://";
    // //https://bulipetech.com/
    QString url_user_input = ui->urlLineEdit->text();
    qDebug()<<"url_user_input : "<<url_user_input;

    // if(url_user_input == "google"){
    //     url_prefix +="www.";
    // }

    // QString url_postfix = ".com";
    // QString url_final_str = url_prefix+url_user_input+ url_postfix;
    // qDebug()<<"url final str: "<<url_final_str;

    // QUrl url = QUrl::fromUserInput(url_final_str);
    QUrl url = QUrl::fromUserInput(url_user_input);
    webView->load(url);

    //setCentralWidget(webView);

}

