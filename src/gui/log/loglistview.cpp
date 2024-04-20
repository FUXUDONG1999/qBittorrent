#include "loglistview.h"

#include <QtGlobal>
#include <QApplication>
#include <QClipboard>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QPainter>
#include <QStyle>
#include <QStyledItemDelegate>

#include "base/global.h"
#include "logmodel.h"

#ifdef Q_OS_WIN

#include "base/preferences.h"

#endif

namespace {
    const QString SEPARATOR = u" - "_s;

    int horizontalAdvance(const QFontMetrics &fontMetrics, const QString &text) {
        return fontMetrics.horizontalAdvance(text);
    }

    QString logText(const QModelIndex &index) {
        return index.data(BaseLogModel::TimeRole).toString()
               + SEPARATOR
               + index.data(BaseLogModel::MessageRole).toString();
    }

    class LogItemDelegate final : public QStyledItemDelegate {
    public:
        explicit LogItemDelegate(QObject *parent = nullptr)
                : QStyledItemDelegate(parent)
#ifdef Q_OS_WIN
                , m_useCustomUITheme(Preferences::instance()->useCustomUITheme())
#endif
        {
        }

    private:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
            painter->save();
            QStyledItemDelegate::paint(painter, option, index); // paints background, focus rect and selection rect

            const QStyle *style = option.widget ? option.widget->style() : QApplication::style();
            const QRect textRect = option.rect.adjusted(1, 0, 0, 0); // shift 1 to avoid text being too close to focus rect
            const bool isEnabled = option.state.testFlag(QStyle::State_Enabled);

#ifdef Q_OS_WIN
            // Windows default theme do not use highlighted text color
            const QPalette::ColorRole textRole = m_useCustomUITheme
                                                 ? (option.state.testFlag(QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::WindowText)
                                                 : QPalette::WindowText;
#else
            const QPalette::ColorRole textRole = option.state.testFlag(QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::WindowText;
#endif

            // for unknown reasons (fixme) painter won't accept some font properties
            // until they are set explicitly, and we have to manually set some font properties
            QFont font = option.font;
            font.setFamily(option.font.family());
            if (option.font.pointSizeF() > 0) // for better scaling we use floating point version
                font.setPointSizeF(option.font.pointSizeF());
            painter->setFont(font);

            QPalette palette = option.palette;

            const QString time = index.data(BaseLogModel::TimeRole).toString();
            palette.setColor(QPalette::Active, QPalette::WindowText, index.data(BaseLogModel::TimeForegroundRole).value<QColor>());
            style->drawItemText(painter, textRect, option.displayAlignment, palette, isEnabled, time, textRole);

            const QFontMetrics fontMetrics = painter->fontMetrics(); // option.fontMetrics adds extra padding to QFontMetrics::width
            const int separatorCoordinateX = horizontalAdvance(fontMetrics, time);
            style->drawItemText(painter, textRect.adjusted(separatorCoordinateX, 0, 0, 0), option.displayAlignment, option.palette, isEnabled, SEPARATOR,
                                textRole);

            const int messageCoordinateX = separatorCoordinateX + horizontalAdvance(fontMetrics, SEPARATOR);
            const QString message = index.data(BaseLogModel::MessageRole).toString();
            palette.setColor(QPalette::Active, QPalette::WindowText, index.data(BaseLogModel::MessageForegroundRole).value<QColor>());
            style->drawItemText(painter, textRect.adjusted(messageCoordinateX, 0, 0, 0), option.displayAlignment, palette, isEnabled, message, textRole);

            painter->restore();
        }

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
            const QSize minimumFontPadding(4, 4);
            const QSize fontSize = option.fontMetrics.size(0, logText(index)) + minimumFontPadding;
            const QSize defaultSize = QStyledItemDelegate::sizeHint(option, index);
            const QSize margins = (defaultSize - fontSize).expandedTo({0, 0});
            return fontSize + margins;
        }

#ifdef Q_OS_WIN
        const bool m_useCustomUITheme = false;
#endif
    };
}

LogListView::LogListView(QWidget *parent)
        : QListView(parent) {
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setItemDelegate(new LogItemDelegate(this));

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
}

void LogListView::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Copy))
        copySelection();
    else
        QListView::keyPressEvent(event);
}

void LogListView::copySelection() const {
    QStringList list;
    const QModelIndexList selectedIndexes = selectionModel()->selectedRows();
    for (const QModelIndex &index: selectedIndexes)
        list.append(logText(index));
    QApplication::clipboard()->setText(list.join(u'\n'));
}
