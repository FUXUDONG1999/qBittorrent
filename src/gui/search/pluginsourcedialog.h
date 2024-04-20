#pragma once

#include <QDialog>

#include "base/settingvalue.h"

namespace Ui {
    class PluginSourceDialog;
}

class PluginSourceDialog final : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(PluginSourceDialog)

public:
    explicit PluginSourceDialog(QWidget *parent = nullptr);

    ~PluginSourceDialog() override;

signals:

    void askForUrl();

    void askForLocalFile();

private slots:

    void on_localButton_clicked();

    void on_urlButton_clicked();

private:
    Ui::PluginSourceDialog *m_ui = nullptr;
    SettingValue<QSize> m_storeDialogSize;
};
