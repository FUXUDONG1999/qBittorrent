#pragma once

#include <QWidgetAction>

class QString;

class QWidget;

class TriStateWidget;

// TriStateWidget is responsible for checkbox state (tri-state) and paint events while
// TriStateAction will keep in sync with it. This allows connecting with the usual
// QAction::triggered or QAction::toggled slots.
class TriStateAction : public QWidgetAction {
public:
    TriStateAction(const QString &text, QWidget *parent);

    // should use this function instead of QAction::setChecked(bool)
    void setCheckState(Qt::CheckState checkState);

    // When set to 'true' toggling the checkbox will emit a signal on
    // QAction::triggered. When placed in a QMenu, this causes the menu to close.
    // When set to 'false' emits a signal on QAction::toggled instead, leaving the
    // menu open.
    // Default value: 'true'.
    void setCloseOnInteraction(bool enabled);

private:
    TriStateWidget *m_triStateWidget = nullptr;
};
