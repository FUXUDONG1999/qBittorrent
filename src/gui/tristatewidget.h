#pragma once

#include <QWidget>

class QString;

class TriStateWidget final : public QWidget {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(TriStateWidget)

public:
    TriStateWidget(const QString &text, QWidget *parent);

    void setCheckState(Qt::CheckState checkState);

    void setCloseOnInteraction(bool enabled);

signals:

    void triggered(bool checked) const;

private:
    QSize minimumSizeHint() const override;

    void paintEvent(QPaintEvent *) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void toggleCheckState();

    bool m_closeOnInteraction = true;
    Qt::CheckState m_checkState = Qt::Unchecked;
    const QString m_text;
};
