#pragma once

#include <QBitArray>
#include <QtContainerFwd>

#include "piecesbar.h"

class QWidget;

class DownloadedPiecesBar final : public PiecesBar {
    using base = PiecesBar;
Q_OBJECT

    Q_DISABLE_COPY_MOVE(DownloadedPiecesBar)

public:
    DownloadedPiecesBar(QWidget *parent);

    void setProgress(const QBitArray &pieces, const QBitArray &downloadedPieces);

    // PiecesBar interface
    void clear() override;

private:
    // scale bitfield vector to float vector
    QVector<float> bitfieldToFloatVector(const QBitArray &vecin, int reqSize);

    bool updateImage(QImage &image) override;

    QString simpleToolTipText() const override;

    // incomplete piece color
    const QColor m_dlPieceColor;
    // last used bitfields, uses to better resize redraw
    // TODO: make a diff pieces to new pieces and update only changed pixels, speedup when update > 20x faster
    QBitArray m_pieces;
    QBitArray m_downloadedPieces;
};
