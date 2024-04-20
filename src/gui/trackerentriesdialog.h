#pragma once

#include <QDialog>
#include <QtContainerFwd>

#include "base/settingvalue.h"

namespace BitTorrent {
    struct TrackerEntry;
}

namespace Ui {
    class TrackerEntriesDialog;
}

class TrackerEntriesDialog : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(TrackerEntriesDialog)

public:
    explicit TrackerEntriesDialog(QWidget *parent);

    ~TrackerEntriesDialog() override;

    void setTrackers(const QVector<BitTorrent::TrackerEntry> &trackers);

    QVector<BitTorrent::TrackerEntry> trackers() const;

private:
    void saveSettings();

    void loadSettings();

    Ui::TrackerEntriesDialog *m_ui = nullptr;
    SettingValue<QSize> m_storeDialogSize;
};
