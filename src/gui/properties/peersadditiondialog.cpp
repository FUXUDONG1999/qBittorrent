#include "peersadditiondialog.h"

#include <QHostAddress>
#include <QMessageBox>

#include "base/bittorrent/peeraddress.h"
#include "base/global.h"
#include "ui_peersadditiondialog.h"

PeersAdditionDialog::PeersAdditionDialog(QWidget *parent)
        : QDialog(parent), m_ui(new Ui::PeersAdditionDialog()) {
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &PeersAdditionDialog::validateInput);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

PeersAdditionDialog::~PeersAdditionDialog() {
    delete m_ui;
}

QVector<BitTorrent::PeerAddress> PeersAdditionDialog::askForPeers(QWidget *parent) {
    PeersAdditionDialog dlg(parent);
    dlg.exec();
    return dlg.m_peersList;
}

void PeersAdditionDialog::validateInput() {
    if (m_ui->textEditPeers->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("No peer entered"),
                             tr("Please type at least one peer."),
                             QMessageBox::Ok);
        return;
    }
    for (const QString &peer: asConst(m_ui->textEditPeers->toPlainText().trimmed().split(u'\n'))) {
        const BitTorrent::PeerAddress addr = BitTorrent::PeerAddress::parse(peer);
        if (!addr.ip.isNull()) {
            m_peersList.append(addr);
        } else {
            QMessageBox::warning(this, tr("Invalid peer"),
                                 tr("The peer '%1' is invalid.").arg(peer),
                                 QMessageBox::Ok);
            m_peersList.clear();
            return;
        }
    }
    accept();
}
