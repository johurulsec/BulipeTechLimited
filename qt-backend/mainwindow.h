#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QWidget>
#include<QDebug>
#include<QResizeEvent>
class QWebEngineView;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

    void setReactView(QWebEngineView* view);
    void setContentView(QWebEngineView* view);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QWebEngineView* reactView = nullptr;
    QWebEngineView* contentView = nullptr;
};

#endif // MAINWINDOW_H
