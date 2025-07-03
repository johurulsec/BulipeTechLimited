#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "tabwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onBackClicked();
    void onForwardClicked();
    void onReloadClicked();
    void onUrlEntered();
    void onUrlChanged(const QUrl &url);

private:
    Ui::MainWindow *ui;
    TabWidget *tabWidget;
    QWebEngineView *webView;
};
#endif // MAINWINDOW_H
