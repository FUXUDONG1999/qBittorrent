#pragma once

#include <QDialog>

#include "base/settingvalue.h"

namespace Ui {
    class AboutDialog;
}

class AboutDialog final : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(AboutDialog)

public:
    explicit AboutDialog(QWidget *parent);

    ~AboutDialog() override;

private:
    void copyVersionsToClipboard() const;

    Ui::AboutDialog *m_ui = nullptr;
    SettingValue<QSize> m_storeDialogSize;
};
