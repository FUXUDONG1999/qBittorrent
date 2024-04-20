#include "transferlistdelegate.h"

#include <QModelIndex>

#include "transferlistmodel.h"

TransferListDelegate::TransferListDelegate(QObject *parent)
        : QStyledItemDelegate{parent} {
}

QWidget *TransferListDelegate::createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const {
    // No editor here
    return nullptr;
}

QSize TransferListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Reimplementing sizeHint() because the 'name' column contains text+icon.
    // When that WHOLE column goes out of view(eg user scrolls horizontally)
    // the rows shrink if the text's height is smaller than the icon's height.
    // This happens because icon from the 'name' column is no longer drawn.

    if (m_nameColHeight == -1) {
        const QModelIndex nameColumn = index.sibling(index.row(), TransferListModel::TR_NAME);
        m_nameColHeight = QStyledItemDelegate::sizeHint(option, nameColumn).height();
    }

    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(std::max(m_nameColHeight, size.height()));
    return size;
}

void TransferListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    switch (index.column()) {
        case TransferListModel::TR_PROGRESS: {
            using namespace BitTorrent;

            const auto isEnableState = [](const TorrentState state) -> bool {
                switch (state) {
                    case TorrentState::Error:
                    case TorrentState::PausedDownloading:
                    case TorrentState::Unknown:
                        return false;
                    default:
                        return true;
                }
            };

            const int progress = static_cast<int>(index.data(TransferListModel::UnderlyingDataRole).toReal());

            const QModelIndex statusIndex = index.siblingAtColumn(TransferListModel::TR_STATUS);
            const auto torrentState = statusIndex.data(TransferListModel::UnderlyingDataRole).value<TorrentState>();

            QStyleOptionViewItem customOption{option};
            customOption.state.setFlag(QStyle::State_Enabled, isEnableState(torrentState));

            m_progressBarPainter.paint(painter, customOption, index.data().toString(), progress);
        }
            break;
        default:
            QStyledItemDelegate::paint(painter, option, index);
            break;
    }
}
