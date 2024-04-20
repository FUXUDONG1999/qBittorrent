#pragma once

#include <QTabWidget>

#ifdef Q_OS_MACOS
class QPaintEvent;
#endif

class HidableTabWidget final : public QTabWidget {
public:
    explicit HidableTabWidget(QWidget *parent = nullptr);

private:
    void tabInserted(int index) override;

    void tabRemoved(int index) override;

#ifdef Q_OS_MACOS
    void paintEvent(QPaintEvent *event) override;
#endif
};
