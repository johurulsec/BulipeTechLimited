#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent)
    : QWidget{parent}
{
    // setTabsClosable(true);
    // connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::onTabCloseRequested);

}


void TabWidget::createNewTab(const QUrl &url) {
    qDebug()<<"createNewTab()";

    WebTab *tab = new WebTab;
    // addTab(tab, "New Tab");
    // setCurrentWidget(tab);
}

void TabWidget::onTabCloseRequested(int index) {
    // QWidget *widget = widget(index);
    // if (widget) {
    //     removeTab(index);
    //     delete widget;
    // }
}

