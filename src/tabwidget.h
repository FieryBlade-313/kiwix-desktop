#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTableWidget>
#include <memory>
#include "webview.h"

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget* parent=nullptr);

    WebView* createNewTab(bool setCurrent);
    void openUrl(const QUrl &url, bool newTab);
    void setTitleOf(WebView* webView, const QString& title);
    void setIconOf(WebView* webView, const QIcon& icon);

public slots:
    void closeTab(int index);
};

#endif // TABWIDGET_H
