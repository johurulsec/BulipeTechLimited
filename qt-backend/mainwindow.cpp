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
    qDebug()<<"setReactView()";
    reactView = view;
}

void MainWindow::setContentView(QWebEngineView* view) {
    qDebug()<<"setContentView()";
    contentView = view;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    qDebug()<<"resizeEvent()";

    QWidget::resizeEvent(event);
    if (reactView) reactView->resize(size());
    if (contentView && contentView->isVisible()) contentView->resize(size()); // Optional
}
