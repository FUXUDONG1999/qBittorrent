#pragma once

#include <functional>

#include <QtContainerFwd>
#include <QTreeView>

#include "base/bittorrent/infohash.h"
#include "transferlistmodel.h"

class MainWindow;

class Path;

class TransferListSortModel;

namespace BitTorrent {
    class Torrent;
}

enum class CopyInfohashPolicy {
    Version1,
    Version2
};

class TransferListWidget final : public QTreeView {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(TransferListWidget)

public:
    TransferListWidget(QWidget *parent, MainWindow *mainWindow);

    ~TransferListWidget() override;

    TransferListModel *getSourceModel() const;

public slots:

    void setSelectionCategory(const QString &category);

    void addSelectionTag(const QString &tag);

    void removeSelectionTag(const QString &tag);

    void clearSelectionTags();

    void setSelectedTorrentsLocation();

    void pauseAllTorrents();

    void resumeAllTorrents();

    void startSelectedTorrents();

    void forceStartSelectedTorrents();

    void startVisibleTorrents();

    void pauseSelectedTorrents();

    void pauseVisibleTorrents();

    void softDeleteSelectedTorrents();

    void permDeleteSelectedTorrents();

    void deleteSelectedTorrents(bool deleteLocalFiles);

    void deleteVisibleTorrents();

    void increaseQueuePosSelectedTorrents();

    void decreaseQueuePosSelectedTorrents();

    void topQueuePosSelectedTorrents();

    void bottomQueuePosSelectedTorrents();

    void copySelectedMagnetURIs() const;

    void copySelectedNames() const;

    void copySelectedInfohashes(CopyInfohashPolicy policy) const;

    void copySelectedIDs() const;

    void openSelectedTorrentsFolder() const;

    void recheckSelectedTorrents();

    void reannounceSelectedTorrents();

    void setTorrentOptions();

    void previewSelectedTorrents();

    void hideQueuePosColumn(bool hide);

    void applyFilter(const QString &name, const TransferListModel::Column &type);

    void applyStatusFilter(int f);

    void applyCategoryFilter(const QString &category);

    void applyTagFilter(const QString &tag);

    void applyTrackerFilterAll();

    void applyTrackerFilter(const QSet<BitTorrent::TorrentID> &torrentIDs);

    void previewFile(const Path &filePath);

    void renameSelectedTorrent();

signals:

    void currentTorrentChanged(BitTorrent::Torrent *torrent);

private slots:

    void torrentDoubleClicked();

    void displayListMenu();

    void displayColumnHeaderMenu();

    void currentChanged(const QModelIndex &current, const QModelIndex &) override;

    void setSelectedTorrentsSuperSeeding(bool enabled) const;

    void setSelectedTorrentsSequentialDownload(bool enabled) const;

    void setSelectedFirstLastPiecePrio(bool enabled) const;

    void setSelectedAutoTMMEnabled(bool enabled);

    void askNewCategoryForSelection();

    void saveSettings();

private:
    void wheelEvent(QWheelEvent *event) override;

    QModelIndex mapToSource(const QModelIndex &index) const;

    QModelIndexList mapToSource(const QModelIndexList &indexes) const;

    QModelIndex mapFromSource(const QModelIndex &index) const;

    bool loadSettings();

    QVector<BitTorrent::Torrent *> getSelectedTorrents() const;

    void askAddTagsForSelection();

    void editTorrentTrackers();

    void exportTorrent();

    void confirmRemoveAllTagsForSelection();

    QStringList askTagsForSelection(const QString &dialogTitle);

    void applyToSelectedTorrents(const std::function<void(BitTorrent::Torrent *const)> &fn);

    QVector<BitTorrent::Torrent *> getVisibleTorrents() const;

    int visibleColumnsCount() const;

    TransferListModel *m_listModel = nullptr;
    TransferListSortModel *m_sortFilterModel = nullptr;
    MainWindow *m_mainWindow = nullptr;
};
