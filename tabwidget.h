#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include "webtab.h"

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    void createNewTab(const QUrl &url = QUrl("https://www.qt.io"));

private slots:
    void onTabCloseRequested(int index);

};

#endif // TABWIDGET_H
