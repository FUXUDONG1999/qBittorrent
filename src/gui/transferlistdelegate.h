#pragma once

#include <QStyledItemDelegate>

#include "progressbarpainter.h"

class TransferListDelegate final : public QStyledItemDelegate {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(TransferListDelegate)

public:
    explicit TransferListDelegate(QObject *parent);

    QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    ProgressBarPainter m_progressBarPainter;
    mutable int m_nameColHeight = -1;
};
