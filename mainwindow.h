#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addNewTab(const QUrl &url = QUrl("https://bulipetech.com"));
    void onBackClicked();
    void onForwardClicked();
    void onReloadClicked();
    void onUrlEntered();
    void onSearchClicked();
    void onTabChanged(int index);
    void onUrlChanged(const QUrl &url);
    void closeTab(int index);

private:
    QWidget *createTopBar();
    QWebEngineView* currentWebView() const;

    QTabWidget *tabWidget;
    QLineEdit *urlLineEdit;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *reloadButton;
    QPushButton *searchButton;
};

#endif // MAINWINDOW_H
