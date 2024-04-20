#include "tristateaction.h"

#include <QString>
#include <QWidget>

#include "tristatewidget.h"

TriStateAction::TriStateAction(const QString &text, QWidget *parent)
        : QWidgetAction{parent}, m_triStateWidget{new TriStateWidget{text, parent}} {
    setCheckable(true);

    // required for QAction::setChecked(bool) to work
    connect(this, &QAction::toggled, this, [this](const bool checked) {
        m_triStateWidget->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    });

    connect(m_triStateWidget, &TriStateWidget::triggered, this, &QAction::toggled);
    setDefaultWidget(m_triStateWidget);
}

void TriStateAction::setCheckState(const Qt::CheckState checkState) {
    QWidgetAction::setChecked((checkState == Qt::Checked) ? true : false);
    m_triStateWidget->setCheckState(checkState);
}

void TriStateAction::setCloseOnInteraction(const bool enabled) {
    m_triStateWidget->setCloseOnInteraction(enabled);
}
