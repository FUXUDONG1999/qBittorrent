#pragma once

#include <QTreeWidget>
#include <QtContainerFwd>

class PropertiesWidget;

namespace BitTorrent {
    class Torrent;
}

class TrackerListWidget : public QTreeWidget {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(TrackerListWidget)

public:
    enum TrackerListColumn {
        COL_TIER,
        COL_URL,
        COL_STATUS,
        COL_PEERS,
        COL_SEEDS,
        COL_LEECHES,
        COL_TIMES_DOWNLOADED,
        COL_MSG,

        COL_COUNT
    };

    explicit TrackerListWidget(PropertiesWidget *properties);

    ~TrackerListWidget();

public slots:

    void setRowColor(int row, const QColor &color);

    void moveSelectionUp();

    void moveSelectionDown();

    void clear();

    void loadStickyItems(const BitTorrent::Torrent *torrent);

    void loadTrackers();

    void copyTrackerUrl();

    void reannounceSelected();

    void deleteSelectedTrackers();

    void editSelectedTracker();

    void showTrackerListMenu();

    void loadSettings();

    void saveSettings() const;

protected:
    QVector<QTreeWidgetItem *> getSelectedTrackerItems() const;

private slots:

    void openAddTrackersDialog();

    void displayColumnHeaderMenu();

private:
    int visibleColumnsCount() const;

    void wheelEvent(QWheelEvent *event) override;

    static QStringList headerLabels();

    PropertiesWidget *m_properties = nullptr;
    QHash<QString, QTreeWidgetItem *> m_trackerItems;
    QTreeWidgetItem *m_DHTItem = nullptr;
    QTreeWidgetItem *m_PEXItem = nullptr;
    QTreeWidgetItem *m_LSDItem = nullptr;
};
