#include "trackerentriesdialog.h"

#include <algorithm>

#include <QHash>
#include <QVector>

#include "base/bittorrent/trackerentry.h"
#include "ui_trackerentriesdialog.h"
#include "utils.h"

#define SETTINGS_KEY(name) u"TrackerEntriesDialog/" name

TrackerEntriesDialog::TrackerEntriesDialog(QWidget *parent)
        : QDialog(parent), m_ui(new Ui::TrackerEntriesDialog), m_storeDialogSize(SETTINGS_KEY(u"Size"_s)) {
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    loadSettings();
}

TrackerEntriesDialog::~TrackerEntriesDialog() {
    saveSettings();

    delete m_ui;
}

void TrackerEntriesDialog::setTrackers(const QVector<BitTorrent::TrackerEntry> &trackers) {
    int maxTier = -1;
    QHash<int, QString> tiers;  // <tier, tracker URLs>

    for (const BitTorrent::TrackerEntry &entry: trackers) {
        tiers[entry.tier] += (entry.url + u'\n');
        maxTier = std::max(maxTier, entry.tier);
    }

    QString text = tiers.value(0);

    for (int i = 1; i <= maxTier; ++i)
        text += (u'\n' + tiers.value(i));

    m_ui->plainTextEdit->setPlainText(text);
}

QVector<BitTorrent::TrackerEntry> TrackerEntriesDialog::trackers() const {
    return BitTorrent::parseTrackerEntries(m_ui->plainTextEdit->toPlainText());
}

void TrackerEntriesDialog::saveSettings() {
    m_storeDialogSize = size();
}

void TrackerEntriesDialog::loadSettings() {
    if (const QSize dialogSize = m_storeDialogSize; dialogSize.isValid())
        resize(dialogSize);
}
