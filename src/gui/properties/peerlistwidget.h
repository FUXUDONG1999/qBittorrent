#pragma once

#include <QHash>
#include <QSet>
#include <QTreeView>

class QHostAddress;

class QStandardItem;

class QStandardItemModel;

class PeerListSortModel;

class PropertiesWidget;

struct PeerEndpoint;

namespace BitTorrent {
    class Torrent;

    class PeerInfo;
}

namespace Net {
    class ReverseResolution;
}

class PeerListWidget final : public QTreeView {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(PeerListWidget)

public:
    enum PeerListColumns {
        COUNTRY,
        IP,
        PORT,
        CONNECTION,
        FLAGS,
        CLIENT,
        PEERID_CLIENT,
        PROGRESS,
        DOWN_SPEED,
        UP_SPEED,
        TOT_DOWN,
        TOT_UP,
        RELEVANCE,
        DOWNLOADING_PIECE,
        IP_HIDDEN,

        COL_COUNT
    };

    explicit PeerListWidget(PropertiesWidget *parent);

    ~PeerListWidget() override;

    void loadPeers(const BitTorrent::Torrent *torrent);

    void updatePeerHostNameResolutionState();

    void updatePeerCountryResolutionState();

    void clear();

private slots:

    bool loadSettings();

    void saveSettings() const;

    void displayColumnHeaderMenu();

    void showPeerListMenu();

    void banSelectedPeers();

    void copySelectedPeers();

    void handleSortColumnChanged(int col);

    void handleResolved(const QHostAddress &ip, const QString &hostname) const;

private:
    void updatePeer(int row, const BitTorrent::Torrent *torrent, const BitTorrent::PeerInfo &peer, bool hideZeroValues);

    int visibleColumnsCount() const;

    void wheelEvent(QWheelEvent *event) override;

    QStandardItemModel *m_listModel = nullptr;
    PeerListSortModel *m_proxyModel = nullptr;
    PropertiesWidget *m_properties = nullptr;
    Net::ReverseResolution *m_resolver = nullptr;
    QHash<PeerEndpoint, QStandardItem *> m_peerItems;
    QList<QStandardItem *> m_I2PPeerItems;
    QHash<QHostAddress, QSet<QStandardItem *>> m_itemsByIP;  // must be kept in sync with `m_peerItems`
    bool m_resolveCountries;
};
