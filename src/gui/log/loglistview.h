#pragma once

#include <QListView>

class LogListView final : public QListView {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(LogListView)

public:
    explicit LogListView(QWidget *parent = nullptr);

public slots:

    void copySelection() const;

private:
    void keyPressEvent(QKeyEvent *event) override;
};
