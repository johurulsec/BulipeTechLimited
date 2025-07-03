#ifndef WEBTAB_H
#define WEBTAB_H

#include <QWidget>
#include <QWebEngineView>
#include <QLineEdit>
#include <QToolBar>
#include<QUrl>

class WebTab : public QWidget
{
    Q_OBJECT
public:
    explicit WebTab(QWidget *parent = nullptr);

private slots:
    void navigateToUrl();
    void updateUrlBar(const QUrl &url);

private:
    QWebEngineView *view;
    QLineEdit *urlBar;
    QToolBar *navigationBar;

};

#endif // WEBTAB_H
