#include "hidabletabwidget.h"

#include <QTabBar>

#ifdef Q_OS_MACOS
#include <QPaintEvent>
#include <QStyle>
#endif

HidableTabWidget::HidableTabWidget(QWidget *parent)
        : QTabWidget(parent) {
}

void HidableTabWidget::tabInserted(const int index) {
    QTabWidget::tabInserted(index);
    tabBar()->setVisible(count() != 1);
}

void HidableTabWidget::tabRemoved(const int index) {
    //QTabWidget::tabInserted(index);
    QTabWidget::tabRemoved(index);
    tabBar()->setVisible(count() != 1);
}

#ifdef Q_OS_MACOS
void HidableTabWidget::paintEvent(QPaintEvent *event)
{
    // Hide the pane for macintosh style
    if (!style()->inherits("QMacStyle"))
        QTabWidget::paintEvent(event);
}
#endif
