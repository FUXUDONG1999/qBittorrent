#pragma once

#include <QDialog>
#include <QVector>

#include "base/bittorrent/peerinfo.h"

namespace Ui {
    class PeersAdditionDialog;
}

class PeersAdditionDialog : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(PeersAdditionDialog)

public:
    PeersAdditionDialog(QWidget *parent);

    ~PeersAdditionDialog();

    static QVector<BitTorrent::PeerAddress> askForPeers(QWidget *parent);

protected slots:

    void validateInput();

private:
    Ui::PeersAdditionDialog *m_ui = nullptr;
    QVector<BitTorrent::PeerAddress> m_peersList;
};
