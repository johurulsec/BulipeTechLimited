#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>
#include<QWebEngineProfile>
#include<QWebEnginePage>

#include <QMenuBar>
#include <QMenu>
#include <QStringList>
#include <QSettings>

#include <QDockWidget>
#include <QListWidget>

//bookmarks
#include <QMap>

//#include <QWebEngineDownloadItem>
#include <QWebEngineDownloadRequest>  // Changed from QWebEngineDownloadItem
#include <QProgressBar>
#include<QMessageBox>
#include<QFileDialog>
#include<QStandardPaths>
#include<QDesktopServices>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QDockWidget *downloadDock;
    QListWidget *downloadListWidget;
    QMap<QWebEngineDownloadRequest*, QProgressBar*> downloadProgressBars;
    QMenu *downloadsMenu;
    QMap<QWebEngineDownloadRequest*, QAction*> downloadActions;


public slots:
    void addToHistory(const QUrl &url);
    void updateHistoryMenu();
    void setupHistoryDock();

    void addToBookmarks(const QUrl &url);
    void updateBookmarksMenu();

    void setupDownloadManager();
    void handleDownload(QWebEngineDownloadRequest *download);

    // void handleDownloadRequested(QWebEngineDownloadRequest *download);

private slots:
    void addNewTab(const QUrl &url = QUrl("https://www.google.com"));
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

    QPushButton *menuButton;
    QMenu *dropdownMenu;

    QWebEngineProfile *profile;

    //for history
    QMenuBar *menuBar;
    QMenu *historyMenu;
    QStringList historyList;
    QDockWidget *historyDock;
    QListWidget *historyListWidget;

    //for bookmarks
    QStringList bookmarksList;
    QMenu *bookmarksMenu;
    QPushButton *bookmarkStarButton;

    QWebEngineView* webView;

};

#endif // MAINWINDOW_H
