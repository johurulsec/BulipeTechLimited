//mainwindow.cpp
#include "mainwindow.h"
#include <QWebEngineView>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    qDebug()<<"constr mw";
    setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::setReactView(QWebEngineView* view) {
    qDebug()<<"setReactView() mw";
    reactView = view;
}

void MainWindow::setContentView(QWebEngineView* view) {
    qDebug()<<"setContentView() mw";
    contentView = view;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    qDebug()<<"resizeEvent() mw";

    QWidget::resizeEvent(event);
    if (reactView) reactView->resize(size());
    if (contentView && contentView->isVisible()) contentView->resize(size()); // Optional
}

