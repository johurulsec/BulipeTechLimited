#include "webtab.h"
#include <QVBoxLayout>
#include <QAction>

WebTab::WebTab(QWidget *parent)
    : QWidget{parent}
{
    view = new QWebEngineView(this);
    urlBar = new QLineEdit(this);
    navigationBar = new QToolBar(this);

    QAction *back = navigationBar->addAction("Back");
    QAction *forward = navigationBar->addAction("Forward");
    QAction *reload = navigationBar->addAction("Reload");
    QAction *home = navigationBar->addAction("Home");

    navigationBar->addWidget(urlBar);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(navigationBar);
    layout->addWidget(view);
    setLayout(layout);

    connect(urlBar, &QLineEdit::returnPressed, this, &WebTab::navigateToUrl);
    connect(view, &QWebEngineView::urlChanged, this, &WebTab::updateUrlBar);
    connect(back, &QAction::triggered, view, &QWebEngineView::back);
    connect(forward, &QAction::triggered, view, &QWebEngineView::forward);
    connect(reload, &QAction::triggered, view, &QWebEngineView::reload);
    connect(home, &QAction::triggered, [this]() {
        view->load(QUrl("https://www.qt.io"));
    });

    view->load(QUrl("https://www.qt.io"));
}


void WebTab::navigateToUrl() {
    QUrl url = QUrl::fromUserInput(urlBar->text());
    view->load(url);
}

void WebTab::updateUrlBar(const QUrl &url) {
    urlBar->setText(url.toString());
}

