#pragma once

#include <QStatusBar>

class QLabel;

class QPushButton;

namespace BitTorrent {
    struct SessionStatus;
}

class StatusBar : public QStatusBar {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(StatusBar)

public:
    StatusBar(QWidget *parent = nullptr);

    ~StatusBar() override;

signals:

    void alternativeSpeedsButtonClicked();

    void connectionButtonClicked();

public slots:

    void showRestartRequired();

private slots:

    void refresh();

    void updateAltSpeedsBtn(bool alternative);

    void capSpeed();

private:
    void updateConnectionStatus();

    void updateDHTNodesNumber();

    void updateSpeedLabels();

    QPushButton *m_dlSpeedLbl = nullptr;
    QPushButton *m_upSpeedLbl = nullptr;
    QLabel *m_DHTLbl = nullptr;
    QPushButton *m_connecStatusLblIcon = nullptr;
    QPushButton *m_altSpeedsBtn = nullptr;
};
