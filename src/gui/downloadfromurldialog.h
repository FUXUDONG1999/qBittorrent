#pragma once

#include <QDialog>

#include "base/settingvalue.h"

namespace Ui {
    class DownloadFromURLDialog;
}

class DownloadFromURLDialog final : public QDialog {
Q_OBJECT

    Q_DISABLE_COPY_MOVE(DownloadFromURLDialog)

public:
    explicit DownloadFromURLDialog(QWidget *parent);

    ~DownloadFromURLDialog() override;

signals:

    void urlsReadyToBeDownloaded(const QStringList &torrentURLs);

private slots:

    void onSubmit();

private:
    void keyPressEvent(QKeyEvent *event) override;

    Ui::DownloadFromURLDialog *m_ui = nullptr;
    SettingValue<QSize> m_storeDialogSize;
};
